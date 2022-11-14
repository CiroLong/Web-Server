//
// Created by ciro on 22-10-25.
//

#include "http/handler.h"
#include "common/common.h"
#include "common/logger.h"
#include "config/config.h"
#include "http/http.h"
#include "util/util.h"

using std::cin, std::cout, std::cerr, std::endl;
const unsigned int MAX_BUF = 8192;

extern concurrent_hashmap<int, shared_ptr<connection>> connection_storage;

void handler::write(int epoll_fd, int conn_fd, shared_ptr<connection> conn) {
  bool exec_result = handler::_write(epoll_fd, conn_fd, conn);

  if (exec_result) {
    ::close(conn_fd);
    epoll_helper::delete_event(epoll_fd, conn_fd, EPOLLIN | EPOLLOUT | EPOLLET);
    connection_storage.erase(conn_fd);
    LOG_INFO("[%d] disposed", conn_fd);
  }
}

void handler::read(int epoll_fd, int conn_fd, shared_ptr<connection> conn) {
  bool exec_result = handler::_read(epoll_fd, conn_fd, conn);

  if (exec_result) {
    ::close(conn_fd);
    epoll_helper::delete_event(epoll_fd, conn_fd, EPOLLIN | EPOLLOUT | EPOLLET);
    connection_storage.erase(conn_fd);
    LOG_INFO("[%d] disposed", conn_fd);
  }
}

bool handler::_write(int epoll_fd, int conn_fd, shared_ptr<connection> conn) {
  if (conn->req == nullptr && conn->read.empty()) {
    return false;
  }
  if (conn->req == nullptr) {
    handler::parse_all(conn, true, conn_fd);
  }

  int n = 0, remain = conn->write.size() - conn->write_ptr;
  while ((n = ::write(conn_fd, conn->write.c_str() + conn->write_ptr, remain)) >
         0) {
    LOG_INFO("[%d] written %d", conn_fd, n);

    conn->write_ptr += n;
    remain -= n;
  }

  if (conn->write.size() == conn->write_ptr) {
    LOG_INFO("[%d] file sent completed", conn_fd);
    return true;
  }

  if (n < 0) {
    LOG_INFO("[%d] remain %d resources, %d -> %s signal received", conn_fd,
             remain, errno, ::strerror(errno));
  }

  if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return false;
  }

  return true;
}

bool handler::_read(int epoll_fd, int conn_fd, shared_ptr<connection> conn) {
  char buf[8196];
  int n = 0;

  string &read_storage = conn->read;
  while ((n = ::read(conn_fd, buf, sizeof(buf))) > 0) {
    LOG_INFO("[%d] received %d", conn_fd, n);
    read_storage.append(buf, n);

    if (conn->req == nullptr) {
      const int pos = read_storage.find("\r\n\r\n");
      if (pos != std::string::npos) {
        conn->header_size = pos + 4;

        helper::parse_header(conn, conn_fd);
      }
    }

    if (conn->req != nullptr &&
        conn->header_size + conn->body_size == read_storage.size()) {
      handler::parse_all(conn, false, conn_fd);

      return handler::_write(epoll_fd, conn_fd, conn);
    }
  }

  if (n < 0) {
    LOG_INFO("[%d] %d -> %s signal received", conn_fd, errno,
             ::strerror(errno));
  }

  if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return false;
  }

  return true;
}

void handler::parse_all(shared_ptr<connection> conn, bool init, int conn_fd) {
  if (init) {
    conn->req = unique_ptr<request>(new request());
    helper::parse_header(conn, conn_fd);
  }

  helper::parse_body(conn, conn_fd);
  // free memory
  conn->read.clear();
  conn->read_ptr = 0;
  LOG_INFO("[%d] Request parsed", conn_fd);

  const auto &http_request = conn->req;
  response http_response{};
  if (http_request->method != "GET") {
    http_response.code = "405";
    http_response.message = "Method not allowed";

    conn->write = http_response.build();
    return;
  }

  const string file_path = Work_Dir() + http_request->path;
  optional<string> file_extension =
      helper::read_file_extension(http_request->path);
  struct stat file_stat {};
  FILE *fp = fopen(file_path.c_str(), "r");

  if (file_extension == std::nullopt ||
      !helper::file_exist(file_path, file_stat) || fp == nullptr) {
    http_response.code = "404";
    http_response.message = "Not found";

    conn->write = http_response.build();
    return;
  }
  http_response.code = "200";
  http_response.message = "OK";
  http_response.header["Content-type"] =
      helper::get_file_type(file_extension.value());
  http_response.header["Content-length"] = std::to_string(file_stat.st_size);

  conn->write = http_response.build();
  int file_block_size = 0;
  char buffer[8192];
  while ((file_block_size = fread(buffer, sizeof(char), 8192, fp)) > 0) {
    conn->write.append(buffer, file_block_size);
  }
  fclose(fp);

  LOG_INFO("File read completed");
}

void handler::print_client_info(sockaddr *client_addr,
                                socklen_t client_addr_len) {
  char host_name[MAX_BUF], host_port[MAX_BUF];

  memset(host_name, 0, sizeof(host_name));
  memset(host_port, 0, sizeof(host_port));

  getnameinfo(client_addr, client_addr_len, host_name, sizeof(host_name),
              host_port, sizeof(host_port), 0);

  LOG_INFO("Received Request from: %s, port: %s", host_name, host_port);
}