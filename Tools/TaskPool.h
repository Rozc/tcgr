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

protected:
    std::queue<std::function<void(void)>> tasks_;
    std::mutex mutex_;
    std::condition_variable cond_;
    bool stopped_;

public:
    TaskQueue() {
        stopped_ = false;
    }

    ~TaskQueue() = default;


    void add_task(std::function<void(void)> task) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.push(std::move(task));
        }
        cond_.notify_all();
    }

    bool empty() {
        std::unique_lock<std::mutex> lock(mutex_);
        return tasks_.empty();
    }

    size_t size() {
        std::unique_lock<std::mutex> lock(mutex_);
        return tasks_.size();
    }

    void clear() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!tasks_.empty()) {
            tasks_.pop();
        }
    }
};

class Task final
        : public Thread {
private:
    TaskQueue *task_queue_;
public:
    explicit Task(TaskQueue *q) noexcept
    : task_queue_(q) {}

    void work_implement() override {
        while (true) {
            std::function<void(void)> task;
            {
                std::unique_lock<std::mutex> lock(task_queue_->mutex_);
                if (task_queue_->tasks_.empty()) {
                    if (task_queue_->stopped_) {
                        break;
                    }
                    task_queue_->cond_.wait(lock, [this]()->bool {
                        return !task_queue_->tasks_.empty() || task_queue_->stopped_;
                    });
                    if (task_queue_->tasks_.empty() && task_queue_->stopped_) {
                        break;
                    }
                }
                task = std::move(task_queue_->tasks_.front());
                task_queue_->tasks_.pop();
            }
            task_queue_->cond_.notify_all();
            task();
        }
    }

};

class TaskPool final :
        public ThreadPool<Task, TaskQueue*> {

private:
    TaskQueue task_queue_;

public:
    explicit TaskPool(int threads_num)
    :task_queue_(), ThreadPool<Task, TaskQueue*>(threads_num, &task_queue_) {}

    TaskPool(const TaskPool&) = delete;
    TaskPool& operator=(const TaskPool&) = delete;
    TaskPool(TaskPool&&) = delete;
    TaskPool& operator=(TaskPool&&) = delete;
    ~TaskPool() override = default;

    template<typename F, typename ...Args>
    auto add_task(F&& f, Args&& ...args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type(void)>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        auto res = task->get_future();
        task_queue_.add_task([task]() {
            (*task)();
        });
        return res;

    }


};

#endif //TCGR_TASKPOOL_H
