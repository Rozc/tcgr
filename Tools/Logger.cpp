//
// Created by aaweo on 2023/11/20.
//

#include "Logger.h"



namespace Tools {
    std::ostream &operator<<(std::ostream &os, Tools::Code code) {
        return os << "\033[" << static_cast<int>(code) << "m";
    }

    Logger &Logger::getInstance() {
        static Logger instance;
        return instance;
    }

    Logger::Logger() {
        _currentLevel = 0;
        _maxLevel = 5;
        _timeNow = time(nullptr);
        _currentTime = new char[20];
        _levelString = new char*[5];
        _levelColor = new Code[5];

        setLevelProperty(LOG_DEBUG, GREEN, "LOG_DEBUG");
        setLevelProperty(LOG_INFO, DEFAULT, "LOG_INFO");
        setLevelProperty(LOG_IMPT, CYAN, "LOG_IMPT");
        setLevelProperty(LOG_WARN, YELLOW, "LOG_WARN");
        setLevelProperty(LOG_ERROR, MAGENTA, "LOG_ERROR");
        setLevelProperty(LOG_FATAL, RED, "LOG_FATAL");

    }

    Logger::~Logger() {
        delete[] _currentTime;
        delete[] _levelString;
        delete[] _levelColor;
    }

    void Logger::_setTime() {
        // TODO: convert time to UTC+8
        _timeNow = time(nullptr);
        strftime(_currentTime, 20, "%Y-%m-%d %H:%M:%S", localtime(&_timeNow));
    }

//    void Logger::Log(Tools::Level level, const char *message) {
//        _setTime();
//        std::cout << _levelColor[level];
//        std::cout << "[" << _currentTime << "] ";
//        std::cout << "[" << _levelString[level] << "] ";
//        std::cout << message;
//        std::cout << DEFAULT << std::endl;
//    }





    void Logger::setLevelProperty(int level, Code color, const char *levelString) {
        _levelColor[level] = color;
        _levelString[level] = const_cast<char *>(levelString);
    }
}


