//
// Created by 12903 on 2022/10/25.
//

#pragma once

#include <unordered_map>
#include <string>

enum StatusCode {
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    INTERNET_SERVER_ERROR = 500,
};


enum RequestType {
    GET,
    POST,
    DELETE,
    PUT,
    PATCH,
    OPTIONS,
    HEAD,
    TRACE,
    CONNECT
};


struct RequestLine {
    RequestType type;
    char *url;
    char *version; // HTTP/1.1 HTTP/1.0
};

struct ResponseLine {
    char *version;
    StatusCode code;
    char *message;
};

union FirstLine {
    RequestLine req;
    ResponseLine resp;
    char *errors{};
};

typedef std::unordered_map<std::string, std::string> Header;