#ifndef __FENG_BASE_BUFFER_H__
#define __FENG_BASE_BUFFER_H__

#include <string>
#include <vector>
#include <atomic>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>
#include <assert.h>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


namespace feng {

/// muduo Buffer
///
/// @code
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode

/// feng Buffer
///
/// @code
/// +------------------+------------------+
/// |  readable bytes  |  writable bytes  |
/// |     (CONTENT)    |                  |
/// +------------------+------------------+
/// |                  |                  |
/// readerIndex(0) <=  writerIndex  <=    size
/// @endcode

class Buffer {

public:
    static const size_t kInitialSize = 1024; // writeable初始大小

    Buffer(size_t initialSize = kInitialSize);
    ~Buffer() = default;

    // 可写字节数
    size_t writableBytes() const;

    // 可读字节数
    size_t readableBytes() const;

    // 预留字节数
    size_t prependableBytes() const;

    // 数据可读处
    const char* peek() const;

    void ensureWriteable(size_t len);
    void hasWritten(size_t len);

    void retrieve(size_t len);
    void retrieveUntil(const char* end);
    void retrieveAll();
    std::string retrieveAllToStr();

    const char* beginWriteConst() const;
    char* beginWrite();

    void append(const std::string& str);
    void append(const char* str, size_t len);
    void append(const void* data, size_t len);
    void append(const Buffer& buff);

    ssize_t readFd(int fd, int* saveErrno);
    ssize_t writeFd(int fd, int* saveErrno);

private:
    char* begin();
    const char* begin() const;
    void makeSpace(size_t len);

    std::vector<char> buffer_;
    std::atomic<std::size_t> readerIndex_;
    std::atomic<std::size_t> writerIndex_;

};

}

#endif