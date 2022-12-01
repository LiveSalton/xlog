//
// Created by pqpo on 2017/11/23.
//

#include "FileFlusher.h"

FileFlusher::FileFlusher() {
    async_thread = std::thread(&FileFlusher::async_log_thread, this);
}

FileFlusher::~FileFlusher() {
    stopFlush();
}

void FileFlusher::async_log_thread() {
    while (true) {
        std::unique_lock<std::mutex> lck_async_log_thread(async_mtx);
        while (!async_buffer.empty()) {
            BufferFlusher* data = async_buffer.back();
            async_buffer.pop_back();
            flush(data);
        }
        if (exit) {
            return;
        }
        async_condition.wait(lck_async_log_thread);
    }
}

ssize_t FileFlusher::flush(BufferFlusher* flushBuffer) {
    ssize_t written = 0;
    FILE* log_file = flushBuffer->logFile();
    if(log_file != nullptr && flushBuffer->length() > 0) {
        written = fwrite(flushBuffer->ptr(), flushBuffer->length(), 1, log_file);
        fflush(log_file);
    }
    delete flushBuffer;
    return written;
}

bool FileFlusher::async_flush(BufferFlusher* flushBuffer) {
    std::unique_lock<std::mutex> lck_async_flush(async_mtx);
    if (exit) {
        delete flushBuffer;
        return false;
    }
    async_buffer.push_back(flushBuffer);
    async_condition.notify_all();
    return true;
}

void FileFlusher::stopFlush() {
    exit = true;
    async_condition.notify_all();
    async_thread.join();
}
