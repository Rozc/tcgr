#include <cstdio>
#include "Tools/Logger.h"
#include "Net/TcpServer.h"

int main() {
    Tools::Logger& logger = Tools::Logger::getInstance();
    logger.Log(Tools::DEBUG, "Welcome, server is starting...");

    Net::TcpServer& server = Net::TcpServer::getInstance();
    server.init(6666);
    server.run();

    return 0;
}
