//
// Created by aaweo on 2023/11/20.
//

#ifndef TCGR_LOGGER_H
#define TCGR_LOGGER_H

#include <iostream>
#include <ctime>
#include <mutex>
#include "TaskPool.h"

#define LOG_DEBUG 16
#define LOG_INFO 32
#define LOG_FOCUS 64
#define LOG_WARN 128
#define LOG_ERROR 256
#define LOG_FATAL 512

#define LOG_FILE 1
#define LOG_CONS 2
#define LOG_ASYNC 4
#define LOG_BLOCK 8

// TODO: 可选打印行号和文件名

namespace mlog {

enum class Code {
    DEFAULT = 39,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    GRAY = 37
};

std::ostream &operator<<(std::ostream &os, Code code);

typedef unsigned int config_t;


class Logger {
private:
    Logger();
    ~Logger();
    int _currentLevel;
    int _maxLevel;
    time_t _timeNow;
    char *_currentTime;
    char **_levelString;
    Code *_levelColor;

    int default_config = LOG_CONS | LOG_ASYNC;


    std::mutex _mx;
    TaskPool* _taskPool;

    void _setTime();

    template<typename T>
    inline int _printEach(const T &arg) {
        std::cout << arg;
        return 0;
    }

    template<typename ...Args>
    void _log(std::ostream& out, unsigned level, const Args &... args);

public:

    static Logger &getInstance();
    void setLevelProperty(int level, Code color, const char *levelString);


    template<typename ...Args>
    void Log(config_t config, const Args &... args);

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void stop() {
        _taskPool->stop();
    }


};

template<typename... Args>
void Logger::_log(std::ostream& out, unsigned level, const Args &... args) {
    std::unique_lock<std::mutex> lock(_mx);
    _setTime();

    std::cout << _levelColor[level];
    std::cout << "[" << _currentTime << "] ";
    std::cout << "[" << _levelString[level] << "] ";

    int _[] = {_printEach(args)...};

    std::cout << Code::DEFAULT << std::endl;
}

template<typename... Args>
void Logger::Log(config_t config, const Args &... args) {
    config_t target = config & 0b11, sync = config & 0b1100, level = config >> 4;
    if (target == 0) {
        target = default_config & 0b11;
    }
    if (sync == 0) {
        sync = default_config & 0b1100;
    }

    unsigned level_int = 0;
    while (level) {
        if (level & 1) {
            break;
        } else {
            level_int++;
            level >>= 1;
        }
    }

    if (level_int > _maxLevel) {
        _log(std::cout, LOG_ERROR, "Invalid Log Level: ", level);
        return;
    }
    if (level_int < _currentLevel) {
        return;
    }
    _taskPool->add_task([=]() {
        _log(std::cout, level_int, args...);
    });
}



// extern Logger& logger;




} // namespace Tools


#endif //TCGR_LOGGER_H
