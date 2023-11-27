//
// Created by aaweo on 2023/11/20.
//

#ifndef TCGR_TCPSERVER_H
#define TCGR_TCPSERVER_H

#include "header.h"
#include <unordered_map>
#include "../Tools/ThreadPool.h"
#include "../Tools/Logger.h"

static const int EPOLL_SIZE = 1024;
static const int MAX_EVENTS = 1024;
static const int LISTENQ = 1024;
static const int MAXLINE = 256; // 一行的最大长度

extern Tools::Logger& logger;

namespace Net {
    class TcpServer {
    private:
        bool _inited;
        int _listenFd;
        std::string _ipAddr;
        int _port;
        int _epollFd;
        std::shared_ptr<ThreadPool> _threadPool;

        TcpServer() {
            _inited = false;
            _listenFd = -1;
            _epollFd = -1;
            _port = -1;
        }
        void _createListenSocket();
        void _epollAdd(int fd, int event);
        void _epollDel(int fd);
        void _epollRun(epoll_event epollEvents[], size_t epollEventCount);
        int _serverAccept();
        static void _receive(int fd);
    public:
        std::unordered_map<int, std::pair<std::string, int>> _commFdMap;
        std::mutex _commFdMapMutex;

        static TcpServer& getInstance() {
            static TcpServer instance;
            return instance;
        }
        ~TcpServer() {
            if (_listenFd >= 0) {
                close(_listenFd);
            }
        }

        void init(int port, size_t threadSize = 5);

        void run();


        TcpServer(const TcpServer&) = delete;
        TcpServer(TcpServer&&) = delete;
        TcpServer& operator=(TcpServer&) = delete;
        TcpServer& operator=(TcpServer&&) = delete;

    };


}




#endif //TCGR_TCPSERVER_H
