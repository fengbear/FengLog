#include "log/Logger.h"
#include "log/LoggerManager.h"
#include "base/ThreadPool.h"
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <atomic>
#include <sys/time.h>

static const int NUM = 10000 * 100;

std::atomic<int> cnt = {0};
feng::ThreadPool &pool = feng::ThreadPool::getInstance();

double compute_timediff(const struct timeval &tvBegin, const struct timeval &tvEnd){
    return (tvEnd.tv_sec - tvBegin.tv_sec) + ((tvEnd.tv_usec - tvBegin.tv_usec) / 1000.0) / 1000.0;
}

void work(int id){
    FENG_LOG_INFO(FENG_LOG_SYNC_FILE_ROOT()) << "work: " << id;
    ++cnt;
}

int main(){
    pool.start();
    struct timeval tvBegin, tvEnd;

    gettimeofday(&tvBegin, NULL);
    for(int i = 0; i < NUM; ++i){
        pool.addTask(std::bind(work, i));
    }
    std::cout << "cnt: " << cnt << std::endl;
    while(cnt < NUM);
    std::cout << "task count: " << cnt.load() << std::endl;
    gettimeofday(&tvEnd, NULL);
    std::cout << "time cost: " << compute_timediff(tvBegin, tvEnd) << " seconds" << std::endl;
    return 0;
}