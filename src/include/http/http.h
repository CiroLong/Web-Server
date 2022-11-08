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
  explicit HTTPPacket(char *data) : data_(data) {}

  ~HTTPPacket() { delete[] data_; };

  Header &header() { return header_; }
  const char *body() { return body_; }

  void set_body(char *body) { body_ = body; }

protected:
  char *data_; // Packet hold the src data of http data
  Header header_{};
  char *body_;
};

class RequestPacket : public HTTPPacket {
public:
  RequestPacket(char *data);
  ~RequestPacket() = default;

  static RequestPacket *parse_request(char *buf) {
    auto *rp = new RequestPacket(buf);
    parse_http(buf, rp);
    return rp;
  }

  friend int parse_http(char *buffer, RequestPacket *rp);
  friend class HTTPHandler;

private:
  RequestMethod method_;
  std::string url_;
  std::string version_;
  // Header
  // body
};

class ResponsePacket : public HTTPPacket {
public:
  friend class HTTPHandler;
  friend int parse_into_http_header(char *buffer, ResponsePacket *rp);

private:
  std::string version_;
  StatusCode code_;
  std::string explain_;
  // Header
  // body
};