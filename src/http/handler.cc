//
// Created by ciro on 22-10-25.
//

#include "http/handler.h"
#include "common/common.h"
#include "http/http.h"

HTTPHandler::HTTPHandler(TCPHandler *tcp_handler)
    : tcp_handler_(tcp_handler), tcp_conn_(tcp_handler->tcp_conn()) {}

HTTPHandler::~HTTPHandler() { delete tcp_handler_; }

void HTTPHandler::handle() {
  char buf[BUF_MAX_SIZE];
  tcp_handler_->read(buf);
  RequestPacket *rp = RequestPacket::parse_request(buf);

  ResponsePacket responsePacket;
  responsePacket.code_ = StatusCode::OK;
  responsePacket.explain_ = "OK";
  responsePacket.version_ = "HTTP/1.1";
  char resp_buf[RESPONSE_BUF_MAX_SIZE];
}
