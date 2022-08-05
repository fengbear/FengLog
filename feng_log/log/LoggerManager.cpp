#include "log/LoggerManager.h"

namespace feng {
namespace log {


Logger::ptr LoggerManager::getLogger(const std::string& name) {
    MutexLockGuard lock_(mutex_);
    auto it = loggers_.find(name);
    if (it != loggers_.end()) {
        return it->second;
    }
    return nullptr;
}


}
}