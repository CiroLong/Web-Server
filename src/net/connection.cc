//
// Created by 12903 on 2022/10/25.
//

#include "net/connection.h"
#include "common/logger.h"
#include "net/handler.h"

#define MAX_REQUESTS_HOLD 20

char *const defaultIP = "127.0.0.1";


TCPConnection::TCPConnection(std::string IP, int port = 8080) : ip_(IP), port_(port) {
    if (ip_.empty()) {
        ip_ = defaultIP;
    }

    // TCP only
    server_sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in serv_addr{};


    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(ip_.c_str());  //具体的IP地址

    // HOST TO NETWORK
    serv_addr.sin_port = htons(port_);  //端口

    bind(server_sock_, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
}

TCPConnection::~TCPConnection() {
    close(server_sock_);
}

void TCPConnection::listen() {
    ::listen(server_sock_, MAX_REQUESTS_HOLD);
}

TCPHandler *TCPConnection::accept() {
//接收客户端请求

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = ::accept(server_sock_, (struct sockaddr *) &clnt_addr, &clnt_addr_size);

    if (clnt_sock == -1) {
        LOG_ERROR("accept error!");
        return nullptr;
    }

    TCPHandler *handler = new TCPHandler();

    handler->client_ip_ = inet_ntoa(clnt_addr.sin_addr);
    handler->client_port_ = ntohs(clnt_addr.sin_port);
    handler->tcp_conn_ = this;
    handler->client_sock_ = clnt_sock;

    return 0;
}

