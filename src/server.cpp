#include <arpa/inet.h>
#include <csignal>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common/hashmap.h"
#include "common/logger.h"
#include "config/config.h"
#include "epoll/epoll.h"
#include "epoll/thread_pool.h"
#include "http/handler.h"
#include "http/http.h"
#include "util/util.h"

const unsigned int MAX_EVENTS = 128;
const unsigned int FD_SIZE = 1024;

extern Config GrobalConfig;

using std::bind;

concurrent_hashmap<int, shared_ptr<connection>> connection_storage;

int main() {
  signal(SIGPIPE, SIG_IGN);
  
  setConfig("127.0.0.1", 8080, "../../etc");

  thread_poll poll(8);

  sockaddr_in server_addr{};
  server_addr.sin_port = htons(GrobalConfig.port);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(GrobalConfig.IP.c_str());

  int epoll_fd = epoll_create(FD_SIZE);
  struct epoll_event events[MAX_EVENTS];

  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  epoll_helper::create_event(epoll_fd, listen_fd, EPOLLIN);

  bind(listen_fd, reinterpret_cast<sockaddr *>(&server_addr),
       sizeof(server_addr));
  listen(listen_fd, 5);
  helper::set_non_block(listen_fd);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
  while (true) {
    const int active = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

    for (int i = 0; i < active; i++) {
      const int fd = events[i].data.fd, ev = events[i].events;
      if (fd == listen_fd && (ev & (EPOLLIN | EPOLLERR))) {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);

        int conn_fd =
            accept(listen_fd, reinterpret_cast<sockaddr *>(&client_addr),
                   &client_addr_len);
        if (conn_fd != -1) {
          LOG_INFO("[%d] Connection Established", conn_fd);

          helper::set_non_block(conn_fd);
          handler::print_client_info(reinterpret_cast<sockaddr *>(&client_addr),
                                     client_addr_len);
          epoll_helper::create_event(epoll_fd, conn_fd,
                                     EPOLLIN | EPOLLOUT | EPOLLET);

          if (connection_storage.count(conn_fd)) {
            connection_storage.erase(conn_fd);
          }
          connection_storage.set(conn_fd,
                                 shared_ptr<connection>(new connection()));
        }
      } else if (ev & EPOLLIN) {
        int conn_fd = events[i].data.fd;
        LOG_INFO("[%d] trigger epoll_in event", conn_fd);
        if (!connection_storage.count(conn_fd)) {
          LOG_INFO("[%d] conn_fd invalid", conn_fd);
          continue;
        }

        poll.push(bind(handler::read, epoll_fd, conn_fd,
                       connection_storage.get(conn_fd)));
      } else if (ev & EPOLLOUT) {
        int conn_fd = events[i].data.fd;
        LOG_INFO("[%d] trigger epoll_out event", conn_fd);
        if (!connection_storage.count(conn_fd)) {
          LOG_INFO("[%d] conn_fd invalid", conn_fd);
          continue;
        }

        poll.push(bind(handler::write, epoll_fd, conn_fd,
                       connection_storage.get(conn_fd)));
      } else if (ev & EPOLLERR) {
        int conn_fd = events[i].data.fd;
        LOG_INFO("[%d] trigger epoll_err event ", conn_fd);
        connection_storage.erase(conn_fd);
        LOG_INFO("[%d] disposed ", conn_fd);
      }
    }
  }
#pragma clang diagnostic pop
  return 0;
}