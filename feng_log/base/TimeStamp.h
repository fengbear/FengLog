#ifndef __FENG_TIMESTAMP_H__
#define __FENG_TIEMSTAMP_H__

#include <limits>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <chrono>
#include <iomanip>

namespace feng {

class TimeStamp {
public:
    static const int MicroSecondsPerSecond = 1000 * 1000;
    static const int MicroSecondsPerMillSecond = 1000;

    explicit TimeStamp(uint64_t microsecond = 0)
        : microSecond_(microsecond)
    {
    }
    // 1s = 1000ms = 1000 000 us == 1000 000 000 ns
    // s
    uint64_t second() const noexcept { return microSecond_ / MicroSecondsPerSecond; }
    // ms
    uint64_t millsecond() const noexcept { return microSecond_ / MicroSecondsPerMillSecond; }
    // us
    uint64_t microsecond() const noexcept { return microSecond_; }
    // ns
    uint64_t nansecond() const noexcept { return microSecond_ * 1000; }

    TimeStamp &operator+(const TimeStamp &t) noexcept {
        microSecond_ += t.microSecond_;
        return *this;
    }

    TimeStamp &operator-(const TimeStamp &t) noexcept {
        microSecond_ -= t.microSecond_;
        return *this;
    }

    TimeStamp &operator+(int second) noexcept {
        microSecond_ += (second * 1000000);
        return *this;
    }

    TimeStamp &operator-(int second) noexcept {
        microSecond_ -= (second * 1000000);
        return *this;
    }

    bool operator<(const TimeStamp &t) const noexcept {
        return microSecond_ < t.microSecond_;
    }

    bool operator<=(const TimeStamp &t) const noexcept {
        return microSecond_ <= t.microSecond_;
    }

    bool operator>(const TimeStamp &t) const noexcept {
        return microSecond_ > t.microSecond_;
    }

    bool operator==(const TimeStamp &t) const noexcept {
        return microSecond_ == t.microSecond_;
    }

    static TimeStamp now() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        uint64_t microsecond = tv.tv_sec * 1000000 + tv.tv_usec;
        return TimeStamp(microsecond);
    }

    static TimeStamp second(int sec) { return TimeStamp(sec * 1000000); }
    static TimeStamp millsecond(int msec) { return TimeStamp(msec * 1000); }
    static TimeStamp microsecond(int microsec) { return TimeStamp(microsec); }
    static TimeStamp nanosecond(int nsec) { return TimeStamp(nsec / 1000); }

    static TimeStamp nowSecond(int sec) { return TimeStamp::now() + second(sec); }
    static TimeStamp nowMsecond(int msec) {
        return TimeStamp::now() + millsecond(msec);
    }
    static TimeStamp nowNanosecond(int nsec) {
        return TimeStamp::now() + nanosecond(nsec);
    }

    // 返回 编译器允许的uint64_t型数最大值
    static TimeStamp max() noexcept {
        return TimeStamp(std::numeric_limits<uint64_t>::max());
    }

    static std::string getDate() {
        char buf[64] = {0};
        time_t t = time(NULL);
        // 星期几, 一个月中的第几天, 缩写的月份名称, 年份, 小时, 分, 秒
        // Sun, 19 MAR 2022 12:55:02
        strftime(buf, 64, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&t));
        return buf;
    }

    std::string toFormattedString(bool showMicroseconds = true) const {
        char buf[64] = {0};
        time_t seconds = static_cast<time_t>(second());
        struct tm tm_time;
        gmtime_r(&seconds, &tm_time);

        if (showMicroseconds) {
            int microseconds = static_cast<int>(microsecond());
            snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
                    tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                    tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
                    microseconds);
        } else {
            snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
                    tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                    tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        }
        return buf;
    }

private:
    // 微秒      
    uint64_t microSecond_;
};
} // feng

#endif
