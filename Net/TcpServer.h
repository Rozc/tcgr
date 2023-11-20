//
// Created by aaweo on 2023/11/20.
//

#ifndef TCGR_TCPSERVER_H
#define TCGR_TCPSERVER_H

#include "header.h"
#include <unordered_map>
#include "../Tools/ThreadPool.h"
#include "../Tools/Logger.h"

namespace Net {
    class TcpServer {
    private:
        int _listenFd;
        std::string _ipAddr;
        int _port;
        int _epollFd;
        std::shared_ptr<ThreadPool> _threadPool;

        TcpServer() = default;
        void _createListenSocket();
        void _epollAdd(int fd, int event);
        void _epollDel(int fd);
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

        void init(int port, size_t threadSize = 5) {
            _port = port;
            _threadPool = std::make_shared<ThreadPool>(threadSize);
            _listenFd = -1;
            _epollFd = -1;

            _createListenSocket();
        }


        TcpServer(const TcpServer&) = delete;
        TcpServer(TcpServer&&) = delete;
        TcpServer& operator=(TcpServer&) = delete;
        TcpServer& operator=(TcpServer&&) = delete;

    };


}




#endif //TCGR_TCPSERVER_H
