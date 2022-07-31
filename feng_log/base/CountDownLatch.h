#ifndef __FENG_BASE_COUNTDOWNLATCH_H__
#define __FENG_BASE_COUNTDOWNLATCH_H__

#include "base/Condition.h"
#include "base/Mutex.h"
#include "base/noncopyable.h"

namespace feng {

class CountDownLatch : public noncopyable {
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();
private:
    mutable MutexLock mutex_;
    Condition cond_;
    int count_;
};

}


#endif  