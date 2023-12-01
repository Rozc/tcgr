//
// Created by UserName on 2023/11/28.
//

#ifndef TCGR_THREAD_H
#define TCGR_THREAD_H

#include <pthread.h>
#include <future>
#include <csignal>

class Thread {

private:
    pthread_t thread_;
    std::promise<bool> promise_;
    std::future<bool> future_;

public:
    Thread() {
        future_ = promise_.get_future();
    }

    virtual ~Thread() {
    }

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
    Thread(Thread&&) = delete;
    Thread& operator=(Thread&&) = delete;

    void run() {
        auto func = [](void* arg) -> void* {
            Thread* pseudo_this = static_cast<Thread*>(arg);
            if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr) != 0) {
                throw std::logic_error("pthread_setcancelstate error");
            }
            if (pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr) != 0) {
                throw std::logic_error("pthread_setcanceltype error");
            }
            pseudo_this->work_implement();
            pseudo_this->promise_.set_value(true);
            return nullptr;
        };
        int ret = pthread_create(&thread_, nullptr, func, this);
        if (ret != 0) {
            throw std::system_error(ret, std::system_category(), "pthread_create error");
        }
    }

    void join() {
        int ret = pthread_join(thread_, nullptr);
        if (ret != 0) {
            throw std::system_error(ret, std::system_category(), "pthread_join error");
        }
    }

    void detach() {
        int ret = pthread_detach(thread_);
        if (ret != 0) {
            throw std::system_error(ret, std::system_category(), "pthread_detach error");
        }
    }

    template<typename Rep, typename Period>
    bool wait_for(const std::chrono::duration<Rep, Period> &span) {
        return future_.wait_for(span) == std::future_status::ready;
    }

    void send_signal(int signal) {
        pthread_kill(thread_, signal);
    }

    virtual void work_implement() = 0;


};


#endif //TCGR_THREAD_H
