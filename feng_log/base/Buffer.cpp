#include "base/Buffer.h"

namespace feng {

Buffer::Buffer(size_t initialSize)
    : buffer_(initialSize),
      readerIndex_(0),
      writerIndex_(0)
    {
        assert(readableBytes() == 0);
        assert(writableBytes() == initialSize);
        assert(prependableBytes() == 0);
    }

size_t Buffer::readableBytes() const {
    return writerIndex_ - readerIndex_;
}

size_t Buffer::writableBytes() const {
    return buffer_.size() - writerIndex_;
}

size_t Buffer::prependableBytes() const {
    return readerIndex_;
}

const char* Buffer::peek() const {
    return begin() + readerIndex_;
}

void Buffer::retrieve(size_t len) {
    assert(len <= readableBytes());
    if (len < readableBytes()) {
        readerIndex_ += len;
    } else {
        // len == readableBytes() 表明已读完
        retrieveAll();
    }
}

void Buffer::retrieveAll() {
    readerIndex_ = 0;
    writerIndex_ = 0;
}

void Buffer::retrieveUntil(const char* end) {
    assert(peek() <= end);
    assert(end <= beginWrite());
    retrieve(end - peek());
}

// 将可读区转化为string字符串返回
std::string Buffer::retrieveAllToStr() {
    std::string str(peek(), readableBytes());
    retrieveAll();
    return str;
}

const char* Buffer::beginWriteConst() const {
    return begin() + writerIndex_;
}

char* Buffer::beginWrite() {
    return begin() + writerIndex_;
}

void Buffer::hasWritten(size_t len) {
    writerIndex_ += len;
}

void Buffer::append(const std::string& str) {
    append(str.data(), str.length());
}

void Buffer::append(const void* data, size_t len) {
    assert(data);
    append(static_cast<const char*>(data), len);
}

void Buffer::append(const char* str, size_t len) {
    assert(str);
    ensureWriteable(len);
    std::copy(str, str + len, beginWrite());
    hasWritten(len);
}

void Buffer::append(const Buffer& buf) {
    append(buf.peek(), buf.readableBytes());
}

void Buffer::ensureWriteable(size_t len) {
    if (writableBytes() < len) {
        makeSpace(len);
    }
    assert(writableBytes() >= len);
}

ssize_t Buffer::readFd(int fd, int* saveErrno) {
    /*
    不能每有一个连接就准备一个超大缓冲区，太耗内存。
    所以可以折中一下，先搞一个小缓冲区，写数据的时候到缓冲区时，
    开辟一个栈上临时缓冲区，存储缓冲区装不下的数据
    如果缓冲区能装的下，就不需要扩容，反之就利用append扩容
    */
    char buff[65536];
    struct iovec iov[2];
    const size_t writeable = writableBytes();
    iov[0].iov_base = begin() + writerIndex_;
    iov[0].iov_len = writeable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    const ssize_t len = readv(fd, iov, 2);
    if (len < 0) {
        *saveErrno = errno;
    } else if (static_cast<size_t>(len) <= writeable) {
        writerIndex_ += len;
    } else {
        writerIndex_ = buffer_.size();
        append(buff, len - writeable);
    }
    return len;
}

ssize_t Buffer::writeFd(int fd, int* saveErrno) {
    // 将已经在缓冲区的数据发送出去
    size_t readSize = readableBytes();
    ssize_t len = write(fd, peek(), readSize);
    if (len < 0) {
        *saveErrno = errno;
        return len;
    }
    readerIndex_ += len;
    return len;
}

char* Buffer::begin() {
    return &*buffer_.begin();
}

const char* Buffer::begin() const {
    return &*buffer_.begin();
}

void Buffer::makeSpace(size_t len) {
    // 扩容
    if (writableBytes() + prependableBytes() < len) {
        buffer_.resize(writerIndex_ + len + 1);
    } else {
        // 内部腾挪
        size_t readable = readableBytes();
        std::copy(begin() + readerIndex_, begin() + writerIndex_, begin());
        readerIndex_ = 0;
        writerIndex_ = readerIndex_ + readable;
        assert(readable == readableBytes());
    }
}

}