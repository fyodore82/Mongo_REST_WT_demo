// VehicleClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CWtClient.h"
#include "Macros.h"
//#include "Constants.h"

int main(int argc, char **argv)
{
	
	try
	{
		return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {

			return Wt::cpp14::make_unique<WtClient::CWtClient>(env);
		});
	}
	catch (std::exception& e)	// Not going to get exceptions here. Mostly for debugging
	{
		TRACE("Exception!");
		TRACE(e.what());
		std::wcout << "Press ENTER to exit." << std::endl;

		std::string line;
		std::getline(std::cin, line);
	}
}

