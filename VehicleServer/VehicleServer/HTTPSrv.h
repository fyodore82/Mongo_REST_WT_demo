#pragma once

#include <memory>
#include <cpprest/http_listener.h>              // HTTP server
#include "CMnConn.h"
#undef U // due to bug in cpprest https://github.com/Microsoft/vcpkg/issues/836

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

namespace HTTPSrv
{
	class CHTTPSrv
	{
	private:
		std::unique_ptr<http_listener> m_listener;
	public:
		CHTTPSrv();
		~CHTTPSrv();

		void InitListerner(MnConn::CMnConn&);
		void StartListerner();
		void StopListerner();
	};
}
