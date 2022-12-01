//
// Created by pqpo on 2017/11/16.
//

#ifndef LOG4A_LOGBUFFER_H
#define LOG4A_LOGBUFFER_H

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
#include "MmapTrigger.h"
#include <zlib.h>

using namespace log_header;

class MetaData {
public:
    MetaData(char* ptr, size_t capacity);
    ~MetaData();

    void initData(char *log_path, size_t log_path_len, bool is_compress);
    size_t length();
    size_t append(const char* log, size_t len);
    void release();
    size_t emptySize();
    char *getLogPath();
    void setAsyncFileFlush(FileFlusher *fileFlush);
    void async_flush();
    void async_flush(FileFlusher *fileFlush);
    void async_flush(FileFlusher *fileFlush, MetaData *releaseThis);
    void changeLogPath(char *log_path);

public:
    bool map_buffer = true;

private:
    void clear();
    void setLength(size_t len);
    bool initCompress(bool compress);
    bool openSetLogFile(const char *log_path);

    FILE* log_file = nullptr;
    FileFlusher *fileFlush = nullptr;
    char* const buffer_ptr = nullptr;
    char* data_ptr = nullptr;
    char* write_ptr = nullptr;

    size_t buffer_size = 0;
    std::recursive_mutex log_mtx;

    MmapTrigger logHeader;
    z_stream zStream;
    bool is_compress = false;

};


#endif //LOG4A_LOGBUFFER_H
