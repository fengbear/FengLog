#include "base/util.h"
#include <algorithm>
#include <assert.h>
#include <sys/types.h>

namespace feng {

static thread_local const pid_t PID = gettid();
pid_t GetThreadId(){
    return PID;
}

}