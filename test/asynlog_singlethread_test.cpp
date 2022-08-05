#include "log/Logger.h"
#include "log/LoggerManager.h"
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>


#define TEST_LOGGERER_WAPPER(logger) \
    feng::log::LogWrapper(logger, feng::log::LogEvent::ptr(new feng::log::LogEvent(logger->getName(), \
                          feng::log::LogLevel::Level::DEBUG, \
                          __FILE__, __LINE__, 0, feng::GetThreadId(), \
                          time(0))))


#define TEST_LOGGEREVENT(logger) \
    feng::log::LogEvent::ptr(new feng::log::LogEvent(logger->getName(), \
                          feng::log::LogLevel::Level::DEBUG, \
                          __FILE__, __LINE__, 0, feng::GetThreadId(), \
                          time(0)))

static const int NUM = 1;

int main() {
    struct timeval tvBegin;

    gettimeofday(&tvBegin, NULL);

    for (int i = 0; i < NUM; ++i) {
        FENG_LOG_INFO(FENG_LOG_SYNC_FILE_ROOT()) << "work: " << i << " done";
        
    }

}
