#include "stdafx.h"

// Defines constants used in program

#include <cpprest/http_listener.h>              // HTTP server

using namespace utility;

const utility::char_t* HTTPServerURI = L"http://localhost/stolenvehicles";
const std::string MDBConnStr = "mongodb://CrsConn:Jjjj7080@cluster0-shard-00-00-dmaqe.mongodb.net:27017,cluster0-shard-00-01-dmaqe.mongodb.net:27017,cluster0-shard-00-02-dmaqe.mongodb.net:27017/test?ssl=true&replicaSet=Cluster0-shard-0&authSource=admin";
const std::string DB = "StolenVehDB";
const std::string VehColl = "Vehicle";
const std::string LogColl = "Log";