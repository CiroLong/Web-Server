//
// Created by 12903 on 2022/10/25.
//

#include "common/logger.h"
#include "config/config.h"
#include "http/handler.h"
#include "http/http.h"
#include "net/connection.h"
#include "string"

int main() {
  LOG_INFO("WEB_SERVER START!!!");

  setConfig("127.0.0.1", 8080, ".");

  auto *tcp_conn = new TCPConnection(C.IP, C.port);
  tcp_conn->listen();
  TCPHandler *handler;
  while ((handler = tcp_conn->accept()) != nullptr) {
    HTTPHandler *hHandler = new HTTPHandler(handler);
    hHandler->handle();
  }

  LOG_INFO("EXIT!!!");
  return 0;
}
