//
// Created by 12903 on 2022/11/8.
//

#include "config/config.h"

int setConfig(std::string IP, int port, std::string work_dir) {
  C.IP = IP;
  C.port = port;
  C.work_dir = work_dir;
  return 0;
}