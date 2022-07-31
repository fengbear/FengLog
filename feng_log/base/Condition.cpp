#include "base/Condition.h"
/*

int clock_gettime(clockid_t clk_id, struct timespec *tp);
    clk_id:
        CLOCK_REALTIME: 系统实时时间,随系统实时时间改变而改变
        CLOCK_MONOTONIC: 从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
        CLOCK_PROCESS_CPUTIME_ID: 本进程到当前代码系统CPU花费的时间
        CLOCK_THREAD_CPUTIME_ID: 本线程到当前代码系统CPU花费的时间
    struct timespec {
        time_t tv_sec; // 秒
        long tv_nsec;  // 纳秒
    };

*/
namespace feng {

bool feng::Condition::waitForSeconds(int seconds) {
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_sec += static_cast<time_t>(seconds);
    return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
}    

}
