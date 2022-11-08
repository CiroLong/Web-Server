//
// Created by ciro on 22-10-25.
//

#pragma once

#include "http/http.h"
#include "http/httpdef.h"

int parse_http(char *buf, FirstLine &first_line_, Header &header_, char *&body_);