//
// Created by ciro on 22-10-25.
//
#pragma once

#include "net/connection.h"
#include "net/handler.h"

class HTTPConnection {
public:

private:
    TCPConnection *tcp_conn_;
    TCPHandler *tcp_handler_;
};

