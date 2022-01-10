/**
 * Time:2022/1/6 11:01
 * Author:
 * Description: 缓冲触发器，mmap共享空间写满后，把内容转写到文件。
 * 协调mmap高速缓存和文件流之间的速度差异
 */

#ifndef XLOG_SDK_MMAPTRIGGER_H
#define XLOG_SDK_MMAPTRIGGER_H

#include <string>
#include <math.h>
#include <unistd.h>
#include <sys/mman.h>
#include <thread>
#include<vector>
#include <mutex>
#include <condition_variable>
#include "FileFlusher.h"
#include "BufferFlusher.h"
#include "Metadata.h"
#include <zlib.h>

using namespace space_mmap_writer;

class MmapTrigger {
public:
    MmapTrigger(char *dataPointer, size_t bufferSize);

    ~MmapTrigger();

    void init(char *logPath, size_t logPathLength, bool compressed);

    size_t getLogLength();

    size_t append(const char *log, size_t length);

    size_t emptySize();

    void asyncFlush(void *releaseThis);

public:
    bool enableMmap = true;
    FileFlusher *fileFlusher = nullptr;

private:
    void clear();

    void updateLogLength(size_t length);

    void initCompressProperty();

    bool openLogFile(const char *logPath);

    FILE *_logFile = nullptr;

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

#endif //XLOG_SDK_MMAPTRIGGER_H
