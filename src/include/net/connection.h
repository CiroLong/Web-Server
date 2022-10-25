//
// Created by 12903 on 2022/10/25.
//

#ifndef WEB_SERVER_CONNECTION_H
#define WEB_SERVER_CONNECTION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// linux only now
class Connection {
public:
    Connection();

    ~Connection();

    virtual bool open() = 0;

    virtual void listen() = 0;

    virtual int accept() = 0;

};

class TCPConnection : public Connection {
public:
    TCPConnection();

    ~TCPConnection();

private:

};


#endif //WEB_SERVER_CONNECTION_H
