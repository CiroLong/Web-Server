//
// Created by ciro on 22-10-25.
//

#include "http/parser/parser.h"
#include "common/logger.h"
#include <cstring>

RequestMethod strtoMethod(std::string &s) {
  if (s == "GET") {
    return RequestMethod::GET;
  } else if (s == "POST") {
    return RequestMethod::POST;
  } else if (s == "DELETE") {
    return RequestMethod::DELETE;
  } else if (s == "PUT") {
    return RequestMethod::PUT;
  } else if (s == "PATCH") {
    return RequestMethod::PATCH;
  } else if (s == "OPTIONS") {
    return RequestMethod::OPTIONS;
  } else if (s == "HEAD") {
    return RequestMethod::HEAD;
  } else if (s == "TRACE") {
    return RequestMethod::TRACE;
  } else if (s == "CONNECT") {
    return RequestMethod::CONNECT;
  } else {
    return RequestMethod::UNDEFINED;
  }
}

int parse_http(char *buffer, RequestPacket *rp) {
  // "GET / HTTP1.1 \r\n"
  int offset = 0;

  int len = 0;
  while (buffer[offset] != ' ' && buffer[offset] != '\n' &&
         buffer[offset] != EOF) {
    offset++;
    len++;
  }
  offset++; // 滤过空格
  std::string method(buffer, len);
  LOG_INFO("method:%s\n", method.c_str());
  rp->method_ = strtoMethod(method);

  if (rp->method_ == RequestMethod::UNDEFINED) {
    LOG_INFO("Bad Request!\n");
    return -1;
  }
  int i = offset;
  len = 0;
  while (buffer[offset] != ' ' && buffer[offset] != '\n' &&
         buffer[offset] != EOF) {
    offset++;
    len++;
  }
  offset++; // 滤过空格,之后本来得拿 HTTP 版本
  rp->url_ = std::string(buffer + i, len);
  LOG_INFO("url = %s\n", rp->url_.c_str());
  // TODO:URL check

  i = offset;
  len = 0;
  while (buffer[offset] != ' ' && buffer[offset] != '\r' &&
         buffer[offset] != '\n' && buffer[offset] != EOF) {
    offset++;
    len++;
  }
  offset += 2; // \r\n
  rp->version_ = std::string(buffer + i, len);
  LOG_INFO("version = %s\n", rp->version_.c_str());

  // Header

  while (buffer[offset] != '\r' && buffer[offset] != '\n' &&
         buffer[offset] != EOF) {
    len = 0;
    i = offset;
    while (buffer[offset] != '\r' && buffer[offset] != '\n' &&
           buffer[offset] != EOF) {
      offset++;
      len++;
    }
    std::string line(buffer + i, len);
    offset += 2;
    int blank = line.find(" ");
    rp->header_.insert(
        std::make_pair(line.substr(0, blank), line.substr(blank + 1)));
    LOG_INFO("line: %s", line.c_str());
  }
  offset += 2;
  rp->body_ = buffer + offset;
  return 0;
}

int parse_into_http_header(char *buffer, ResponsePacket *rp) {
  // HTTP/1.1 200 OK\r\n
  int offset = 0;
  int len = 0;
  len = sprintf(buffer, "%s %d %s\r\n", rp->version_.c_str(), rp->code_,
                rp->explain_.c_str());
  offset += len;

  for (auto &header : rp->header_) {
    len = sprintf(buffer + offset, "%s: %s\r\n", header.first.c_str(),
                  header.second.c_str());
    offset += len;
  }
  sprintf(buffer + offset, "\r\n");
  offset += 2;
  return offset;
}