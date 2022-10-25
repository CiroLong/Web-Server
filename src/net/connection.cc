//
// Created by 12903 on 2022/10/25.
//

#include "net/connection.h"

TCPConnection::~TCPConnection() {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
}

TCPConnection::TCPConnection() {

}
