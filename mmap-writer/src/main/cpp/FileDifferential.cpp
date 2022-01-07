/**
 * Time:2022/1/7 14:41
 * Author:
 * Description:
 */

#include "includes/FileDifferential.h"

FileDifferential::FileDifferential() {
    //初始化线程
    asyncThread = std::thread(&FileDifferential::asyncThreadTask, this);
}

FileDifferential::~FileDifferential() {
    isExited = true;
    asyncCondition.notify_all();
    asyncThread.join();
}

void FileDifferential::asyncThreadTask() {
    while (true) {
        std::unique_lock<std::mutex> lockAsyncLog(asyncMutex);
        while (!asyncBuffer.empty()) {
            //不断写入数据
            FlushBuffer *flushBuffer = asyncBuffer.back();
            asyncBuffer.pop_back(); //移除
            flush(flushBuffer);
        }
        if (isExited) {
            return;
        }
        //等待下一次写入
        asyncCondition.wait(lockAsyncLog);
    }
}

bool FileDifferential::asyncFlush(FlushBuffer *flushBuffer) {
    std::unique_lock<std::mutex> lck_async_flush(asyncMutex);
    if (isExited) {
        delete flushBuffer;
        return false;
    }
    //压入 flushBuffer
    asyncBuffer.push_back(flushBuffer);
    //唤醒所有的等待(wait)线程。如果当前没有等待线程，则该函数什么也不做
    asyncCondition.notify_all();
    return true;
}

ssize_t FileDifferential::flush(FlushBuffer *flushBuffer) {
    ssize_t written = 0;
    FILE *logFile = flushBuffer->logFile();
    if (logFile != nullptr && flushBuffer->length() > 0) {
        written = fwrite(flushBuffer->ptr(), flushBuffer->length(), 1, logFile);
        fflush(logFile);
    }
    delete flushBuffer;
    return written;
}