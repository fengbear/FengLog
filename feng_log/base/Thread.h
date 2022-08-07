#ifndef __FENG_BASE_THREAD_H__
#define __FENG_BASE_THREAD_H__

#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <functional>
#include <memory>
#include <string>
#include "base/CountDownLatch.h"
#include "base/noncopyable.h"

namespace feng{

class Thread : noncopyable {

public:
    typedef std::shared_ptr<Thread> ptr;
    typedef std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc&, const std::string& name = std::string());
    ~Thread();
    void start();
    int join();
    bool started() const { return started_; }
    pid_t tid() const { return tid_; }
    const std::string& name() const { return name_; }

private:
    void setDefaultName();            // 设置默认线程名
    bool started_;                    // 是否启动
    bool joined_;                     // 是否被回收
    pthread_t pthreadId_;             // 线程号        pthread_self返回  一个进程内线程号不同
    pid_t tid_;                       // 真实的线程号   gettid返回 不同进程的线程号也会不同 
    ThreadFunc func_;                 // 任务函数
    std::string name_;                // 线程名
    CountDownLatch latch_;            // 计数+条件变量，计数等于0时条件变量做通知
};

}

#endif