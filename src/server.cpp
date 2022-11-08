//
// Created by 12903 on 2022/10/25.
//

#include "common/logger.h"
#include "http/handler.h"
#include "http/http.h"
#include "iostream"
#include "net/connection.h"
#include "stdio.h"
#include "string"

int main() {
  std::cout << "Hello, World" << std::endl;
  LOG_INFO("info test");
  LOG_TRACE("trace test");

  auto *tcp_conn = new TCPConnection(std::string("127.0.0.1"), 8080);
  tcp_conn->listen();

  TCPHandler *handler;
  while ((handler = tcp_conn->accept()) != nullptr) {
    HTTPHandler *hHandler = new HTTPHandler(handler);
    hHandler->handle();
  }
  return 0;
}
