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
    explicit ThreadPool(size_t);

    template<class F, class... Args>
    auto enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()> >(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(_queue_mutex);
            if (_stop) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            _tasks.emplace([task]() { (*task)(); });
        }
        _condition.notify_one();
        return res;
    }

    ~ThreadPool();

private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;

    std::mutex _queue_mutex;
    std::condition_variable _condition;
    std::atomic<bool> _stop;
};


#endif //TCGR_THREADPOOL_H
