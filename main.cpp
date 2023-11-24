#include "Tools/Logger.h"
#include "Net/TcpServer.h"
#include "Net/ServerIO.h"
#include "include/json/json.h"
#include "Game/MatchQueue.h"


Tools::Logger& logger = Tools::Logger::getInstance();

int main() {
    logger.Log(LOG_FOCUS, "Welcome, server is starting...");

//    Net::TcpServer& server = Net::TcpServer::getInstance();
//    server.init(6666);
//    server.run();



//    Json::Value json;
//    ServerIO::str2json(R"({"name": "aaweo"})", &json);
//    logger.Log(LOG_INFO, json["name"].asString());

    MatchQueue& matchQueue = MatchQueue::getInstance();
    matchQueue.enqueue(1, 1488);
    matchQueue.enqueue(2, 1401);
    matchQueue.enqueue(3, 1000);
    matchQueue.enqueue(4, 3000);

    sleep(3);

    matchQueue.enqueue(5, 1000);
    matchQueue.enqueue(6, 3000);

    getchar();


    return 0;
}
