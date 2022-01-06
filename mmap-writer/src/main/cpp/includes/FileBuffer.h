/**
 * Time:2022/1/6 11:01
 * Author:
 * Description:
 */

#ifndef XLOG_SDK_FILEBUFFER_H
#define XLOG_SDK_FILEBUFFER_H

#include <string>
#include <math.h>
#include <unistd.h>
#include <sys/mman.h>
#include <thread>
#include<vector>
#include <mutex>
#include <condition_variable>
#include "AsyncFileFlush.h"
#include "FlushBuffer.h"
#include "Metadata.h"
#include <zlib.h>

using namespace log_header;

class FileBuffer {
public:
    FileBuffer(char *dataPointer, size_t bufferSize);

    ~FileBuffer();

    void init(char *logPath, size_t logPathLength, bool compressed);

    size_t getLogLength();

    size_t append(const char *log, size_t length);

    size_t emptySize();

    void setAsyncFileFlush(AsyncFileFlush *flush);

    void asyncFlush(AsyncFileFlush *flush, void *releaseThis);

public:
    bool enableMmap = true;

private:
    void clear();

    void updateLogLength(size_t length);

    void initCompressProperty();

    bool openLogFile(const char *logPath);

    FILE *_logFile = nullptr;

    AsyncFileFlush *asyncFileFlush = nullptr;

    char *const _bufferPointer = nullptr;

    char *_dataPointer = nullptr;

    char *_writerPointer = nullptr;

    size_t _bufferSize = 0;

    //递归锁
    std::recursive_mutex logMutex;
    MetaData metaData;
    z_stream zStream;
    bool compressed = false;
};

#endif //XLOG_SDK_FILEBUFFER_H
