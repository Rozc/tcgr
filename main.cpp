#include <cstdio>
#include "Tools/Logger.h"
#include "Net/TcpServer.h"

Tools::Logger& logger = Tools::Logger::getInstance();

int main() {

    logger.Log(LOG_IMPT, "Welcome, server is starting...");

    Net::TcpServer& server = Net::TcpServer::getInstance();
    server.init(6666);
    server.run();

    return 0;
}
