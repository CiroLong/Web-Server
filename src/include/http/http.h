#pragma once

#include <string>
#include <unordered_map>

using std::string, std::unordered_map, std::pair;

class request {
public:
  string method, path, version;
  unordered_map<string, string> header;
  string body;
};

class response {
public:
  string version, code, message;
  unordered_map<string, string> header;
  string build();
  response();
};