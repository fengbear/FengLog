#ifndef __FENG_BASE_CONDITION_H
#define __FENG_BASE_CONDITION_H

#include <semaphore.h>
#include <pthread.h>
#include "base/Mutex.h"
#include "base/noncopyable.h"
#include <errno.h>
#include <time.h>
#include <memory>

namespace feng {

class Condition : noncopyable {
public:
    typedef std::shared_ptr<Condition> ptr;
    explicit Condition(MutexLock& mutex) : mutex_(mutex) {
        pthread_cond_init(&pcond_, NULL);
    }

    ~Condition() {
        pthread_cond_destroy(&pcond_);
    }

    void wait() {
        pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
    }

    void notify() {
        pthread_cond_signal(&pcond_);
    }

    void notifyAll() {
        pthread_cond_broadcast(&pcond_);
    }

    // 先使用int 不用算纳秒
    // 超时返回true 
    bool waitForSeconds(int seconds);

private:

    MutexLock& mutex_;
    pthread_cond_t pcond_;

};

}
#endif