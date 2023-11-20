//
// Created by aaweo on 2023/11/20.
//

#ifndef TCGR_SESSION_H
#define TCGR_SESSION_H


class Session {
public:
    virtual int sessionId() = 0;
    virtual int read(char* buff, int len) = 0;
    virtual int write(char* buff, int len) = 0;
    virtual int reWrite() = 0;

    virtual ~Session() = default;

};


#endif //TCGR_SESSION_H
