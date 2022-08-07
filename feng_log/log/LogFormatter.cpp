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

// %xxx %xxx{xxx} %%
// %p 输出等级
// %d 输出日志时间点的日期或时间 比如：%d{yyy MMM dd HH:mm:ss , SSS}
// 默认格式 
// %d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n
void LogFormatter::init() {
    // str format type
    std::vector<std::tuple<std::string, std::string, int>> vec;
    // 存[ ]
    std::string nstr;
    for (size_t i = 0; i < m_pattern.size(); i++) {
        if (m_pattern[i] != '%') {
            nstr.append(1, m_pattern[i]);
            continue;
        }
        // %%->%
        if (i + 1 < m_pattern.size()) {
            if (m_pattern[i + 1] == '%') {
                nstr.append(1, '%');
                continue;
            }
        }

        size_t n = i + 1;
        int fmt_status = 0;
        size_t fmt_begin = 0;

        std::string str; // % 到 { 之间的字符串
        std::string fmt; // { 到 } 之间的字符串

        while (n < m_pattern.size()) {
            // 
            if (!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{' && m_pattern[n] != '}')) {
                str = m_pattern.substr(i + 1, n - i - 1);
                break;
            }
            if (fmt_status == 0) {
                if (m_pattern[n] == '{') {
                    str = m_pattern.substr(i+1, n-i-1);
                    fmt_status = 1;
                    fmt_begin = n;
                    ++n;
                    continue;
                }
            } else if (fmt_status == 1) {
                if (m_pattern[n] == '}') {
                    fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                    fmt_status = 0;
                    ++n;
                    break;
                }
            }
            ++n;
            if (n == m_pattern.size()) {
                if (str.empty()) {
                    str = m_pattern.substr(i+1);
                }
            }
        }

        if (fmt_status == 0) {
            // [ ]
            if (!nstr.empty()) {
                vec.push_back(std::make_tuple(nstr, std::string(), 0));
                nstr.clear();
            }
            // 有格式字符串
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n - 1;
        }
        // 未转到状态0 解析出错
        else if (fmt_status == 1) {
            std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
            m_error = true;
            vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));            
        }
    }
    if (!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, std::string(), 0));
    }

    // %m -- 消息体
    // %p -- level
    // %r -- 启动后的时间
    // %c -- 日志名称
    // %t -- 线程id
    // %n -- 回车换行
    // %d -- 时间
    // %f -- 文件名
    // %l -- 行号
    // %T -- tab

    // 建立格式符到相应的格式处理器的映射
    static std::map<std::string, std::function<FormatItem::ptr(const std::string &str)> > s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string &fmt){return FormatItem::ptr(new C(fmt));}}
        XX(m, MessageFormatItem),           //m:消息
        XX(p, LevelFormatItem),             //p:日志级别
        XX(r, ElapseFormatItem),            //r:累计毫秒数
        XX(c, NameFormatItem),              //c:日志名称
        XX(t, ThreadIdFormatItem),          //t:线程id
        XX(n, NewLineFormatItem),           //n:换行
        XX(d, DateTimeFormatItem),          //d:时间
        XX(f, FilenameFormatItem),          //f:文件名
        XX(l, LineFormatItem),              //l:行号
        XX(T, TabFormatItem),               //T:Tab
        // XX(F, FiberIdFormatItem),           //F:协程id
        // XX(N, ThreadNameFormatItem),        //N:线程名称
#undef XX
    };

    for (auto &i : vec) {
        // [ ] <<pattern_error>>
        if (std::get<2>(i) == 0) {
            m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
        } else {
            auto it = s_format_items.find(std::get<0>(i));
            // 找不到报错
            if (it == s_format_items.end()) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                m_error = true;
            } else {
                m_items.push_back(FormatItem::ptr(it->second(std::get<1>(i))));
            }
        }
    }
}


void LogFormatter::format(LogStream& os, const LogEvent& event) {
    for (auto& m_item : m_items) {
        m_item->format(os, event);
    }
}

} // feng
} // log