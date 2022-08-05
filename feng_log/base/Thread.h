#ifndef __FENG_BASE_THREAD_H__
#define __FENG_BASE_THREAD_H__

#include <pthread.h>
#include "base/noncopyable.h"
#include <functional>
#include <string>
#include <atomic>
#include <thread>
#include <memory>

namespace feng {

class Thread : noncopyable {
public:
    typedef std::shared_ptr<Thread> ptr;
    typedef std::function<void()> Task;

    explicit Thread(Task task, const std::string& name = "");
    ~Thread();
    
    void join();
    // 线程存在 并且没有被回收 即可以被回收 返回true
    bool joinable() { return !joined_ && pthreadId_; }
    bool isStarted() const { return started_; }
    
    static const Thread* getCurrentThread();
    static const pthread_t getCurrentThreadId();
    static const std::string& getCurrentThreadName();
    
    const std::string& name() const { return name_; }
    static const int getThreadCount() { return numThread_; }  

private:
    void setDefaultName();
    static void* run(void*);

private:
    pthread_t                pthreadId_;  // 线程号
    bool                     joined_;     // 是否要join
    Task                     task_;       // 任务函数
    std::string              name_;       // 名字
    bool                     started_;    // 是否启动
    static std::atomic<int>  numThread_;  // 线程数量
};

}


#endif