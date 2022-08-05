#include "log/LogAppender.h"
#include <iostream>
#include "assert.h"


namespace feng {
namespace log {

void LogAppender::setFormatter(LogFormatter::ptr val) {
    MutexLockGuard lock(*mutex_);
    m_formatter = val;
}

LogFormatter::ptr LogAppender::getFormatter() {
    MutexLockGuard lock(*mutex_);
    return m_formatter;
}

// ********************************************同步******************************************** //

void StdoutSyncLogAppender::append(const LogEvent &event) {
    auto level = event.getLevel();
    if (level >= m_level) {
        MutexLockGuard lock(*mutex_);
        LogStream stream;
        m_formatter->format(stream, event);
        std::cout << stream.buffer().data();
    }
}

FileSyncLogAppender::FileSyncLogAppender(const std::string &filepath, off_t rollSize)
    : filepath_(filepath),
    rollSize_(rollSize)
{
    file_ = std::make_unique<LogFile>(filepath_, rollSize_, false);
}

void FileSyncLogAppender::append(const LogEvent& event) {
    auto level = event.getLevel();
    if (level >= m_level) {
        MutexLockGuard lock(*mutex_);
        LogStream stream;
        m_formatter->format(stream, event);
        auto &buffer = stream.buffer();
        file_->append(buffer.data(), buffer.length());
    }
}
















}
}