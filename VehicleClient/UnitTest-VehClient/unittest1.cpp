#include "stdafx.h"
#include "CppUnitTest.h"
#define EXPORT_TEST_FUNCTIONS
#include "..\VehicleClient\CHTTPClient.h"
#include <memory>
#include <string>
#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestVehClient
{		
	TEST_CLASS(VehicleClientTest)
	{
	public:
		
		TEST_METHOD(SendRequestTest)
		{
			// This test completes only when Server App started!!!
			std::unique_ptr<HTTPClient::CHTTPClient> httpclient(new HTTPClient::CHTTPClient);
			httpclient->InitClient();
			httpclient->SendRequest(
				L"",
				L"toyota",
				L"",
				L"",
				[](http_response& response)
				{
					Assert::AreEqual(response.extract_json().get().size() > 0, static_cast<bool>(true), L"Server returns no results. This can be due to no Make:Toyota in Vehicle collection");
				});
			httpclient->WaitForRequestUnitTest();
		}

	};
}