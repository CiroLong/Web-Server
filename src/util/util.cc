//
// Created by 12903 on 2022/10/25.
//

#include "util/util.h"
#include "common/logger.h"

int helper::read_one_char_from_connection(char &buff,
                                          shared_ptr<connection> conn) {
  if (conn->read_ptr == conn->read.size()) {
    return 0;
  }
  buff = conn->read[conn->read_ptr];
  conn->read_ptr++;
  return 1;
}

string helper::readline_from_connection(shared_ptr<connection> conn) {
  string result;
  char last_buf = '\0', buf = '\0';

  while (helper::read_one_char_from_connection(buf, conn) > 0) {
    if (last_buf == '\r' && buf == '\n') {
      result.pop_back();
      break;
    }

    if (buf == '\0') {
      break;
    }

    result += buf;
    last_buf = buf;
  }

  return result;
}

void helper::read_http_first_line(shared_ptr<connection> conn, int conn_fd) {
  stringstream ss;

  ss << readline_from_connection(conn);
  ss >> conn->req->method >> conn->req->path >> conn->req->version;

  LOG_INFO("[%d] %s %s %s", conn_fd, conn->req->method.c_str(),
           conn->req->path.c_str(), conn->req->version.c_str());
}

void helper::parse_header(shared_ptr<connection> conn, int conn_fd) {
  conn->req = unique_ptr<request>(new request());
  helper::read_http_first_line(conn, conn_fd);

  string buffer;
  while (!(buffer = helper::readline_from_connection(conn)).empty()) {
    const size_t line_length = buffer.size();
    string key, value;
    size_t ptr = 0;
    while (ptr < line_length) {
      if (buffer[ptr] == ':') {
        break;
      }
      ptr++;
    }

    key = buffer.substr(0, ptr);
    value = buffer.substr(ptr + 2);

    conn->req->header[key] = value;
  }

  if (conn->req->header.count("Content-length")) {
    conn->body_size = stol(conn->req->header["Content-length"]);
  } else if (conn->req->header.count("content-length")) {
    conn->body_size = stol(conn->req->header["content-length"]);
  } else if (conn->req->header.count("Content-Length")) {
    conn->body_size = stol(conn->req->header["Content-Length"]);
  } else {
    conn->body_size = 0;
  }

  LOG_INFO("[%d] header parse invoked, body_size = %lu", conn_fd,
           conn->body_size);
}

void helper::parse_body(shared_ptr<connection> conn, int conn_fd) {
  conn->req->body = conn->read.substr(conn->read_ptr);
  LOG_INFO("[%d] body parse invoked", conn_fd);
}

optional<string> helper::read_file_extension(const string &url) {
  int index = url.find('.');
  if (index == std::string::npos) {
    return std::nullopt;
  }

  string result;
  const size_t url_size = url.size();
  for (int i = 0; i < url_size; i++) {
    if (url[url_size - i - 1] == '.')
      break;
    result.insert(result.begin(), url[url_size - i - 1]);
  }
  return result;
}

string helper::get_file_type(const string &file_extensions) {
  const unordered_set<string> image_type_collection = {"jpg", "jpeg", "png",
                                                       "gif", "bmp"};

  if (image_type_collection.count(file_extensions)) {
    return "image/" + file_extensions;
  } else if (file_extensions == "html") {
    return "text/html";
  } else {
    return "text/plain";
  }
}

bool helper::file_exist(const string &file_name, struct stat &file_stat) {
  return stat(file_name.c_str(), &file_stat) >= 0;
}

void helper::set_non_block(const int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) {
    LOG_ERROR("[%d] set non-block io failed", fd);
    return;
  }
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}