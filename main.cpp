#include <iostream>
#include "Tools/Logger.h"

int main() {
    Tools::Logger& logger = Tools::Logger::getInstance();

    logger.Log(Tools::INFO, "Server Starting...");

    return 0;
}
