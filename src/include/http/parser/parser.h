//
// Created by ciro on 22-10-25.
//

#pragma once

#ifndef HTTP_MAX_HEADER_SIZE
#define HTTP_MAX_HEADER_SIZE (80 * 1024)
#endif

#include "http/http.h"
#include "http/httpdef.h"

class RequestPacket;

int parse_http(char *buf, RequestPacket *rp);