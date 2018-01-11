#include "stdafx.h"
#include "HTTPSrv.h"
#include "CMnConn.h"

#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>
#undef U // due to bug in cpprest https://github.com/Microsoft/vcpkg/issues/836

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


namespace HTTPSrv
{
	// For POST handler we'll use lambda, which call MnConn::CMnConn::QueryDB to get data from db
	void CHTTPSrv::InitListerner(MnConn::CMnConn& conn)
	{
		m_listener.reset(new http_listener(HTTPServerURI));
		m_listener->support(methods::POST, [&conn](http_request& request)
		{
			TRACE("handle POST");
			// Prepare json body and send to query
			json::value jsonreq = request.extract_json().get();
			web::json::value reply = conn.QueryDB(jsonreq);
			
			// This code is not strictly needed, but helps understand that server is processing requests correctly
			//  For production, such "logging" greatly helps investigate misconfiguration (of net, server, etc.)
			std::stringstream str;
			str  << " Request size " << jsonreq.size() << "Reply size " << reply.size();
			TRACE(str.str());
			utility::stringstream_t stream;
			jsonreq.serialize(stream);
			wcout << "\n Serilize request " << stream.str() << "\n";

			utility::stringstream_t stream1;
			reply.serialize(stream1);
			wcout << "\n Serilize reply " << stream1.str() << "\n";

			request.reply(status_codes::OK, reply);
		});
	}

	void CHTTPSrv::StartListerner()
	{
		m_listener
			->open()
			.then([]() {TRACE("starting to listen\n"); })
			.wait();
	}

	void CHTTPSrv::StopListerner()
	{
		m_listener->close()
			.then([]() {TRACE("stopping listener\n"); })
			.wait();
	}

	CHTTPSrv::CHTTPSrv() :
		m_listener(nullptr)
	{
	}


	CHTTPSrv::~CHTTPSrv()
	{
	}

}