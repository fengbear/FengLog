#ifndef FENG_BASE_NONCOPYABLE_H
#define FENG_BASE_NONCOPYABLE_H

namespace feng
{

class noncopyable
{ 
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;

};

} // namespace feng

#endif // FENG_BASE_NONCOPYABLE_H