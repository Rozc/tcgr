cmake_minimum_required(VERSION 3.22)
project(tcgr)

set(CMAKE_CXX_STANDARD 17)

add_executable(uni_threads unitest/unitest_threads.cpp
        Tools/ThreadPool.h
        Tools/TaskPool.h
        Tools/Thread.h)

add_executable(uni_logger unitest/unitest_logger.cpp
        Tools/Logger.h
        Tools/ThreadPool.h
        Tools/TaskPool.h
        Tools/Thread.h)

add_executable(tcgr main.cpp
        Tools/Logger.h
        Net/TcpServer.cpp
        Net/TcpServer.h
        Net/header.h
        Tools/ThreadPool.h
        Game/Room.cpp
        Game/Room.h
        Net/ServerIO.cpp
        Net/ServerIO.h
        Game/Player.cpp
        Game/Player.h
        include/json/json.h
        include/jsoncpp.cpp
        Game/MatchQueue.cpp
        Game/MatchQueue.h
        Game/GameServer.cpp
        Game/GameServer.h
        Tools/Poolable.cpp
        Tools/Poolable.h
        Tools/ObjectPool.cpp
        Tools/ObjectPool.h
        Tools/Thread.h
        Tools/TaskPool.h)

add_executable(tcgr_client Client/client.cpp)
