// VehicleServer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "HTTPSrv.h"
#include "CMnConn.h"

#include <memory>
#include <exception> 

int main()
{
	std::unique_ptr<HTTPSrv::CHTTPSrv> httpsrv (new HTTPSrv::CHTTPSrv);
	std::unique_ptr<MnConn::CMnConn> mnconn(new MnConn::CMnConn);

	try
	{
		mnconn->Connect();
		httpsrv->InitListerner(*mnconn);
		httpsrv->StartListerner();

		// Just wait for termination by Enter key
		std::wcout << "\nPress ENTER to exit." << std::endl;
		std::string line;
		std::getline(std::cin, line);

		httpsrv->StopListerner();
	}
	catch (std::exception& e)		// mongocxx::exception is inherited from std::exception, so we'll handle only it
									// used mostly for debugging. We don't expect exceptions during execution
	{
		TRACE("Exception!");
		TRACE(e.what());
		std::wcout << "Press ENTER to exit." << std::endl;
		std::string line;
		std::getline(std::cin, line);
	}


    return 0;
}

