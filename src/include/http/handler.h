#pragma once

#include "common/hashmap.h"
#include "epoll/epoll.h"
#include "http/connection.h"
#include "http/http.h"
#include "util/util.h"
#include <cstring>
#include <fstream>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>

using std::shared_ptr;

extern concurrent_hashmap<int, shared_ptr<connection>> connection_storage;

class handler {
public:
  static void print_client_info(sockaddr *client_addr,
                                socklen_t client_addr_len);

  static void read(int epoll_fd, int conn_fd, shared_ptr<connection> conn);
  static void write(int epoll_fd, int conn_fd, shared_ptr<connection> conn);

private:
  static void parse_all(shared_ptr<connection> conn, bool init, int conn_fd);

  static bool _read(int epoll_fd, int conn_fd, shared_ptr<connection> conn);
  static bool _write(int epoll_fd, int conn_fd, shared_ptr<connection> conn);
};