//
// Created by 12903 on 2022/11/14.
//
#include "http/connection.h"

connection::connection() {
  this->read_ptr = 0;
  this->write_ptr = 0;
  this->req = nullptr;
  this->header_size = 0;
  this->body_size = 0;
}