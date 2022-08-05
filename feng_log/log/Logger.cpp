#include "log/Logger.h"
#include <iostream>

namespace feng {
namespace log {

void Logger::setFormatter(LogFormatter::ptr val) {
    MutexLockGuard lock(*mutex_);
    formatter_ = val;
    for (auto &i : appenders_) {
        if (!i->hasFormatter()) {
            i->setFormatter(formatter_);
        }
    }
}

void Logger::setFormatter(const std::string &val) {
    LogFormatter::ptr new_val(new LogFormatter(val));
    if (new_val->isError()) {
        std::cout << "Logger setFormatter name=" << name_
                  << " value=" << val << " invalid formatter"
                  << std::endl;
        return;
    }
    setFormatter(new_val);
}

LogFormatter::ptr Logger::getFormatter() {
    MutexLockGuard lock(*mutex_);
    return formatter_;
}

void Logger::addAppender(LogAppender::ptr appender) {
    MutexLockGuard lock(*mutex_);
    if (!appender->getFormatter()) {
        appender->setFormatter(formatter_);
    }
    appenders_.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    MutexLockGuard lock(*mutex_);
    for (auto it = appenders_.begin(); it != appenders_.end(); it++) {
        if (*it == appender) {
            appenders_.erase(it);
            break;
        }
    }
}

void Logger::clearAppenders() {
    MutexLockGuard lock(*mutex_);
    appenders_.clear();
}

/*********************************************同步日志器*******************************************/

SyncLogger::SyncLogger(const std::string& name, SyncLogAppender::ptr appender) 
    :Logger(name, appender)
{
    defaultAppender_ = std::make_shared<StdoutSyncLogAppender>();
    defaultAppender_->setFormatter(formatter_);
}

void SyncLogger::log(LogEvent::ptr event) {
    auto level = event->getLevel();
    if (level >= level_) {
        if (!appenders_.empty()) {
            for (auto &i : appenders_) {
                i->append(*event);
            }
        } else if (defaultAppender_) {
            defaultAppender_->append(*event);
        }
    }
}

}
}