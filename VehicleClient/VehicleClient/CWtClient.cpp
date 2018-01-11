#include "stdafx.h"
#include "CWtClient.h"
#include <Wt/WTable.h>
#include <Wt/WServer.h>


#include <string>

namespace WtClient
{
	void CWtClient::DoGetResults(http_response& response)
	{

		m_Timer->stop();

		m_VehTable->clear();
		m_VehTable->setHeaderCount(1);
		m_VehTable->setWidth(Wt::WLength("100%"));

		if (response.status_code() != status_codes::OK)		// Something wrong - print status
		{
			std::stringstream strresp;
			strresp << "Error getting data. Status code: " << response.status_code();
			m_VehTable->elementAt(0, 0)->addWidget(Wt::cpp14::make_unique<Wt::WText>(strresp.str()));
			return;
		}

		// Status = OK
		// Render results
		// 1. Create table header
		m_VehTable->elementAt(0, 0)->addWidget(Wt::cpp14::make_unique<Wt::WText>("Registration"));
		m_VehTable->elementAt(0, 1)->addWidget(Wt::cpp14::make_unique<Wt::WText>("Make"));
		m_VehTable->elementAt(0, 2)->addWidget(Wt::cpp14::make_unique<Wt::WText>("Model"));
		m_VehTable->elementAt(0, 3)->addWidget(Wt::cpp14::make_unique<Wt::WText>("Owner"));

		// 2. Render results
		auto jsonreply = response.extract_json().get();

		// Cycle thru JSON reply, extract data and fill in table
		int i = 1;
		for (auto iter = jsonreply.as_array().cbegin(); iter != jsonreply.as_array().cend(); ++iter, ++i)
		{
			m_VehTable->elementAt(i, 0)->addWidget(Wt::cpp14::make_unique<Wt::WText>(iter->as_object().at(L"Vehicle_Registration").as_string()));
			m_VehTable->elementAt(i, 1)->addWidget(Wt::cpp14::make_unique<Wt::WText>(iter->as_object().at(L"Vehicle_Make").as_string()));
			m_VehTable->elementAt(i, 2)->addWidget(Wt::cpp14::make_unique<Wt::WText>(iter->as_object().at(L"Vehicle_Model").as_string()));
			m_VehTable->elementAt(i, 3)->addWidget(Wt::cpp14::make_unique<Wt::WText>(iter->as_object().at(L"Vehicle_Owner").as_string()));
		}
		
		// 3. We may not called from main loop. Call triggerUpdate, so Wt update page
		triggerUpdate();
	}

	void CWtClient::DoSearch()
	{
		// Clear table, and pring Searching...
		m_VehTable->clear();
		m_VehTable->setHeaderCount(1);
		m_VehTable->setWidth(Wt::WLength("100%"));
		m_VehTable->elementAt(0, 0)->addWidget(Wt::cpp14::make_unique<Wt::WText>("Searching..."));

		// Give ReqTimeout seconds timeout to Request
		m_Timer->setSingleShot(true);
		m_Timer->start();

		// Invoke CHTTPClient::SendRequest to send request to server
		m_httpclient->SendRequest(m_VehReg->text()
			, m_VehMake->text()
			, m_VehModel->text()
			, m_VehOwner->text()
		// Last parameter - lambda will be executed on response, and post event to fill in table with results (DoGetResults will be called for this)
			, [this](http_response& response)
		{
			// This lambda will be called in different thread, so
			//   use post method to post event to msg query 
			// We cannot manpulate widgets in different thread w/o lock
			Wt::WServer::instance()->post(sessionId(), std::bind(&WtClient::CWtClient::DoGetResults, this, response));
		});
	}

	void CWtClient::InitWebPage(const Wt::WEnvironment& env)
	{
		enableUpdates(true);	// Enable Widgets updates outside event loop
								// This is for updating page when HTTP respond from server comes
		
		setTitle("Stolen Vehicles search page");    // application title 
		
		root()->addWidget(Wt::cpp14::make_unique<Wt::WText>("This page is used to search thru stolen vehicles database. Enter any fields below and press Search.")); // show some text 
		root()->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());    
		root()->addWidget(Wt::cpp14::make_unique<Wt::WText>("Vehicle Registration: ")); 
		m_VehReg = root()->addWidget(Wt::cpp14::make_unique<Wt::WLineEdit>());  
		m_VehReg->setFocus();                              // give focus 
		
		root()->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());   
		root()->addWidget(Wt::cpp14::make_unique<Wt::WText>("Vehicle Make: "));  
		m_VehMake = root()->addWidget(Wt::cpp14::make_unique<Wt::WLineEdit>());

		root()->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());    
		root()->addWidget(Wt::cpp14::make_unique<Wt::WText>("Vehicle Model: "));  
		m_VehModel = root()->addWidget(Wt::cpp14::make_unique<Wt::WLineEdit>());  

		root()->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());    
		root()->addWidget(Wt::cpp14::make_unique<Wt::WText>("Vehicle Owner: ")); 
		m_VehOwner = root()->addWidget(Wt::cpp14::make_unique<Wt::WLineEdit>()); 


		auto button = root()->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>("Search"));
		button->setMargin(5, Wt::Side::Left);                    
		button->clicked().connect(this, &CWtClient::DoSearch);

		root()->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());    
		m_VehTable = root()->addWidget(Wt::cpp14::make_unique<Wt::WTable>());

		// Configure timer for timeouts
		m_Timer = root()->addChild(std::make_unique<Wt::WTimer>());
		m_Timer->setInterval(ReqTimeout);
		m_Timer->timeout().connect(std::bind(&WtClient::CWtClient::DoGetResults, this, http_response(status_codes::RequestTimeout)));
	}

	CWtClient::CWtClient(const Wt::WEnvironment& env) 
		: Wt::WApplication(env)
		, m_httpclient(new (HTTPClient::CHTTPClient))
	{
		InitWebPage(env);
		m_httpclient->InitClient();
	}


	CWtClient::~CWtClient()
	{
		root()->clear();
	}

}
