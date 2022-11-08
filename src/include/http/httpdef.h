//
// Created by 12903 on 2022/10/25.
//

#pragma once

#include <string>
#include <unordered_map>

enum StatusCode {
  OK = 200,
  BAD_REQUEST = 400,
  NOT_FOUND = 404,
  INTERNET_SERVER_ERROR = 500,
};

enum RequestMethod {
  GET,
  POST,
  DELETE,
  PUT,
  PATCH,
  OPTIONS,
  HEAD,
  TRACE,
  CONNECT,
  UNDEFINED
};

typedef std::unordered_map<std::string, std::string> Header;