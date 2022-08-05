#ifndef __FENG_BASE_LOCK_H
#define __FENG_BASE_LOCK_H

#include <pthread.h>
#include "base/noncopyable.h"
#include <cstdio>

namespace feng {

class MutexLock : noncopyable {    
friend class Condition;
public:
    MutexLock() {
        // 初始化互斥锁
        pthread_mutex_init(&mutex_, NULL);
    }

    ~MutexLock() {
        pthread_mutex_destroy(&mutex_);
    }

    void lock() {
        pthread_mutex_lock(&mutex_);
    }

    void unlock() {
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t* getPthreadMutex() {
        return &mutex_;
    }
private:
    pthread_mutex_t mutex_;
};

/* RAII */
class MutexLockGuard : noncopyable {

public:
    explicit MutexLockGuard(MutexLock &mutex) : mutex_(mutex) {
        mutex_.lock();
    }

    ~MutexLockGuard() {
        mutex_.unlock();    
    }

private:
    // 引用成员变量
    MutexLock& mutex_; 
};

}



#endif