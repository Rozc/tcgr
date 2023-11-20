//
// Created by aaweo on 2023/11/20.
//

#include "TcpServer.h"
namespace Net {
    void TcpServer::_createListenSocket() {
        _listenFd = socket(AF_INET, SOCK_STREAM, 0);
        if (_listenFd == -1) {
            Tools::Logger::getInstance().Log(Tools::FATAL, "Create Listen Socket Failed");
            exit(-1);
        }

        _epollAdd(_listenFd, EPOLLIN | EPOLLET);
        Tools::Logger::getInstance().Log(Tools::INFO, "Listen on");
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
}

