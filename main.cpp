#include "Tools/Logger.h"
//#include "Net/TcpServer.h"
//#include "Net/ServerIO.h"
//#include "include/json/json.h"
//#include "Game/MatchQueue.h"
//
//
Tools::Logger& logger = Tools::Logger::getInstance();
//
//int main() {
//    logger.Log(LOG_FOCUS, "Welcome, server is starting...");
//
//    MatchQueue& matchQueue = MatchQueue::getInstance();
//
//    Net::TcpServer& server = Net::TcpServer::getInstance();
//    server.init(6666);
//    server.run();
//
//
//
////    Json::Value json;
////    ServerIO::str2json(R"({"name": "aaweo"})", &json);
////    logger.Log(LOG_INFO, json["name"].asString());
//
//
//
//
//    return 0;
//}
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>

#define MAX_EVENTS 1024
#define MAX_BUFF_SIZE 1024

int main(int argc, char** argv) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "create socket failed" << std::endl;
        return -1;
    }

    int port = 8888;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cout << "bind socket failed" << std::endl;
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, 10) == -1) {
        std::cout << "listen socket failed" << std::endl;
        close(sockfd);
        return -1;
    }

    int epfd = epoll_create(MAX_EVENTS);
    if (epfd == -1) {
        std::cout << "create epoll failed" << std::endl;
        close(sockfd);
        return -1;
    }

    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        std::cout << "add sockfd to epoll failed" << std::endl;
        close(sockfd);
        close(epfd);
        return -1;
    }

    std::cout << "server running" << std::endl;

    while (true) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            std::cout << "wait epoll events failed" << std::endl;
            continue;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == sockfd) { // new connection
                struct sockaddr_in client_addr;
                socklen_t addrlen = sizeof(client_addr);
                int connfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
                if (connfd == -1) {
                    std::cout << "accept new connection failed" << std::endl;
                    continue;
                }

                // set non-blocking
                int flags = fcntl(connfd, F_GETFL, 0);
                fcntl(connfd, F_SETFL, flags | O_NONBLOCK);

                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = connfd;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev) == -1) {
                    std::cout << "add connfd to epoll failed" << std::endl;
                    close(connfd);
                    continue;
                }
                std::cout << "accept new connection" << std::endl;
            } else { // read data
                char buff[MAX_BUFF_SIZE];
                int n = read(events[i].data.fd, buff, MAX_BUFF_SIZE);
                if (n == -1) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        std::cout << "read data from connfd failed" << std::endl;
                        epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                        close(events[i].data.fd);
                    }
                } else if (n == 0) {
                    std::cout << "connection closed by peer" << std::endl;
                    epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                } else {
                    std::cout << "recv data: " << buff << std::endl;
                    ev.events = EPOLLOUT | EPOLLET;
                    ev.data.fd = events[i].data.fd;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, events[i].data.fd, &ev);
                }
            }
        }
    }

    return 0;
}