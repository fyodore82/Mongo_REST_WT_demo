#pragma once

// Defines constants used in program

#include <cpprest/http_listener.h>              // HTTP server
#undef U // due to bug in cpprest https://github.com/Microsoft/vcpkg/issues/836

using namespace utility;

extern const utility::char_t* HTTPServerURI;
extern const std::string MDBConnStr;
extern const std::string DB;
extern const std::string VehColl;
extern const std::string LogColl;