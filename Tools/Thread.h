//
// Created by UserName on 2023/11/28.
//

#ifndef TCGR_THREAD_H
#define TCGR_THREAD_H

#include <thread>
#include <future>

class Thread {

private:
    std::thread* thread_;
    std::promise<bool> promise_;
    std::future<bool> future_;

public:
    Thread() {
        thread_ = nullptr;
    }

    virtual ~Thread() {
        delete thread_;
    }

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
    Thread(Thread&&) = delete;
    Thread& operator=(Thread&&) = delete;

    void run() {
        thread_ = new std::thread(&Thread::work_implement, this);
        future_ = promise_.get_future();
    }

    void stop() {
        promise_.set_value(true);
        thread_->join();
    }

    void join() {
        thread_->join();
    }

    void detach() {
        thread_->detach();
    }

    virtual void work_implement() = 0;

    bool is_running() {
        return future_.wait_for(std::chrono::seconds(0)) == std::future_status::timeout;
    }
};


#endif //TCGR_THREAD_H
