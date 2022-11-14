#ifndef WEBSERVER_CONNECTION_H
#define WEBSERVER_CONNECTION_H

#include <cstring>
#include <iostream>
#include <memory>

#include "http.h"

using std::string, std::unique_ptr;

class connection {
public:
  string read;
  string write;

  size_t header_size;
  size_t body_size;

  size_t read_ptr;
  size_t write_ptr;

  unique_ptr<request> req;

  connection();
  // void dispose();
};

#endif // WEBSERVER_CONNECTION_H
