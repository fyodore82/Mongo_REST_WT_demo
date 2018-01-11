#include "stdafx.h"

const web::uri ServerURI(U("http://localhost"));
const utf16string PathURI = L"/stolenvehicles";
const std::chrono::milliseconds ReqTimeout(5000);	// HTTP request timeout in ms