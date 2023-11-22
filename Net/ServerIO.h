//
// Created by Rozc on 2023/11/22.
//

#ifndef TCGR_SERVERIO_H
#define TCGR_SERVERIO_H

// transfer data between TcpServer(Net) and Room(Game)

#include <unordered_map>
#include "../Game/Room.h"

// 进来的消息 先检查 fd 是否在 map 中
// 如果在，就是已经有房间了，解析后转发给对应房间
// 如果不在, 就解析用户信息然后加入匹配队列
// 对应匹配队列如果存在等待中的用户, 则完成匹配, 并将对应 fd 加入 map 中.
// 否则在匹配队列中等待.

class ServerIO {
private:
    std::unordered_map<int, Room*> _commFdMap;
public:
    bool Recv(int fd, char* buf, int len);
    bool Send(int fd, char* buf, int len);
    void resolve(char* msg, int len);
};


#endif //TCGR_SERVERIO_H
