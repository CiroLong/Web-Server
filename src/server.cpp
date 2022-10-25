//
// Created by 12903 on 2022/10/25.
//

#include "stdio.h"
#include "iostream"
#include "common/logger.h"

int main() {
    std::cout << "Hello, World" << std::endl;
    LOG_INFO("info test");
    LOG_TRACE("trace test");
    return 0;
}