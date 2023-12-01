//
// Created by UserName on 2023/12/01.
//
#include <iostream>
#include "../Tools/Logger.h"


int main() {
    mlog::Logger& logger = mlog::Logger::get_instance();
    std::string path = "./log.txt";
    logger.set_output_file(path);
    std::cout << "welcome" << std::endl;
    logger.Log(LOG_FOCUS | LOG_BLOCK | LOG_CONS, "blocked console info");
    logger.Log(LOG_INFO | LOG_ASYNC | LOG_CONS | LOG_FILE, "async hello world");
    std::cout << "after" << std::endl;
    logger.stop();
    return 0;
}