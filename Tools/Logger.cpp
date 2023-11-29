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
        _levelString = new char*[_maxLevel+1];
        _levelColor = new Code[_maxLevel+1];

        _taskPool = new TaskPool(1);

        setLevelProperty(LOG_DEBUG, Code::GREEN, "DEBUG");
        setLevelProperty(LOG_INFO, Code::DEFAULT, "INFO");
        setLevelProperty(LOG_FOCUS, Code::CYAN, "FOCUS");
        setLevelProperty(LOG_WARN, Code::YELLOW, "WARN");
        setLevelProperty(LOG_ERROR, Code::MAGENTA, "ERROR");
        setLevelProperty(LOG_FATAL, Code::RED, "FATAL");

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

    Logger& logger = Logger::getInstance();
}


