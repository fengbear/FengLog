#ifndef __FENG_BASE_SINGLETON_H
#define __FENG_BASE_SINGLETON_H

#include "base/noncopyable.h"
#include <memory>
namespace feng {

template<typename T>
class Singleton: noncopyable {
public:
    static T& getInstance() {
        static T instance_;
        return instance_;
    }
};


// 返回单例智能指针
template<typename T>
class SingletonPtr : noncopyable {
public:
    static std::shared_ptr<T> GetInstance() {
        static std::shared_ptr<T> v(new T);
        return v;
    }
};

}



#endif