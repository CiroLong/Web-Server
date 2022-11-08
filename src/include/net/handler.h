//
// Created by ciro on 22-10-25.
//

#pragma once

#include "common/common.h"
#include "connection.h"
#include "net/connection.h"

class TCPConnection;

class TCPHandler {
public:
  TCPHandler();

  ~TCPHandler();

  size_t write(std::string &s);

  size_t write(const char *str, size_t len);

  size_t read(char *buf, size_t n = BUF_MAX_SIZE);

public:
  TCPConnection *tcp_conn() { return tcp_conn_; }

  int client_sock() { return client_sock_; }

  const std::string &client_ip() { return client_ip_; }

  int client_port() { return client_port_; }

private:
  friend class TCPConnection;

  TCPConnection *tcp_conn_;
  int client_sock_;
  std::string client_ip_;
  int client_port_;
};
