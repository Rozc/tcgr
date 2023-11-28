#include "Tools/Logger.h"
#include "Net/TcpServer.h"
#include "Net/ServerIO.h"
#include "include/json/json.h"
#include "Game/MatchQueue.h"


int main() {
    Tools::logger.Log(LOG_FOCUS, "Welcome, server is starting...");

    MatchQueue& matchQueue = MatchQueue::getInstance();

    Net::TcpServer& server = Net::TcpServer::getInstance();
    server.init(6666);
    server.run();



//    Json::Value json;
//    ServerIO::str2json(R"({"name": "aaweo"})", &json);
//    logger.Log(LOG_INFO, json["name"].asString());




    return 0;
}
