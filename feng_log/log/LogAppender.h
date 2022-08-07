#ifndef __FENG_LOGAPPENDER_H__
#define __FENG_LOGAPPENDER_H__

#include <fstream>
#include <functional>
#include <atomic>
#include <iostream>
#include "log/LogFile.h"
#include "log/LogStream.h"
#include "log/LogFormatter.h"
#include "base/Mutex.h"
#include "base/Condition.h"

namespace feng{
namespace log{

// 日志输出地
class LogAppender {
friend class Logger;    
public:
    typedef std::shared_ptr<LogAppender> ptr;
    LogAppender() : mutex_(new MutexLock) {}  
    virtual ~LogAppender() { }

    virtual void append(const LogEvent& event) = 0;
    virtual void flush() = 0;

    LogFormatter::ptr getFormatter();
    void setFormatter(LogFormatter::ptr val);

    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level val) { m_level = val; }

    bool hasFormatter() { return m_formatter != nullptr; }

    virtual void setNotifyFunc(const std::function<void()> &notify_func) = 0;
    virtual bool empty() = 0;

protected:
    std::unique_ptr<MutexLock> mutex_;
    LogLevel::Level m_level = LogLevel::DEBUG;
    LogFormatter::ptr m_formatter;
};

/***************************************   同步 ********************************************/

// 同步Appender
class SyncLogAppender : public LogAppender {
public:
    virtual ~SyncLogAppender() {}
    void flush() {}
    void setNotifyFunc(const std::function<void()> &notify_func) override {}
    bool empty() { return true; }
};

// 输出到控制台的同步Appender
class StdoutSyncLogAppender : public SyncLogAppender {
public:
    typedef std::shared_ptr<StdoutSyncLogAppender> ptr;
    void append(const LogEvent& event) override;

};

// 输出到文件的同步Appender
class FileSyncLogAppender : public SyncLogAppender {
public:
    typedef std::shared_ptr<FileSyncLogAppender> ptr;
    FileSyncLogAppender(const std::string &filepath,
                        off_t rollSize);
    void append(const LogEvent& event) override;
private:
    std::string filepath_;
    const off_t rollSize_;
    std::unique_ptr<LogFile> file_;
    uint64_t lastTime_;
};

/***************************************  异步 ********************************************/

class AsyncLogAppender : public LogAppender {
public:
    AsyncLogAppender();
    virtual ~AsyncLogAppender() { }
    void setNotifyFunc(const std::function<void()> &notify_func) { notify_func_ = std::move(notify_func); }

    bool empty() {
        MutexLockGuard lock(*mutex_);
        return buffer_.empty();
    }

protected:
    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;

    // 输入缓冲区
    BufferPtr currentBuffer_;  // 当前缓冲区
    BufferPtr nextBuffer_;     // 预备缓冲区
    BufferVector buffer_;      // 前端缓冲区队列 待写入文件的已填满的缓冲

    // 输出缓冲区
    BufferPtr newBuffer1;
    BufferPtr newBuffer2;
    BufferVector buffersToWrite;

    // 唤醒日志线程
    std::function<void()> notify_func_;
};

// 输出到文件的异步Appender
class FileAsyncLogAppender : public AsyncLogAppender {
public:
    typedef std::shared_ptr<FileAsyncLogAppender> ptr;

    FileAsyncLogAppender(const std::string &filepath,
                        off_t rollSize);
    ~FileAsyncLogAppender(){ 
        file_->flush(); 
    }
    void append(const LogEvent& event) override;
    void flush() override;
    
private:
    std::string filepath_;
    const off_t rollSize_;
    std::unique_ptr<LogFile> file_;
    uint64_t lastTime_;
};

} // log
} // feng


#endif