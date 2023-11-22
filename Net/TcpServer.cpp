//
// Created by aaweo on 2023/11/20.
//

#include "TcpServer.h"
namespace Net {

    void TcpServer::init(int port, size_t threadSize) {
        _port = port;
        _threadPool = std::make_shared<ThreadPool>(threadSize);
        _listenFd = -1;
        _epollFd = -1;

        _createListenSocket();
        logger.Log(LOG_INFO, "TcpServer inited. Port = ", _port);
        _inited = true;
    }


    void TcpServer::run() {
        if (!_inited) {
            logger.Log(LOG_FATAL, "TcpServer not inited");
            exit(-1);
        }
        _epollFd = epoll_create(EPOLL_SIZE);
        if (_epollFd == -1) {
            logger.Log(LOG_FATAL, "Create Epoll Failed");
            exit(-1);
        }
        _epollAdd(_listenFd, EPOLLIN | EPOLLET);

        logger.Log(LOG_IMPT, "TcpServer running, Listening on ", _ipAddr, ":", _port);

        while (true) {
            epoll_event epollEvents[MAX_EVENTS];
            int epollEventCount = epoll_wait(_epollFd, epollEvents, MAX_EVENTS, -1);
            if (epollEventCount == -1) {
                logger.Log(LOG_FATAL, "Epoll Wait Failed");
                exit(-1);
            } else if (epollEventCount == 0) {
                logger.Log(LOG_WARN, "Epoll Wait Timeout");
                continue;
            } else {
                _epollRun(epollEvents, epollEventCount);
            }
        }

    }


    void TcpServer::_createListenSocket() {

        // 创建监听 socket
        _listenFd = socket(AF_INET, SOCK_STREAM, 0);
        if (_listenFd == -1) {
            logger.Log(LOG_FATAL, "Create Listen Socket Failed");
            exit(-1);
        }

        // 端口复用
        int opt = 1;
        setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(_port);
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        _ipAddr = inet_ntoa(serverAddr.sin_addr);
        if (bind(_listenFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            logger.Log(LOG_FATAL, "Bind Listen Socket Failed");
            exit(-1);
        }
        if (listen(_listenFd, LISTENQ) == -1) {
            logger.Log(LOG_FATAL, "Listen Failed");
            exit(-1);
        }
    }


    void TcpServer::_epollAdd(int fd, int event) {
        epoll_event epollEvent{};
        epollEvent.data.fd = fd;
        epollEvent.events = event;
        epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &epollEvent);
    }

    void TcpServer::_epollDel(int fd) {
        epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, nullptr);
    }

    void TcpServer::_epollRun(epoll_event *epollEvents, size_t epollEventCount) {
        logger.Log(LOG_DEBUG, "Epoll Event Count = ", epollEventCount);
        for (int i=0; i<epollEventCount; ++i) {
            int fd = epollEvents[i].data.fd;
            if (epollEvents[i].events & EPOLLIN) {
                if (fd == _listenFd) {
                    int commFd = _serverAccept();
                    _epollAdd(commFd, EPOLLIN);
                } else {
                    _threadPool->enqueue(_serverIO, fd);
                }
            }
        }
    }

    int TcpServer::_serverAccept() {
        sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);
        int commFd = accept(_listenFd, (sockaddr*)&clientAddr, &clientAddrLen);
        if (commFd == -1) {
            logger.Log(LOG_FATAL, "Accept Failed");
            exit(-1);
        }
        _epollAdd(commFd, EPOLLIN);
        _commFdMap[commFd] = std::make_pair(inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        logger.Log(LOG_INFO, "Accept Connection From ", _commFdMap[commFd].first, ":", _commFdMap[commFd].second, ", fd = ", commFd);
        return commFd;
    }

    void TcpServer::_serverIO(int fd) {
        char buffer[1024];
        ssize_t recvLen = recv(fd, buffer, sizeof(buffer), 0);
        if (recvLen == -1) {
            logger.Log(LOG_ERROR, "Recv Failed");
            return;
        } else if (recvLen == 0) {
            logger.Log(LOG_INFO, "Connection Closed, fd = ", fd, ", ip = ", TcpServer::getInstance()._commFdMap[fd].first, ", port = ", TcpServer::getInstance()._commFdMap[fd].second);
            close(fd);
            TcpServer::getInstance()._epollDel(fd);
            TcpServer::getInstance()._commFdMap.erase(fd);
            return;
        } else {
            buffer[recvLen] = '\0';
            logger.Log(LOG_DEBUG, "Recv From ", TcpServer::getInstance()._commFdMap[fd].first, ":", TcpServer::getInstance()._commFdMap[fd].second, ", fd = ", fd, ", message = ", buffer);

//            ssize_t sendLen = send(fd, buffer, recvLen, 0);
//            if (sendLen == -1) {
//                logger.Log(LOG_ERROR, "Send Failed");
//                return;
//            } else {
//                logger.Log(LOG_DEBUG, "Send To ", TcpServer::getInstance()._commFdMap[fd].first, ":", TcpServer::getInstance()._commFdMap[fd].second, ", fd = ", fd, ", message = ", buffer);
//            }
        }
    }




}

