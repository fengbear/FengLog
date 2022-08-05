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
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(const std::string &name = "root", LogAppender::ptr appender = nullptr) 
        : name_(name),
          mutex_(new MutexLock),
          level_(LogLevel::DEBUG) {
            formatter_.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%T[%p]%T[%c]%T%f:%l%T%m%n"));
            if (appender) {
                if (!appender->getFormatter()) {
                    appender->setFormatter(formatter_);
                }
                appenders_.push_back(appender);
            }
    } 

    virtual ~Logger() {}
    virtual void log(LogEvent::ptr event) = 0;
    virtual void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    void clearAppenders();

    LogLevel::Level getLevel() const { return level_; }
    void setLevel(LogLevel::Level val){ level_ = val; }
    const std::string& getName() const { return name_; }

    void setFormatter(LogFormatter::ptr val);
    void setFormatter(const std::string &val);
    LogFormatter::ptr getFormatter();

protected:
    std::string name_;                      // 日志名称
    std::unique_ptr<MutexLock> mutex_;
    LogLevel::Level level_;                 // 日志级别
    std::list<LogAppender::ptr> appenders_; // 输出地集合
    LogFormatter::ptr formatter_;           // 格式器
};


// 同步日志器
class SyncLogger : public Logger {
public:
    SyncLogger(const std::string &name, SyncLogAppender::ptr appender = nullptr);
    void log(LogEvent::ptr event) override;
protected:
    StdoutSyncLogAppender::ptr defaultAppender_;
};


// 异步日志器

} // log
} // feng
#endif