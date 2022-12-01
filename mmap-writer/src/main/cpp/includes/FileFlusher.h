//
// Created by pqpo on 2017/11/23.
//

#ifndef LOG4A_FILEFLUSH_H
#define LOG4A_FILEFLUSH_H

#include <sys/types.h>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include "BufferFlusher.h"

class FileFlusher {

public:
    FileFlusher();
    ~FileFlusher();
    bool async_flush(BufferFlusher* flushBuffer);
    void stopFlush();

private:
    void async_log_thread();
    ssize_t flush(BufferFlusher* flushBuffer);

    bool exit = false;
    std::vector<BufferFlusher*> async_buffer;
    std::thread async_thread;
    std::condition_variable async_condition;
    std::mutex async_mtx;
};


#endif //LOG4A_FILEFLUSH_H
