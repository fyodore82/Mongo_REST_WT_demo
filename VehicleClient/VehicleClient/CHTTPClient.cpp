#include "stdafx.h"
#include "CHTTPClient.h"

#include <string>
#include <memory>
#include <cpprest/http_client.h>
#include <cpprest/json.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features

namespace HTTPClient
{
	void CHTTPClient::InitClient()
	{
		m_client.reset(new http_client(ServerURI));
	}

	void CHTTPClient::SendRequest(const std::wstring& Reg
		, const std::wstring& Make
		, const std::wstring& Model
		, const std::wstring& Owner
		, std::function<void(http_response&)> const& FuncCallback)
	{
		try
		{
			TRACE("\n\nSend request --------\n\n");

			// 1. Prepare JSON request from fields
			std::wstringstream reqstream;
			reqstream << L"{";
			if (Make != L"")	// skip empty fields
				reqstream << L"\"Vehicle_Make\":\"" << Make << L"\"";
			if (Model != L"")	// skip empty fields
				reqstream << (reqstream.str().length() > 1 ? "," : "")
				<< "\"Vehicle_Model\":\"" << Model << L"\"";
			if (Owner != L"")	// skip empty fields
				reqstream << (reqstream.str().length() > 1 ? "," : "")
				<< "\"Vehicle_Owner\":\"" << Owner << L"\"";
			if (Reg != L"")		// skip empty fields
				reqstream << (reqstream.str().length() > 1 ? "," : "")
				<< "\"Vehicle_Registration\":\"" << Reg << L"\"";

			if (reqstream.str().length() <= 1)
				return;	// All fields empty. We're not going to send empty query
			reqstream << L"}";

			// Print request to output (this is good practice for support app in production).
			// Maintanace team will see that server forms and sends HTTP requests
			std::wcout << "\nRequest: " << reqstream.str() << "\n";

			m_taskinprogress.release();	// Drop any incoplete task
			m_taskinprogress =
				std::make_unique<pplx::task<void>>(m_client->request(methods::POST
				, PathURI
				, static_cast<utf16string>(reqstream.str())
				, L"application/json")// , m_ctstoken.get_token());
				.then([=](http_response response)
			{
				FuncCallback(response);
			}));
			TRACE("Request sent --------\n");
		}
		catch (std::exception& e)
		{
			std::cout << "\n\nSendReq exception: " << e.what() << "\n\n\n";
		}
	}

	CHTTPClient::CHTTPClient()
		: m_client(nullptr)
		, m_taskinprogress(nullptr)
	{
	}


	CHTTPClient::~CHTTPClient()
	{
	}

}