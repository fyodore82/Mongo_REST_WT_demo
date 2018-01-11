#pragma once
#include <string>
#include <memory>
#include <cpprest/http_client.h>
#include <cpprest/json.h>

#ifdef EXPORT_TEST_FUNCTIONS

#define VEHCLIENT_UNITTESTAPP_EXPORT __declspec(dllexport)
#else
#define VEHCLIENT_UNITTESTAPP_EXPORT
#endif

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features

namespace HTTPClient
{

	class CHTTPClient
	{
	public:
		VEHCLIENT_UNITTESTAPP_EXPORT void InitClient();
		VEHCLIENT_UNITTESTAPP_EXPORT void SendRequest(const std::wstring& Reg
			, const std::wstring& Make
			, const std::wstring& Model
			, const std::wstring& Owner
			, std::function<void(http_response&)> const& FuncCallback);
		VEHCLIENT_UNITTESTAPP_EXPORT void WaitForRequestUnitTest()
		{
			m_taskinprogress->wait();
		}

		VEHCLIENT_UNITTESTAPP_EXPORT CHTTPClient();
		VEHCLIENT_UNITTESTAPP_EXPORT ~CHTTPClient();
	private:

		std::unique_ptr<http_client> m_client;
		
		// Seems that pplx or REST API has a bug.
		// https://github.com/Microsoft/cpprestsdk/issues/207
		// If request times out, task hangs in running state and never finished (cancellation token doesn't work)
		// We'll use unique_ptr wrapper to terminate hanged task
		std::unique_ptr<pplx::task<void>> m_taskinprogress;

	};

}