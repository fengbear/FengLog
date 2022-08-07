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

static const int NUM = 10000 * 100;

double compute_timediff(const struct timeval &tvBegin, const struct timeval &tvEnd){
    return (tvEnd.tv_sec - tvBegin.tv_sec) + ((tvEnd.tv_usec - tvBegin.tv_usec) / 1000.0) / 1000.0;
}

int main() {
    struct timeval tvBegin, tvEnd;

    gettimeofday(&tvBegin, NULL);

    for (int i = 0; i < NUM; ++i) {
        FENG_LOG_INFO(FENG_LOG_SYNC_FILE_ROOT()) << "work: " << i << " done";   
    }

    gettimeofday(&tvEnd, NULL);
    std::cout << "task count: " << NUM << std::endl;
    std::cout << "time cost: " << compute_timediff(tvBegin, tvEnd) << " seconds" << std::endl;
    return 0;

}
