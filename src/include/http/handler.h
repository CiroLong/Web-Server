//
// Created by ciro on 22-10-25.
//
#pragma once

#include "net/connection.h"
#include "net/handler.h"

class HTTPHandler {
public:
  HTTPHandler(TCPHandler *tcp_handler);
  ~HTTPHandler();

  void handle();

private:
  TCPHandler *tcp_handler_;
  TCPConnection *tcp_conn_;
};
