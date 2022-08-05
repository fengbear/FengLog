#ifndef __FENG_LOG_LOGFILE_H__
#define __FENG_LOG_LOGFILE_H__

#include <memory>
#include <string>
#include "base/noncopyable.h"
#include "base/Mutex.h"

namespace feng {
namespace log {

class AppendFile : noncopyable {
public:
    AppendFile(const char* filename);

    ~AppendFile();

    void append(const char* logline, size_t len);

    void flush();

    off_t writtenBytes() const { return writtenBytes_; }

private:

    // 向文件中写入字节
    size_t write(const char* logline, size_t len);
    // 维护的文件指针
    FILE* fp_;
    // 缓冲区大小为64K 
    char buffer_[64*1024]; 
    // 标识当前文件一共写入了多少字节的数据,
    // 如果超过了rollsize, LogFile就会进行rollFile, 创建新的日志文件,
    // 而这个文件就不会再写入了
    // off_t 指示文件的偏移量 一般为long int类型
    off_t writtenBytes_; 
};


class LogFile : noncopyable
{
public:
    typedef std::shared_ptr<LogFile> ptr;
    LogFile(const std::string& basename,
            off_t rollSize_,
            bool threadSafe = true,
            int flushInterval = 3,
            int checkEveryN = 1024);
    ~LogFile();

    void append(const char* logline, int len);
    void flush();
    bool rollFile();

private:
    void append_unlocked(const char* logline, int len); // 不加锁的append方式

    static std::string getLogFileName(const std::string& basename, time_t* now); // 获取日志文件的名称

    const std::string basename_; // 文件名
    const off_t rollSize_;       // 日志file_缓冲区数据如果超过rollSize_ 就要用新的文件写日志
    const int flushInterval_;    // 日志写入间隔
    const int checkEveryN_;      // 每调用checkEveryN_次日志写，就滚动一次日志 

    int count_;

    std::unique_ptr<MutexLock> mutex_;
    time_t startOfPeriod_;       // 开始的周期，在同一个周期内的日志数据会写入同一个日志文件中（如果数据不是太大的话）
    time_t lastRoll_;            // 上一次滚动日志的时间 
    time_t lastFlush_;           // 上一次日志写入文件的时间
    std::unique_ptr<AppendFile> file_; // 文件智能指针

    const static int kRollPerSeconds_ = 60*60*24; // 写新日志文件的周期 即一天24小时 换算为秒

};

} // namespace log
} // namespace feng


#endif