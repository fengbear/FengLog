#ifndef __FENG_LOGAPPENDER_H__
#define __FENG_LOGAPPENDER_H__

#include "log/LogFile.h"
#include "log/LogStream.h"
#include "log/LogFormatter.h"

namespace feng{
namespace log{

// 日志输出地
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;


protected:

};



} // log
} // feng


#endif