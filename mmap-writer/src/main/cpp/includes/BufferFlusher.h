/**
 * Time:2021/12/10 14:27
 * Author:
 * Description:
 */

#ifndef XLOG_SDK_BUFFERFLUSHER_H
#define XLOG_SDK_BUFFERFLUSHER_H

#include <sys/types.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

class BufferFlusher {

public:
    BufferFlusher(FILE* log_file, size_t size = 128);
    ~BufferFlusher();
    void write(void* data, size_t len);
    void reset();
    size_t length();
    void* ptr();
    FILE* logFile();

    void releaseThis(void *release);

private:
    FILE* log_file = nullptr;
    void* release = nullptr;
    char* data_ptr = nullptr;
    char* write_ptr = nullptr;
    size_t capacity;

    size_t emptySize();

};


#endif //XLOG_SDK_BUFFERFLUSHER_H
