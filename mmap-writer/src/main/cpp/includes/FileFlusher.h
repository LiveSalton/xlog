/**
 * Time:2022/1/7 14:41
 * Author:
 * Description: 差速器，协调mmap高速缓存和文件流之间的速度差异
 */

#ifndef XLOG_SDK_FILEFLUSHER_H
#define XLOG_SDK_FILEFLUSHER_H

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

    bool runAsync(BufferFlusher *flushBuffer);

private:
    void asyncThreadTask();

    ssize_t flush(BufferFlusher *flushBuffer);

    std::vector<BufferFlusher *> asyncBuffer;
    std::thread asyncThread;
    std::condition_variable asyncCondition;
    std::mutex asyncMutex;
    bool isExited = false;
};

#endif //XLOG_SDK_FILEFLUSHER_H
