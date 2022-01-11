/**
 * Time:2022/1/10 17:51
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
public :
    BufferFlusher(FILE *logFile, size_t blockSize = 128);

    ~BufferFlusher();

    void write(void *data, size_t length);

    size_t getLength();

    void *dataPointer();

    FILE *logFile();

    void release(void *releaseObj);

private:
    FILE *_logFile = nullptr;
    void *_releaseObj = nullptr;
    char *_dataPointer = nullptr;
    char *_writerPointer = nullptr;
    size_t _blockSize;

    size_t emptySize();
};

#endif //XLOG_SDK_BUFFERFLUSHER_H
