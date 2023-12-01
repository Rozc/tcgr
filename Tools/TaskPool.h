//
// Created by UserName on 2023/11/29.
//

#ifndef TCGR_TASKPOOL_H
#define TCGR_TASKPOOL_H

#include <queue>
#include "Thread.h"
#include "ThreadPool.h"

class TaskQueue {
    friend class Task;
    friend class TaskPool;

protected:
    bool stopped_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasks_mutex_;
    std::condition_variable cond_;


public:
    TaskQueue() : stopped_(false) {};
    virtual ~TaskQueue() = default;

    void add_task(const std::function<void()>& task) {
        {
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            tasks_.push(task);
            cond_.notify_one();
        }

    }

    void add_task(std::function<void()>&& task) {
        {
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            tasks_.push(std::forward<std::function<void()>>(task));
            cond_.notify_one();
        }

    }

};

class Task final
        : public Thread {
private:
    TaskQueue *task_queue_;
public:
    explicit Task(TaskQueue *q)
    : task_queue_(q) {}

    void work_implement() override {
        std::function<void()> task;
        while (true) {
            {
                std::unique_lock<std::mutex> lock(task_queue_->tasks_mutex_);
                if (task_queue_->tasks_.empty()) {
                    if (task_queue_->stopped_) {
                        break;
                    }
                    task_queue_->cond_.wait(lock, [this]()->bool {
                        return (!task_queue_->tasks_.empty()) || task_queue_->stopped_;
                    });
                    if (task_queue_->tasks_.empty() && task_queue_->stopped_) {
                        break;
                    }
                }
                task = std::move(task_queue_->tasks_.front());
                task_queue_->tasks_.pop();
            } // unlock
            task_queue_->cond_.notify_all();
            task();
        }
    }

};

class TaskPool final :
        public TaskQueue,
        public ThreadPool<Task, TaskQueue*> {

private:


public:
    explicit TaskPool(int threads_num)
    : TaskQueue(), ThreadPool<Task, TaskQueue*>(threads_num, this) {}

    TaskPool(const TaskPool&) = delete;
    TaskPool& operator=(const TaskPool&) = delete;
    TaskPool(TaskPool&&) = delete;
    TaskPool& operator=(TaskPool&&) = delete;
    ~TaskPool() override = default;

    void stop() {
        {
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            stopped_ = true;
            cond_.notify_all();
        }
        join_all();
    }

//    template<typename F, typename ...Args>
//    auto add_task(F&& f, Args&& ...args) -> std::future<typename std::result_of<F(Args...)>::type> {
//        using return_type = typename std::result_of<F(Args...)>::type;
//        auto task = std::make_shared<std::packaged_task<return_type(void)>>(
//                std::bind(std::forward<F>(f), std::forward<Args>(args)...));
//        auto res = task->get_future();
//        task_queue_.add_task([task]() {
//            (*task)();
//        });
//
//
//        return res;
//    }

//    void stop() {
//        {
//            std::unique_lock<std::mutex> lock(task_queue_.tasks_mutex_);
//            task_queue_.stopped_ = true;
//            task_queue_.cond_.notify_all();
//        }
//        join_all();
//    }


};

#endif //TCGR_TASKPOOL_H
