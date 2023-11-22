#include <cstdio>
#include "Tools/Logger.h"
#include "Net/TcpServer.h"
#include "Net/ServerIO.h"
#include "include/json/json.h"

Tools::Logger& logger = Tools::Logger::getInstance();

int main() {

    logger.Log(LOG_FOCUS, "Welcome, server is starting...");

    Json::Value json;
    ServerIO::str2json(R"({"name": "aaweo"})", &json);
    logger.Log(LOG_INFO, json["name"].asString());

//    Net::TcpServer& server = Net::TcpServer::getInstance();
//    server.init(6666);
//    server.run();

    return 0;
}
