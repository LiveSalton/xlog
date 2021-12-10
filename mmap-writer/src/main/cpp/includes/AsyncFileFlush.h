/**
 * Time:2021/12/10 14:25
 * Author:
 * Description:
 */
//模版代码的写法 https://www.runoob.com/w3cnote/c-templates-detail.html
#ifndef XLOG_SDK_ASYNCFILEFLUSH_H
#define XLOG_SDK_ASYNCFILEFLUSH_H

#include <sys/types.h>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include "FlushBuffer.h"

class AsyncFileFlush {

public:
    AsyncFileFlush();
    ~AsyncFileFlush();
    bool async_flush(FlushBuffer* flushBuffer);
    void stopFlush();

private:
    void async_log_thread();
    ssize_t flush(FlushBuffer* flushBuffer);

    bool exit = false;
    std::vector<FlushBuffer*> async_buffer;
    std::thread async_thread;
    std::condition_variable async_condition;
    std::mutex async_mtx;
};

#endif //XLOG_SDK_ASYNCFILEFLUSH_H
