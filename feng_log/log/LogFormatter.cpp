#include "log/LogFormatter.h"
#include <map>
#include <functional>
#include <iostream>

namespace feng{
namespace log{

LogFormatter::LogFormatter(const std::string& pattern)
    : m_pattern(pattern) {
        init();
    }

void LogFormatter::init() {
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string PTHREAD_RWLOCK_WRITER_NONRECURSIVE_INITIALIZER_N

}


void LogFormatter::format(LogStream& os, const LogEvent& event) {
    for (auto& m_item : m_items) {
        m_item->format(os, event);
    }
}

} // feng
} // log