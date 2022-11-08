//
// Created by ciro on 22-10-25.
//

#pragma once

#include "map"
#include "http/httpdef.h"
#include "http/parser/parser.h"

enum PacketType {
    Request,
    Response
};


class HTTPPacket {
public:
    HTTPPacket();

    ~HTTPPacket();

    static HTTPPacket *parse_request(char *buf) {
        HTTPPacket *hp = new HTTPPacket;
        parse_http(buf, hp->first_line_, hp->header_, hp->body_);
        hp->data_ = buf;
        hp->type_ = Request;
        return hp;
    }

protected:
    char *data_; // Packet hold the src data of http data

    PacketType type_;
    FirstLine first_line_;
    Header header_{};
    char *body_;
};