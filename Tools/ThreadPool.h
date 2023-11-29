//
// Created by aaweo on 2023/11/20.
//

#ifndef TCGR_THREADPOOL_H
#define TCGR_THREADPOOL_H

#include <iostream>
#include <cstdlib>
#include <queue>
#include <mutex>			  // 锁
#include <thread>			  // 线程
#include <condition_variable> // 条件变量
#include <functional>		  // 接受所有类似函数的东西
#include <future>			  // 来自未来的值
#include <atomic>			  // 原子操作
#include <memory>			  // 智能指针
#include <stdexcept> 	      // C++异常

#include "Thread.h"

template<typename ThreadType, typename... Args>
class ThreadPool {
    static_assert(std::is_base_of<Thread, ThreadType>::value, "Not thread");
    static_assert(std::is_constructible<ThreadType, Args &&...>::value, "Not constructible");
public:
    explicit ThreadPool(size_t threads_num, Args&&... args) {
        for (size_t i = 0; i < threads_num; ++i) {
            threads_.emplace_back(std::make_unique<ThreadType>(std::forward<Args>(args)...));
        }
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = default;
    ThreadPool& operator=(ThreadPool&&) = default;
    virtual ~ThreadPool() = default;

    void run() {
        stopped_ = false;
        for (auto& thread : threads_) {
            thread->run();
        }
    }

    void stop() {
        stopped_ = true;
        join_all();
    }

    void join_all() {
        for (auto& thread : threads_) {
            thread->join();
        }
    }

    void detach_all() {
        for (auto& thread : threads_) {
            thread->detach();
        }
    }


protected:
    std::vector<std::unique_ptr<ThreadType>> threads_;
    std::atomic<bool> stopped_{true};
};


#endif //TCGR_THREADPOOL_H
