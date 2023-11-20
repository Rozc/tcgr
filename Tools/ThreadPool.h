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

class ThreadPool {
public:
    ThreadPool() = delete;
    explicit ThreadPool(size_t);
    template<class F, class... Args>
            auto enqueue(F &&f, Args &&... args)
                -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();

private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;

    std::mutex _queue_mutex;
    std::condition_variable _condition;
    std::atomic<bool> _stop;
};


#endif //TCGR_THREADPOOL_H
