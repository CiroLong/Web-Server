//
// Created by ciro on 22-10-25.
//

#pragma once

#include "http/httpdef.h"
#include "http/parser/parser.h"
#include "map"

class HTTPPacket;
class RequestPacket;
class ResponsePacket;

class HTTPPacket {
public:
  HTTPPacket() = default;

  ~HTTPPacket() { delete data_; };

  Header &header() { return header_; }
  const char *body() { return body_; }

protected:
  char *data_; // Packet hold the src data of http data
  Header header_{};
  char *body_;
};

class RequestPacket : public HTTPPacket {
public:
  RequestPacket();
  ~RequestPacket() = default;

  static RequestPacket *parse_request(char *buf) {
    RequestPacket *rp = new RequestPacket();
    parse_http(buf, rp);
    return rp;
  }

private:
  RequestMethod method_;
  std::string url_;
  std::string version_;
  // Header
  // body
};

class ResponsePacket : public HTTPPacket {
public:
private:
  StatusCode code_;
  std::string explain_;
  std::string version_;
  // Header
  // body
};