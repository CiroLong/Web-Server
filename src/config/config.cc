//
// Created by 12903 on 2022/11/8.
//

#include "config/config.h"

Config GrobalConfig = {"127.0.0.1", 8080, "."};

int setConfig(std::string IP, int port, std::string work_dir) {
  GrobalConfig.IP = IP;
  GrobalConfig.port = port;
  GrobalConfig.work_dir = work_dir;
  return 0;
}

std::string &Work_Dir() { return GrobalConfig.work_dir; }