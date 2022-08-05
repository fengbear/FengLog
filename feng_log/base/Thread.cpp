#include "base/Thread.h"
#include <iostream>

namespace feng {

std::atomic<int> Thread::numThread_{0};

static thread_local Thread* cur_thread = nullptr;
static thread_local std::string cur_thread_name = "UNKONW";

const Thread* Thread::getCurrentThread() {
    return cur_thread;
}

const pthread_t Thread::getCurrentThreadId() {
    return pthread_self();
}

const std::string& Thread::getCurrentThreadName() {
    return cur_thread_name;
}

Thread::Thread(Task task, const std::string& name)
    : pthreadId_(0),
      joined_(false),
      task_(std::move(task)),
      name_(name),
      started_(false) {
        ++numThread_;
        setDefaultName();
        int ret = pthread_create(&pthreadId_, nullptr, &Thread::run, this);
        if (ret) {
            throw std::logic_error("pthread_create error");
        }
        while(!started_); // 等待线程成功运行
    }

Thread::~Thread() {
    if (pthreadId_) {
        pthread_detach(pthreadId_);
    }
    --numThread_;
}

void Thread::setDefaultName()
{
    if (name_.empty()) {
        int curThreadCnt = numThread_;
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread_%d", curThreadCnt);
        name_ = buf;
    }
}

void* Thread::run(void* args) {
    Thread* t = static_cast<Thread*>(args);
    cur_thread = t;
    cur_thread_name = t->name_;
    t->started_ = true;
    t->task_();
    return nullptr;
}

// 回收线程
void Thread::join() {
    if (pthreadId_) {
        int r = pthread_join(pthreadId_, nullptr);
        if (r) {
            throw std::logic_error("pthread_join error");
        }
        pthreadId_ = 0;
        joined_ = true;
    }
}

}