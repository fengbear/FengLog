#include "base/ProcessInfo.h"
namespace feng{
namespace ProcessInfo{

pid_t pid()
{
  return ::getpid();
}

std::string hostname()
{
  // HOST_NAME_MAX 64
  // _POSIX_HOST_NAME_MAX 255
  char buf[256];
  if (::gethostname(buf, sizeof buf) == 0)
  {
    buf[sizeof(buf)-1] = '\0';
    return buf;
  }
  else
  {
    return "unknownhost";
  }
}

} // ProcessInfo
} // feng