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

int setConfig(std::string IP, int port, std::string work_dir);

std::string &Work_Dir();
