#ifndef __FENG_LOG_LOGGERMANAGER__ 
#define __FENG_LOG_LOGGERMANAGER__

#include "log/Logger.h"
#include "base/Mutex.h"
#include "base/util.h"
#include <map>

#define FENG_LOG_LEVEL(logger, level) \
    if (logger->getLevel() <= level) \
    feng::log::LogWrapper(logger, feng::log::LogEvent::ptr(new feng::log::LogEvent(logger->getName(), level, \
                          __FILE__, __LINE__, 0, feng::GetThreadId(), \
                          time(0)))).getSS()

#define FENG_LOG_DEBUG(logger) FENG_LOG_LEVEL(logger, feng::log::LogLevel::Level::DEBUG)
#define FENG_LOG_INFO(logger) FENG_LOG_LEVEL(logger, feng::log::LogLevel::Level::INFO)
#define FENG_LOG_WARN(logger) FENG_LOG_LEVEL(logger, feng::log::LogLevel::Level::WARN)
#define FENG_LOG_ERROR(logger) FENG_LOG_LEVEL(logger, feng::log::LogLevel::Level::ERROR)
#define FENG_LOG_FATAL(logger) FENG_LOG_LEVEL(logger, feng::log::LogLevel::Level::FATAL)

#define FENG_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if (logger->getLevel() <= level) \
    feng::log::LogWrapper(logger, feng::log::LogEvent::ptr(new feng::log::LogEvent(logger->getName(), level, \
                          __FILE__, __LINE__, 0, feng::GetThreadId(), \
                          time(0)))).getLogEvent()->format(fmt, __VA_ARGS__)


#define FENG_LOG_FMT_DEBUG(logger, fmt, ...) FENG_LOG_FMT_LEVEL(logger, feng::log::LogLevel::Level::DEBUG, fmt, __VA_ARGS__)
#define FENG_LOG_FMT_INFO(logger, fmt, ...) FENG_LOG_FMT_LEVEL(logger, feng::log::LogLevel::Level::INFO, fmt, __VA_ARGS__)
#define FENG_LOG_FMT_WARN(logger, fmt, ...) FENG_LOG_FMT_LEVEL(logger, feng::log::LogLevel::Level::WARN, fmt, __VA_ARGS__)
#define FENG_LOG_FMT_ERROR(logger, fmt, ...) FENG_LOG_FMT_LEVEL(logger, feng::log::LogLevel::Level::ERROR, fmt, __VA_ARGS__)
#define FENG_LOG_FMT_FATAL(logger, fmt, ...) FENG_LOG_FMT_LEVEL(logger, feng::log::LogLevel::Level::FATAL, fmt, __VA_ARGS__)


#define FENG_LOG_SYNC_STDOUT_ROOT() feng::log::LoggerManager::getInstance().getSyncStdoutRoot()
#define FENG_LOG_SYNC_FILE_ROOT() feng::log::LoggerManager::getInstance().getSyncFileRoot()
#define FENG_LOG_ASYNC_FILE_ROOT() feng::log::LoggerManager::getInstance().getAsynFileRoot()


namespace feng {
namespace log {

static const std::string default_sync_log_file = "feng_sync";
static const std::string default_async_log_file = "feng_async";
static const int default_roll_size = 50 * 1000 * 1000;
static const int default_flush_interval = 3;

class LogWrapper {
public:
    LogWrapper(Logger::ptr logger, LogEvent::ptr event)
        : logger_(logger),
          event_(event)
    { }

    ~LogWrapper() {
        logger_->log(event_);
    }
    LogEvent::ptr getLogEvent() const { return event_; }
    LogStream& getSS() { return event_->getSS(); }

private:
    Logger::ptr logger_;
    LogEvent::ptr event_;
};

class LoggerManager : public noncopyable {
public:
    static LoggerManager& getInstance() {
        static LoggerManager manager;
        return manager;
    }

    Logger::ptr getLogger(const std::string &name);

    // 默认同步日志器 写入到标准输出
    static Logger::ptr getSyncStdoutRoot() {
        static std::shared_ptr<Logger> sync_root = std::make_shared<SyncLogger>(
            "sync_stdout_root",
            std::make_shared<StdoutSyncLogAppender>()
        );
        return sync_root;
    }

    // 默认同步日志器 写入到文件
    static Logger::ptr getSyncFileRoot() {
        static std::shared_ptr<Logger> sync_root = std::make_shared<SyncLogger>(
            "sync_file_root",
            std::make_shared<FileSyncLogAppender>(default_sync_log_file, default_roll_size)
        );
        return sync_root;
    }
    
    // 异步日志器 写入文件
    static Logger::ptr getAsynFileRoot() {
        static std::shared_ptr<Logger> async_root = std::make_shared<AsyncLogger>(
            "async_file_root",
            default_flush_interval,
            std::make_shared<FileAsyncLogAppender>(default_async_log_file, default_roll_size)
        );
        return async_root;
    }
private:
    MutexLock mutex_;
    std::map<std::string, Logger::ptr> loggers_;
};

}
}

#endif