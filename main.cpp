#include <csignal>
#include "Tools/Logger.h"
#include "Tools/TaskPool.h"

int f(int& a, int& b) {
    sleep(a);
    mlog::logger.Log(LOG_INFO, "In f, a = ", a, ", b = ", b);
    std::cout << "Test Log in F" << a << b << std::endl;
    b--;
    return 77;
}


int main() {
    std::cout << "before welcome" << std::endl;
    mlog::logger.Log(LOG_FOCUS, "Welcome, server is starting...");
//    {
//        int a = 3;
//        mlog::logger.Log(LOG_INFO, "a = ", a);
//    }
//    std::cout << "after welcome" << std::endl;
//
//
//    TaskPool taskPool(2);
//    taskPool.run();
//    int a = 1, b = 2;
//
//    auto c = taskPool.add_task(f, std::ref(a), std::ref(b));
//    auto d = taskPool.add_task(f, std::ref(b), std::ref(b));
//    int e = c.get(), f = d.get();
//    mlog::logger.Log(LOG_INFO, a, b, e, f);

    // taskPool.stop();
    // mlog::logger.stop();

//    MatchQueue& matchQueue = MatchQueue::get_instance();
//
//    Net::TcpServer& server = Net::TcpServer::get_instance();
//    server.init(6666);
//    server.run();



//    Json::Value json;
//    ServerIO::str2json(R"({"name": "aaweo"})", &json);
//    logger.Log(LOG_INFO, json["name"].asString());




    return 0;
}
