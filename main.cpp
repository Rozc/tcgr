#include <iostream>
#include "Tools/Logger.h"

int main() {
    Tools::Logger& logger = Tools::Logger::getInstance();

    logger.Log(Tools::DEBUG, "Server Starting...", 123, "Test", 3.14);

    return 0;
}
