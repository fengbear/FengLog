#ifndef __FENG_LOG_EVENT_H_
#define __FENG_LOG_EVENT_H_

#include <string>
#include <memory>
#include <chrono>
#include <string.h>
#include "log/LogStream.h"
#include "base/Thread.h"

namespace feng {
namespace log {

/* 取文件名
    * 例如feng_log/log/LogEvent.h只取
    * LogEvent.h
*/ 

class SourceFile
{
public:
    template<int N>
    SourceFile(const char (&arr)[N])
        : data_(arr),
          size_(N-1)
    {
        // 在参数 str 所指向的字符串中搜索最后一次出现字符 c（一个无符号字符）的位置
        // 返回 str 中最后一次出现字符 c 的位置。如果未找到该值，则函数返回一个空指针。
        const char* slash = strrchr(data_, '/');
        if (slash) {
            data_ = slash + 1;
            size_ -= static_cast<int>(data_ - arr);
        }
    }

    explicit SourceFile(const char* filename) 
        : data_(filename)
    {
        const char* slash = strrchr(filename, '/');
        if (slash) {
            data_ = slash + 1;
        }
        size_ = static_cast<int>(strlen(data_));
    }
    const char* data_;
    int size_;
};


// 日志级别
class LogLevel {
public:
    enum Level{
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };

    static const char* toString(LogLevel::Level level);
    static LogLevel::Level FromString(const std::string &str);  
};


// 日志事件
class LogEvent {
public:
    // 表示时间的一个点
    using TimePoint = std::chrono::_V2::system_clock::time_point; 
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(const std::string& loggerName,
             LogLevel::Level level,
             const char *file,
             int32_t line,
             uint32_t elapse,
             uint32_t thread_id,
             uint64_t time);

    const std::string& getLoggerName() const { return loggerName_; }
    const SourceFile& getFile() const { return file_; }
    int32_t getLine() const { return line_; }
    uint32_t getElapse() const { return elapse_; }
    uint32_t getThreadId() const { return threadId_; }

    uint64_t getTime() const { return time_; }
    const std::string& getThreadName() const { return feng::Thread::getCurrentThreadName(); }
    const LogStream::Buffer& getContent() const { return ss_.buffer(); }
    LogLevel::Level getLevel() const { return level_; }

    LogStream& getSS() { return ss_; }
    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);


private:
    std::string loggerName_;          // 日志器名称
    SourceFile file_;                 // 文件名
    int32_t line_ = 0;                // 行号
    uint32_t elapse_ = 0;             // 启动到现在的毫秒数
    uint32_t threadId_ = 0;           // 线程ID 
    uint64_t time_;                  // 时间戳
    LogStream ss_;                    // 事件内容
    LogLevel::Level level_;

};



} // log
} // feng


#endif