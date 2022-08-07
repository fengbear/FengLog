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


// ******************************************** 异步 ******************************************** //

AsyncLogAppender::AsyncLogAppender()
    : currentBuffer_(std::make_unique<Buffer>()),
      nextBuffer_(std::make_unique<Buffer>()),
      newBuffer1(std::make_unique<Buffer>()),
      newBuffer2(std::make_unique<Buffer>())
    {
        newBuffer1->bzero();
        newBuffer2->bzero();
        buffersToWrite.reserve(16);
    }

FileAsyncLogAppender::FileAsyncLogAppender(const std::string &filepath,
                                            off_t rollSize)
    : filepath_(filepath),
      rollSize_(rollSize)
    {
        file_ = std::make_unique<LogFile>(filepath_, rollSize_, false);
    }

void FileAsyncLogAppender::append(const LogEvent& event) {
    auto level = event.getLevel();
    if (level >= m_level) {
        LogStream stream;
        const char *logline = nullptr;
        int len = 0;

        m_formatter->format(stream, event);
        auto& buffer = stream.buffer();
        logline = buffer.data();
        len = buffer.length();

        MutexLockGuard lock(*mutex_);
        if (currentBuffer_->avail() > len) {
            currentBuffer_->append(logline, len);
        // 满了就把当前缓冲区推入队列 启动预备缓冲区
        } else {
            buffer_.push_back(std::move(currentBuffer_));
            // 如果没用预备缓冲区 现在就使用预备缓冲区
            if (nextBuffer_) {
                currentBuffer_ = std::move(nextBuffer_);
            // else 说明已经使用了预备缓冲区 那么得开辟新的空间了
            } else {
                currentBuffer_.reset(new Buffer);
            }
            currentBuffer_->append(logline, len);

            // 唤醒日志线程
            if (notify_func_) {
                notify_func_();
            }
        }
    }
}

void FileAsyncLogAppender::flush() {
    assert(newBuffer1 && newBuffer1->length() == 0);
    assert(newBuffer2 && newBuffer2->length() == 0);
    assert(buffersToWrite.empty());
    {
        MutexLockGuard lock(*mutex_);
        buffer_.push_back(std::move(currentBuffer_));
        currentBuffer_ = std::move(newBuffer1);
        buffersToWrite.swap(buffer_);
        if (!nextBuffer_) {
            nextBuffer_ = std::move(newBuffer2);
        }
    }

    assert(!buffersToWrite.empty());

    // 如果写入太多数据
    if (buffersToWrite.size() > 25) {
        // 保留一部分数据 丢弃多的 
        buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
    }

    for (const auto& buffer : buffersToWrite) {
        file_->append(buffer->data(), buffer->length());
    }

    // 读完 保留2个以下缓冲区就行
    if (buffersToWrite.size() > 2) 
    {
        buffersToWrite.resize(2);
    }

    if (!newBuffer1) {
        assert(!buffersToWrite.empty());
        newBuffer1 = std::move(buffersToWrite.back());
        buffersToWrite.pop_back();
        newBuffer1->reset();
    }

    if (!newBuffer2)
    {
        assert(!buffersToWrite.empty());
        newBuffer2 = std::move(buffersToWrite.back());
        buffersToWrite.pop_back();
        newBuffer2->reset(); // 清空缓冲
    }

    buffersToWrite.clear();
    file_->flush();
}

} // namespace log
} // namespace feng