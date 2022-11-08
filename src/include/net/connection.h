//
// Created by 12903 on 2022/10/25.
//

#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "net/handler.h"

class Connection;

class TCPHandler;

// linux only now
class Connection {
public:
  Connection() = default;

  virtual ~Connection() {}
};

class TCPConnection : public Connection {
public:
  // open a TCPConnection
  TCPConnection(std::string IP, int port);

  ~TCPConnection() override;

  void listen();

  TCPHandler *accept();

  int server_sock() { return server_sock_; }

  std::string ip() { return ip_; }

  int port() { return port_; }

private:
  int server_sock_;
  std::string ip_;
  int port_;
};