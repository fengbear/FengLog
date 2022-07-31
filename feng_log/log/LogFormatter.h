#ifndef __FENG_LOG_LOGFORMATTER_H__
#define __FENG_LOG_LOGFORMATTER_H__


#include <string>
#include <vector>
#include <ostream>
#include <memory>
#include <chrono>
#include "log/LogStream.h"
#include "log/LogEvent.h"
#include "base/TimeStamp.h"

namespace feng {
namespace log {

// 日志格式器
class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    
    LogFormatter(const std::string &pattern);
    void format(LogStream&, const LogEvent& event);

    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;

        virtual ~FormatItem() { }
        virtual void format(LogStream& os, const LogEvent& event) = 0;
    };

    void init();

    bool isError() const { return m_error; }
    const std::string getPattern() const { return m_pattern; }

private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
    bool m_error = false;
};

class MessageFormatItem : public LogFormatter::FormatItem {
public:
    MessageFormatItem(const std::string &fmt = ""){ }
    void format(LogStream& os, const LogEvent& event) {
        os << event.getContent();
    }
};

class LevelFormatItem : public LogFormatter::FormatItem {
public:
    LevelFormatItem(const std::string &fmt = ""){ }
    void format(LogStream& os, const LogEvent& event) {
        os << LogLevel::toString(event.getLevel());
    }
};

class ElapseFormatItem : public LogFormatter::FormatItem {
public:
    ElapseFormatItem(const std::string &fmt = ""){ }
    void format(LogStream& os, const LogEvent& event) {
        os << event.getElapse();
    }
};

class NameFormatItem : public LogFormatter::FormatItem {
public:
    NameFormatItem(const std::string &fmt = ""){ }
    void format(LogStream& os, const LogEvent& event) {
        os << event.getLoggerName();
    }
};


class ThreadIdFormatItem : public LogFormatter::FormatItem {
public:
    ThreadIdFormatItem(const std::string &fmt = ""){ }
    void format(LogStream& os, const LogEvent& event) {
        os << event.getThreadId();
    }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
public:
    ThreadNameFormatItem(const std::string &fmt = ""){ }
    void format(LogStream& os, const LogEvent& event) {
        os << event.getThreadName();
    }
};

class LineFormatItem : public LogFormatter::FormatItem {
public:
    LineFormatItem(const std::string &fmt = ""){ }
    void format(LogStream& os, const LogEvent& event) {
        os << event.getLine();
    }
};

class NewLineFormatItem : public LogFormatter::FormatItem {
public:
    NewLineFormatItem(const std::string &fmt = ""){ }
    void format(LogStream& os, const LogEvent& event) {
        os << '\n';
    }
};

class TabFormatItem : public LogFormatter::FormatItem {
public:
    TabFormatItem(const std::string &fmt = ""){ }
    void format(LogStream& os, const LogEvent& event) {
        os << '\t';
    }
};

class StringFormatItem : public LogFormatter::FormatItem {
public:
    StringFormatItem(const std::string &str) : m_string(str){ }
    void format(LogStream& os, const LogEvent& event) {
        os << m_string;
    }
private:
    std::string m_string;
};

class FilenameFormatItem : public LogFormatter::FormatItem {
public:
    FilenameFormatItem(const std::string &fmt = ""){ }
    void format(LogStream& os, const LogEvent& event) {
        auto &f = event.getFile();
        os.append(f.data_, f.size_);
    }
};

class DateTimeFormatItem : public LogFormatter::FormatItem {
public:
    DateTimeFormatItem(const std::string &fmt = "%Y-%m-%d %H:%M:%S") : m_format(fmt) {
        if (m_format.empty()) {
            m_format = "%Y-%m-%d %H:%M:%S";
        }
    }

    void format(LogStream& os, const LogEvent& event) {
        struct tm tm;
        time_t time = event.getTime();
        localtime_r(&time, &tm);
        char buf[64];
        size_t n = strftime(buf, sizeof(buf), m_format.c_str(), &tm);
        os.append(buf, n);
    }

private:
    std::string m_format;
};


















} // log
} // feng

#endif