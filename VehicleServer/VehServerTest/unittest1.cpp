#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\VehicleServer\Constants.h"
#include "..\VehicleServer\CMnConn.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace VehServerTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMongoConnect)
		{
			// Test connection to MongoDB
			std::unique_ptr<MnConn::CMnConn> mnconn(new MnConn::CMnConn());
			mnconn->Connect();

			// Preapre JSON like in request
			utility::stringstream_t ss1;
			ss1 << "{ \"Vehicle_Make\" : \"toyota\" }";
			
			web::json::value jsonreq = web::json::value::parse(ss1);
			web::json::value reply = mnconn -> QueryDB(jsonreq);

			Assert::AreEqual(reply.size() > 0, static_cast<bool>(1), L"DB doesn't return response...");
		}

	};
}