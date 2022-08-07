#include "log/LogFile.h"
#include <string.h>
#include <assert.h>
#include "base/ProcessInfo.h"
namespace feng {
namespace log {

thread_local char t_errnobuf[512];
thread_local char t_time[64];
thread_local time_t t_lastSecond;

const char* strerror_tl(int savedErrno)
{
    return strerror_r(savedErrno, t_errnobuf, sizeof(t_errnobuf));
}


/*********************************AppendFile*********************************/

AppendFile::AppendFile(const char* filename)
    : fp_(::fopen(filename, "ae")), // 'e' for O_CLOEXEC
    writtenBytes_(0)
{
    assert(fp_);
    // 设置文件流的缓冲区
    // 本地缓冲区是64*1024，每次写入1024，64次写入才会刷新到文件里。
    ::setbuffer(fp_, buffer_, sizeof(buffer_));
}

AppendFile::~AppendFile()
{
    ::fclose(fp_);
}

void AppendFile::append(const char* logline, const size_t len) 
{
    size_t written = 0;
    
    while (written != len)
    {
        size_t remain = len - written;
        // 调用私有成员函数write
        size_t n = write(logline + written, remain);
        if (n != remain) {
            int err = ferror(fp_);
            if (err) {
                fprintf(stderr, "AppendFile::append() failed %s\n", strerror_tl(err));
                break;
            }
        }
        written += n;
    }

    writtenBytes_ += written;
}

void AppendFile::flush() {
    ::fflush(fp_); // 从缓冲区刷到文件中
}

size_t AppendFile::write(const char* logline, size_t len)
{
    // fwrite的线程不安全版 写入文件 1为1个字节 len为len个
    return ::fwrite_unlocked(logline, 1, len, fp_);
}


/*********************************LogFile*********************************/

LogFile::LogFile(const std::string& basename,
                 off_t rollSize,
                 bool threadSafe,
                 int flushInterval,
                 int checkEveryN)
    : basename_(basename),
      rollSize_(rollSize),
      flushInterval_(flushInterval),
      checkEveryN_(checkEveryN),
      count_(0),
      mutex_(new MutexLock),
      startOfPeriod_(0),
      lastRoll_(0),
      lastFlush_(0)
{
    assert(basename.find('/') == std::string::npos);  // 断言basename不包含'/'
    rollFile();
}

LogFile::~LogFile() = default;



void LogFile::append(const char* logline, int len)
{
    if (mutex_) {
        MutexLockGuard lock(*mutex_);
        append_unlocked(logline, len);
    } else {
        append_unlocked(logline, len);
    }
}

void LogFile::flush()
{
    if (mutex_) {
        MutexLockGuard lock(*mutex_);
        file_->flush();
    } else {
        file_->flush();
    }
}

void LogFile::append_unlocked(const char* logline, int len) {
    // 调用Appendfile智能指针将内容写到文件的缓冲区
    file_->append(logline, len);
    if (file_->writtenBytes() > rollSize_) {
        rollFile();
    } else {
        ++count_;
        // 连续写入次数超过checkEveryN_ 
        if (count_ >= checkEveryN_) { 
            count_ = 0;
            time_t now = ::time(NULL);
            // 注意，这里先除KRollPerSeconds然后乘KPollPerSeconds表示对齐值KRollPerSeconds的整数倍，
            // 也就是事件调整到当天零点(/除法会引发取整) 
            time_t thisPeriod = now / kRollPerSeconds_ * kRollPerSeconds_;
            if (thisPeriod != startOfPeriod_) {
                rollFile();
            } else if (now - lastFlush_ > flushInterval_) {
                lastFlush_ = now;
                file_->flush();
            }
        }
    }
}

/* 
 * 构造一个日志文件名 
 * 日志名由基本名字+时间戳+主机名+进程id+加上“.log”后缀 
 * logfile_test.20120603-144022.hostname.3605.log
*/  
std::string LogFile::getLogFileName(const std::string& basename, time_t* now) {
    std::string filename;
    filename.reserve(basename.size() + 64);
    filename = basename;

    char timebuf[32];
    struct tm tm;
    *now = time(NULL);
    gmtime_r(now, &tm);
    strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
    filename += timebuf;

    filename += ProcessInfo::hostname();

    char pidbuf[32];
    snprintf(pidbuf, sizeof(pidbuf), ".%d", ProcessInfo::pid());
    filename += pidbuf;

    filename += ".log";

    return filename;
}

bool LogFile::rollFile() {
    time_t now = 0;
    std::string filename = getLogFileName(basename_, &now);
    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;

    if (now > lastRoll_) {
        lastRoll_ = now;
        lastFlush_ = now;
        startOfPeriod_ = start;
        file_.reset(new AppendFile(filename.c_str()));
        return true;
    }
    return false;
}



} // log
} // feng