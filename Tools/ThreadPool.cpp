//
// Created by aaweo on 2023/11/20.
//

#include "ThreadPool.h"


inline ThreadPool::ThreadPool(size_t threads) : _stop(false) {
    threads = threads < 1 ? 1 : threads;
    for (size_t i = 0; i < threads; ++i) {
        _workers.emplace_back(
                [this] {
                    for (;;) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(this->_queue_mutex);
                            this->_condition.wait(lock,
                                                  [this] { return this->_stop || !this->_tasks.empty(); });
                            if (this->_stop && this->_tasks.empty()) {
                                return;
                            }
                            task = std::move(this->_tasks.front());
                            this->_tasks.pop();
                        }
                        task();
                    }
                }
        );
    }
}



template<class F, class... Args>
auto ThreadPool::enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
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

inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(_queue_mutex);
        _stop = true;
    }
    _condition.notify_all();
    for (std::thread &worker : _workers) {
        worker.join();
    }
}
