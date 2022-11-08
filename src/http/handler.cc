//
// Created by ciro on 22-10-25.
//

#include "http/handler.h"
#include "common/common.h"
#include "common/logger.h"
#include "config/config.h"
#include "http/http.h"
#include <fstream>

HTTPHandler::HTTPHandler(TCPHandler *tcp_handler)
    : tcp_handler_(tcp_handler), tcp_conn_(tcp_handler->tcp_conn()) {}

HTTPHandler::~HTTPHandler() { delete tcp_handler_; }

char *FindFile(std::string filename) {

  auto target = Work_Dir() + filename;

  std::fstream in(target,
                  std::fstream::in); // std::fstream::in表示以只读方式打开
  bool open = in.is_open();
  if (!open) {
    LOG_ERROR("Can't Find file");
    return nullptr;
  }
  char *data = new char[FILE_MAX_SIZE];

  in.read(data, FILE_MAX_SIZE);

  in.close(); // 在流使用完毕后关闭流

  return data;
}

void HTTPHandler::handle() {
  // src data
  char *const buf = new char[BUF_MAX_SIZE];

  tcp_handler_->read(buf);
  RequestPacket *rp = RequestPacket::parse_request(buf);

  if (rp->method_ != RequestMethod::GET) {
    // bad request
    LOG_ERROR("404");
    return;
  }

  ResponsePacket *responsePacket = new ResponsePacket();
  responsePacket->code_ = StatusCode::OK;
  responsePacket->explain_ = "OK";
  responsePacket->version_ = "HTTP/1.1";
  char header_buf[BUF_MAX_SIZE];
  // Add Header
  //
  int len_header_buf = parse_into_http_header(header_buf, responsePacket);

  // find body

  char *fliedata = FindFile(rp->url_);
  if (fliedata == nullptr) {
    LOG_ERROR("404");
    return;
  }
  std::string body(fliedata);
  LOG_INFO("body:%s", body.c_str());

  // responsePacket->set_body(body);

  tcp_handler_->write(header_buf, len_header_buf);
  tcp_handler_->write(body);

  LOG_INFO("handle() done");
  delete[] fliedata;
}
