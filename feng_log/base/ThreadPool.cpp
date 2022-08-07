#include "base/ThreadPool.h"

namespace feng {


ThreadPool::~ThreadPool() {
    isRunning_ = false;
    cond_.notifyAll();
    for (auto &t : threads_) {
        t->join();
    }
}

void ThreadPool::addTask(const Task &task) {
    if (!isRunning_) return;
    {
        MutexLockGuard lock(mtx_);
        taskPool_.emplace(task);
    }
    cond_.notify();
}

void ThreadPool::start() {
    isRunning_ = true;
    for (size_t i = 0; i < num_thread_; i++) {
        Thread::ptr p = std::make_shared<Thread>(std::bind(&ThreadPool::run, this));
        p->start();
        threads_.emplace_back(p);
    }
}

void ThreadPool::run() {
    Task task;
    while (isRunning_) {
        {
            MutexLockGuard lock(mtx_);
            while (isRunning_ && taskPool_.empty()) {
                cond_.wait();
            }
            if (!isRunning_) break;
            task = taskPool_.front();
            taskPool_.pop();            
        }
        task();
    }
}

}