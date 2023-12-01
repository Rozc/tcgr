//
// Created by aaweo on 2023/11/20.
//

#ifndef TCGR_LOGGER_H
#define TCGR_LOGGER_H

#include <iostream>
#include <ctime>
#include <mutex>
#include <fstream>
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

enum class Color {
    DEFAULT = 39,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    GRAY = 37
};

std::ostream &operator<<(std::ostream &os, mlog::Color code) {
    return os << "\033[" << static_cast<int>(code) << "m";
}

using config_t = unsigned int;
using level_bit = unsigned int;


class Logger {

private:
    int _current_level;
    int _max_level;
    time_t _time_now;
    char *_current_time_string;
    std::vector<std::string> _level_names;
    std::vector<Color> _level_color;

    int _default_config = LOG_CONS | LOG_ASYNC;
    std::string _log_file_path;
    std::ofstream _log_file;

    std::mutex _mx;
    TaskPool* _task_pool;


    Logger() {
        _current_level = 0;
        _max_level = 5;
        _time_now = time(nullptr);
        _current_time_string = new char[20];
        _level_color.resize(_max_level + 1);
        _level_names.resize(_max_level + 1);

        _task_pool = new TaskPool(2);
        _task_pool->run();

        set_level_property(LOG_DEBUG, Color::GREEN, "DEBUG");
        set_level_property(LOG_INFO, Color::DEFAULT, "INFO");
        set_level_property(LOG_FOCUS, Color::CYAN, "FOCUS");
        set_level_property(LOG_WARN, Color::YELLOW, "WARN");
        set_level_property(LOG_ERROR, Color::MAGENTA, "ERROR");
        set_level_property(LOG_FATAL, Color::RED, "FATAL");

    }
    ~Logger() {
        delete[] _current_time_string;
        delete _task_pool;
    }


    void _set_time_string() {
        // TODO: convert time to UTC+8
        _time_now = time(nullptr);
        strftime(_current_time_string, 20, "%Y-%m-%d %H:%M:%S", localtime(&_time_now));
    }

    template<typename T>
    inline int _out_each(std::ostream& out, const T &arg) {
        out << arg;
        return 0;
    }

    template<typename ...Args>
    void _log(unsigned level, bool to_file, const Args &... args);

public:

    static Logger &get_instance() {
        static Logger instance;
        return instance;
    }
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void set_level_property(int level, Color color, const std::string& levelString) {
        level >>= 4;
        if (level == 0) {
            exit(-1);
        }
        unsigned level_int = _get_level(level);

        _level_color[level_int] = color;
        _level_names[level_int] = levelString;
    }

    void set_output_file(std::string& file_name) {
        _log_file_path = file_name;
        _log_file.open(_log_file_path, std::ios::out | std::ios::app);
    }

    template<typename ...Args>
    void Log(config_t config, const Args &... args);

    void stop() {
        _task_pool->stop();
    }

    inline unsigned _get_level(level_bit level) {
        unsigned level_int = 0;
        while (level) {
            if (level & 1) {
                break;
            } else {
                level_int++;
                level >>= 1;
            }
        }
        return level_int;
    }


};

template<typename... Args>
void Logger::_log(unsigned level, bool to_file, const Args &... args) {
    std::ostream out(nullptr);
    if (to_file) {
        if (_log_file.is_open()) {
            out.rdbuf(_log_file.rdbuf());
        } else {
            Log(LOG_ERROR | LOG_CONS | LOG_BLOCK, "Failed to open log file, path: ", _log_file_path);
            return;
        }
    } else {
        out.rdbuf(std::cout.rdbuf());
    }
    std::unique_lock<std::mutex> lock(_mx);
    _set_time_string();

    if (!to_file) out << _level_color[level];
    out << "[" << _current_time_string << "] ";
    out << "[" << _level_names[level] << "] ";

    int _[] = {_out_each(out, args)...};

    if(!to_file) out << Color::DEFAULT << std::endl;
}

template<typename... Args>
void Logger::Log(config_t config, const Args &... args) {
    config_t target = config & 0b11, sync = config & 0b1100;
    level_bit level = config >> 4;
    if (target == 0) {
        target = _default_config & 0b11;
    }
    if (sync == 0) {
        sync = _default_config & 0b1100;
    }

    unsigned level_int = _get_level(level);

    if (level_int > _max_level) {
        _log(LOG_ERROR, false, "Invalid Log Level: ", level);
        return;
    }
    if (level_int < _current_level) {
        return;
    }

    if (sync & LOG_BLOCK) {
        if (target & LOG_FILE) {
            _log(level_int, true, args...);
        }
        if (target & LOG_CONS) {
            _log(level_int, false, args...);
        }
    } else {
        _task_pool->add_task([=]() {
            if (target & LOG_FILE) {
                _log(level_int, true, args...);
            }
            if (target & LOG_CONS) {
                _log(level_int, false, args...);
            }
        });
    }

}



// extern Logger& logger;




} // namespace Tools


#endif //TCGR_LOGGER_H
