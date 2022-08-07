#ifndef __FENG_BASE_THREADPOOL_H__
#define __FENG_BASE_THREADPOOL_H__

#include <functional>
#include <queue>
#include <mutex>
#include <atomic>
#include "base/noncopyable.h"
#include "base/Thread.h"
#include "base/Mutex.h"
#include "base/Condition.h"

namespace feng {

using Task = std::function<void()>;

class ThreadPool : noncopyable {
public:
    static ThreadPool& getInstance() {
        static ThreadPool pool;
        return pool;
    }
    ~ThreadPool();
    void addTask(const Task &task);
    void start();
    bool isRunning() const {return isRunning_;}
    size_t getThreadNum() const {return num_thread_;}

private:
    ThreadPool(size_t ThreadNum = 4) : num_thread_(ThreadNum), mtx_(), cond_(mtx_) { }  
    void run();

private:
    std::queue<Task> taskPool_;
    std::vector<Thread::ptr> threads_;
    size_t num_thread_;
    std::atomic<bool> isRunning_;
    MutexLock mtx_;
    Condition cond_;

};



}




#endif