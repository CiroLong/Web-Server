//
// Created by 12903 on 2022/11/8.
//
#pragma once
#include "string"

struct Config {
  std::string IP;
  int port;
  std::string work_dir;
};

Config C = {"127.0.0.1", 8080, "."};

int setConfig(std::string IP, int port, std::string work_dir);
