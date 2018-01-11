#include "stdafx.h"
#include "CMnConn.h"

#include <memory>

#include <codecvt>		// for std::codecvt

namespace MnConn
{
	web::json::value CMnConn::QueryDB(web::json::value& request)
	{
		assert(m_client);
		assert(m_VehColl);
		assert(m_LogColl);

		utility::stringstream_t stream;
		request.serialize(stream);

		// mongodbcxx accepts UTF8 string, serialize gives UTF16 string. We need to convert
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> wcharConv;

		// 1. Prep query
		auto query_value = bsoncxx::from_json(wcharConv.to_bytes(stream.str()));
		// We should catch in case of malformed JSON request...
		
		// 2. Log query
		bsoncxx::document::value LogEntry = bsoncxx::builder::stream::document{}
			<< "ReqDateTime" 
		// here we getting DateTime in milleseconds for b_date constructor 
			<< bsoncxx::types::b_date{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) }
			<< "Query" << wcharConv.to_bytes(stream.str())
			<< bsoncxx::builder::stream::finalize;
		
		m_LogColl->insert_one(LogEntry.view());

		// 3. Now query collection
		auto vehcursor = m_VehColl->find(query_value.view());

		// 4. Going to prepare respond
		std::stringstream strreply;
		strreply << "["; // start array
		for (mongocxx::cursor::iterator iter = vehcursor.begin();
			iter != vehcursor.end();) 
		{
			strreply << bsoncxx::to_json(*iter);
			if (++iter != vehcursor.end())
				strreply << ",";
		}
		strreply << "]"; // finish array

		return web::json::value::parse(strreply);
	}

	void CMnConn::Connect()
	{
		m_client.reset(new mongocxx::client(mongocxx::uri(MDBConnStr)));
		m_VehColl = std::make_unique<mongocxx::collection>((*m_client)["StolenVehDB"]["Vehicle"]);
		m_LogColl = std::make_unique<mongocxx::collection>((*m_client)["StolenVehDB"]["Log"]);
	}

	CMnConn::CMnConn()
		: m_inst(new mongocxx::instance())
		, m_client(nullptr)		// We're going to connect in Connect member function
		, m_VehColl(nullptr)
		, m_LogColl(nullptr)
	{
		
	}


	CMnConn::~CMnConn()
	{
	}
}
