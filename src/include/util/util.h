//
// Created by 12903 on 2022/11/9.
//

#ifndef WEBSERVER_UTIL_H
#define WEBSERVER_UTIL_H

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <unordered_set>

#include "http/connection.h"
#include "http/http.h"

using std::string, std::stringstream, std::optional, std::unordered_set,
    std::memset, std::shared_ptr;

class helper {
public:
  static string readline_from_connection(shared_ptr<connection> conn);
  static void parse_header(shared_ptr<connection> conn, int conn_fd);
  static void parse_body(shared_ptr<connection> conn, int conn_fd);
  static optional<string> read_file_extension(const string &url);
  static string get_file_type(const string &file_extensions);
  static bool file_exist(const string &file_name, struct stat &file_stat);
  static void set_non_block(const int fd);

private:
  static void read_http_first_line(shared_ptr<connection> conn, int conn_fd);
  static int read_one_char_from_connection(char &buff,
                                           shared_ptr<connection> conn);
};

#endif // WEBSERVER_UTIL_H
