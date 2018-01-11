#pragma once

#include <memory>
#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/exception.hpp>

#include <cpprest/json.h>

#ifdef EXPORT_TEST_FUNCTIONS

#define VEHCLIENT_UNITTESTAPP_EXPORT __declspec(dllexport)
#else
#define VEHCLIENT_UNITTESTAPP_EXPORT
#endif

namespace MnConn
{
	class CMnConn
	{
	public:
		VEHCLIENT_UNITTESTAPP_EXPORT void Connect();
		VEHCLIENT_UNITTESTAPP_EXPORT web::json::value QueryDB(web::json::value& request);
		VEHCLIENT_UNITTESTAPP_EXPORT CMnConn();
		VEHCLIENT_UNITTESTAPP_EXPORT ~CMnConn();
	private:
		std::unique_ptr<mongocxx::instance> m_inst;
		std::unique_ptr<mongocxx::client> m_client;
		std::unique_ptr<mongocxx::collection> m_VehColl;
		std::unique_ptr<mongocxx::collection> m_LogColl;
	};
}

