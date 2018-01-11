#pragma once

#include <memory>
#include "CHTTPClient.h"

#include <cpprest/json.h>

#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTable.h>
#include <Wt/WTableCell.h>
#include <Wt/WTimer.h>

namespace WtClient 
{

	class CWtClient : public Wt::WApplication
	{
	public:
		CWtClient(const Wt::WEnvironment& env);
		~CWtClient();

	private:
		// We don't own resources, so raw pointers
		// WApplication::clear removes all these resources.
		Wt::WLineEdit* m_VehReg;
		Wt::WLineEdit* m_VehMake;
		Wt::WLineEdit* m_VehModel;
		Wt::WLineEdit* m_VehOwner;
		Wt::WTable* m_VehTable;		// Table for results output
		Wt::WTimer* m_Timer;
		
		std::unique_ptr<HTTPClient::CHTTPClient> m_httpclient;

		void InitWebPage(const Wt::WEnvironment& enc);
		void DoSearch();
		void DoGetResults(http_response& response);

	};

}

