//
// Created by UserName on 2023/12/01.
//
#include <iostream>
#include "../Tools/Logger.h"


int main() {
    mlog::Logger& logger = mlog::Logger::getInstance();
    std::cout << "welcome" << std::endl;
    // logger.Log(LOG_INFO, "hello world");
    logger.stop();
    return 0;
}