//
// Created by ciro on 22-10-25.
//

#pragma once

#include "net/handler.h"
#include "common/logger.h"


TCPHandler::TCPHandler() {
}

TCPHandler::~TCPHandler() {
    if (-1 == close(client_sock_)) {
        LOG_ERROR("close client_sock error: %s", strerror(errno));
    }
}

size_t TCPHandler::write(std::string &s) {
    return write(s.c_str(), s.size());
}

size_t TCPHandler::write(const char *str, size_t len) {
    return ::write(client_sock_, str, len);
}

size_t TCPHandler::read(char *buf, size_t n) {
    return ::read(client_sock_, buf, n);
}

