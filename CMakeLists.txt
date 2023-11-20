cmake_minimum_required(VERSION 3.22)
project(tcgr)

set(CMAKE_CXX_STANDARD 17)

add_executable(tcgr main.cpp
        Tools/Logger.cpp
        Tools/Logger.h
        Net/TcpServer.cpp
        Net/TcpServer.h
        Net/Session.h
        Net/header.h
        Tools/ThreadPool.cpp
        Tools/ThreadPool.h)