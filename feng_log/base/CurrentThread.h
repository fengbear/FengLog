#ifndef __FENG_BASE_CURRENTTHREAD_H__
#define __FENG_BASE_CURRENTTHREAD_H__

#include <stdint.h>

namespace feng {

namespace CurrentThread {
// internal
extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char* t_threadName;
void cacheTid();
inline int tid() {
  if (__builtin_expect(t_cachedTid == 0, 0)) {
    cacheTid();
  }
  return t_cachedTid;
}

} // namespace CurrentThread
} // namespace feng

#endif