//
// Created by 12903 on 2022/11/14.
//

#ifndef WEBSERVER_EPOLL_H
#define WEBSERVER_EPOLL_H

#include <sys/epoll.h>

class epoll_helper {
public:
  static void create_event(int epoll_fd, int fd, int state);
  static void delete_event(int epoll_fd, int fd, int state);
  static void modify_event(int epoll_fd, int fd, int state);
};

#endif // WEBSERVER_EPOLL_H
