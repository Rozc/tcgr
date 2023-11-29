//
// Created by aaweo on 2023/11/20.
//

#ifndef TCGR_LOGGER_H
#define TCGR_LOGGER_H

#include <iostream>
#include <ctime>
#include <mutex>
#include "Thread.h"
#include "TaskPool.h"

#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_FOCUS 2
#define LOG_WARN 3
#define LOG_ERROR 4
#define LOG_FATAL 5



namespace Tools {

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

    std::mutex _mx;
    TaskPool* _taskPool;

    void _setTime();



    template<class T>
    inline int _printEach(const T &arg) {
        std::cout << arg;
        return 0;
    }

public:
    template<typename ...Args>
    void _log(int level, const Args &... args) {
        if (level < 0 || level > _maxLevel) {
            _log(LOG_FATAL, "Invalid Log Level");
            return;
        }
        if (level < _currentLevel) {
            return;
        }
        std::unique_lock<std::mutex> lock(_mx);
        _setTime();
        std::cout << _levelColor[level];
        std::cout << "[" << _currentTime << "] ";
        std::cout << "[" << _levelString[level] << "] ";

        int _[] = {_printEach(args)...};

        std::cout << Code::DEFAULT << std::endl;
    }

    static Logger &getInstance();
    void setLevelProperty(int level, Code color, const char *levelString);

    template<typename ...Args>
    void Log(int level, const Args &... args) {
        _taskPool->add_task(std::bind(&Logger::_log<Args...>), this, level, args...);
    }



    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
};


extern Logger& logger;




} // namespace Tools


#endif //TCGR_LOGGER_H
