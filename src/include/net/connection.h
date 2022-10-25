//
// Created by 12903 on 2022/10/25.
//

#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

#include "net/handler.h"

class Connection;

class TCPHandler;


// linux only now
class Connection {
public:
    Connection();

    ~Connection();
};

class TCPConnection : public Connection {
public:
    // open a TCPConnection
    TCPConnection(std::string IP, int port);

    ~TCPConnection();

    void listen();

    TCPHandler *accept();

    int server_sock() { return server_sock_; }

    std::string ip() { return ip_; }

    int port() { return port(); }

private:
    int server_sock_;
    std::string ip_;
    int port_;
};