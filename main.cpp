#include <csignal>
#include "Tools/Logger.h"
#include "Tools/TaskPool.h"

int f(int& a, int& b) {
    sleep(a);
    Tools::logger.Log(LOG_INFO, "In f, a = ", a, ", b = ", b);
    b--;
    return 77;
}


int main() {
    Tools::logger.Log(LOG_FOCUS, "Welcome, server is starting...");

    TaskPool taskPool(2);
    taskPool.run();
    int a = 5, b = 10;

    auto c = taskPool.add_task(f, std::ref(a), std::ref(b));
    auto d = taskPool.add_task(f, std::ref(b), std::ref(b));
    int e = c.get(), f = d.get();
    Tools::logger.Log(LOG_INFO, a, b, e, f);

//    MatchQueue& matchQueue = MatchQueue::getInstance();
//
//    Net::TcpServer& server = Net::TcpServer::getInstance();
//    server.init(6666);
//    server.run();



//    Json::Value json;
//    ServerIO::str2json(R"({"name": "aaweo"})", &json);
//    logger.Log(LOG_INFO, json["name"].asString());




    return 0;
}
