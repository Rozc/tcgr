//
// Created by Rozc on 2023/11/22.
//

#ifndef TCGR_SERVERIO_H
#define TCGR_SERVERIO_H

// transfer data between TcpServer(Net) and Room(Game)

#include <unordered_map>
#include "../Tools/Logger.h"
#include "../Game/Room.h"
#include "../include/json/json.h"
#include <string>

// 进来的消息 先检查 fd 是否在 map 中
// 如果在，就是已经有房间了，解析后转发给对应房间
// 如果不在, 就解析用户信息然后加入匹配队列
// 对应匹配队列如果存在等待中的用户, 则完成匹配, 并将对应 fd 加入 map 中.
// 否则在匹配队列中等待.


struct Msg {
    int fd;
    Json::Value data;
};

class ServerIO {
private:
    static std::unordered_map<int, Game::Room*> _commFdMap;
public:
    static std::string json2str(const Json::Value& json, bool styled = false) {
        if (styled) {
            Json::StreamWriterBuilder writer;
            std::string jsonString = Json::writeString(writer, json);
            return jsonString;
        } else {
            Json::FastWriter writer;
            std::string jsonString = writer.write(json);
            return jsonString;
        }
    }

    static bool str2json(const std::string& str, Json::Value* json) {
        Json::CharReaderBuilder reader;
        std::istringstream iss(str);

        std::string errs;
        bool ok = Json::parseFromStream(reader, iss, json, &errs);
        if (!ok) {
            mlog::logger.Log(LOG_ERROR, "json parse error: ", errs);
            return false;
        }
        return true;
    }

public:
    static bool Recv(int fd, const std::string& str) {
        Msg msg;
        msg.fd = fd;
        msg.data = Json::Value();
        if (!str2json(str, &msg.data)) {
            return false;
        }
        // 检查 fd 是否在 map 中
        if (_commFdMap.find(fd) == _commFdMap.end()) {
            // 不在, 说明是新用户, 将其加入匹配队列
            // TODO 将消息转发给匹配队列
            return true;
        } else {
            // 在, 说明已经有房间了, 将消息转发给对应房间
            // TODO 将消息转发给对应房间
            return true;
        }


        return true;
    }

    bool Send(int fd, const std::string& str) {
        // 接收来自房间的消息, 转发给对应 fd
        return false;
    }

    void resolve(char* msg, int len);
};


#endif //TCGR_SERVERIO_H
