#ifndef __FENG_LOG_LOGGER_H__
#define __FENG_LOG_LOGGER_H__

#include "base/Mutex.h"
#include "log/LogEvent.h"
#include "log/LogAppender.h"
#include <memory>
#include <list>
#include <iostream>

namespace feng {
namespace log {

// 日志器
class Logger : public std::enable_shared_from_this<Logger> {



private:
    std::unique_ptr<MutexLock> mutex_;
    std::string name_;                      // 日志名称
    LogLevel::Level level_;                 // 日志级别
    std::list<LogAppender::ptr> appenders_; // 输出地集合
    LogFormatter::ptr formatter_;           // 格式器
};

} // log
} // feng
#endif