#include "base/CountDownLatch.h"

namespace feng {

CountDownLatch::CountDownLatch(int count)
    : mutex_(), cond_(mutex_), count_(count) { }


void CountDownLatch::wait() {
    MutexLockGuard lock(mutex_);
    while (count_ > 0) {
        cond_.wait();
    }
}

void CountDownLatch::countDown() {
    MutexLockGuard lock(mutex_);
    --count_;
    if (count_ == 0) {
        cond_.notifyAll();
    }
}

}