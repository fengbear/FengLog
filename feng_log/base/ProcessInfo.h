#ifndef __FENG_PROCESSINFO_H__
#define __FENG_PROCESSINFO_H__

#include <string>
#include <vector>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include "base/TimeStamp.h"

namespace feng {
namespace ProcessInfo
{
    pid_t pid();
    std::string hostname();
} // namespace ProcessInfo
} // feng

#endif