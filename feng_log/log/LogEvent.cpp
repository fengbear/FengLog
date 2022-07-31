#include "log/LogEvent.h"
#include "stdarg.h"

namespace feng {
namespace log
{

    const char* LogLevel::toString(LogLevel::Level level) {
        switch (level)
        {
    #define XX(name) \ 
        case LogLevel::name: \
            return #name; \
            break;
        XX(DEBUG);
        XX(INFO);
        XX(WARN);
        XX(ERROR);
        XX(FATAL);
    #undef XX
        default:
            return "UNKNOW";
            break;
        }
        return "UNKNOW";
    }

    LogLevel::Level LogLevel::FromString(const std::string &str) {
    #define XX(level, v) \
        if (str == #v) { \
            return LogLevel::level; \
        }
        XX(DEBUG, debug);
        XX(INFO, info);
        XX(WARN, warn);
        XX(ERROR, error);
        XX(FATAL, fatal);

        XX(DEBUG, DEBUG);
        XX(INFO, INFO);
        XX(WARN, WARN);
        XX(ERROR, ERROR);
        XX(FATAL, FATAL);
    #undef XX
        return LogLevel::UNKNOW;
    }

    LogEvent::LogEvent(const std::string& loggerName,
             LogLevel::Level level,
             const char *file,
             int32_t line,
             uint32_t elapse,
             uint32_t thread_id,
             uint64_t time)
        : loggerName_(loggerName),
          file_(file),
          line_(line),
          elapse_(elapse),
          threadId_(thread_id),
          time_(time),
          level_(level)
    { 
    }

    void LogEvent::format(const char* fmt, ...) {
        va_list al;
        va_start(al, fmt);
        format(fmt, al);
        va_end(al);
    }

    void LogEvent::format(const char* fmt, va_list al) {
        char *buf = nullptr;
        int len = vasprintf(&buf, fmt, al);
        if (len != -1) {
            ss_.append(buf, len);
            free(buf);
        }
    }
    
} // namespace log
} // feng