//
// Created by aaweo on 2023/11/20.
//

#ifndef TCGR_LOGGER_H
#define TCGR_LOGGER_H

#include <iostream>
#include <ctime>

#define DEBUG 0
#define INFO 1
#define IMPORTANT 2
#define WARN 3
#define ERROR 4
#define FATAL 5

namespace Tools {
    enum Code {
        DEFAULT = 39,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        GRAY = 37
    };
    std::ostream& operator<<(std::ostream& os, Code code);

    class Logger {
    public:
        static Logger& getInstance();
        // static Logger& getInstance(int level);
        void setLevelProperty(int level, Code color, const char* levelString);

        template<typename ...Args>
        void Log(int level, const Args&... args) {
            if (level < 0 || level > _maxLevel) {
                Log(FATAL, "Invalid Log Level");
                return;
            }
            if (level < _currentLevel) {
                return;
            }
            _setTime();
            std::cout << _levelColor[level];
            std::cout << "[" << _currentTime << "] ";
            std::cout << "[" << _levelString[level] << "] ";

            int _[] = { _printEach(args)... };

            std::cout << DEFAULT << std::endl;
        }

        template<class T>
        int _printEach(const T& arg) {
            std::cout << arg;
            return 0;
        }


        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
    private:
        Logger();
        ~Logger();

        int _currentLevel;
        int _maxLevel;
        time_t _timeNow;
        char* _currentTime;
        char** _levelString;
        Code* _levelColor;

        void _setTime();

    };
}




#endif //TCGR_LOGGER_H
