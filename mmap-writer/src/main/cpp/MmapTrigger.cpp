/**
 * Time:2022/1/6 11:01
 * Author:
 * Description: 文件缓存交接处理层(filename_buf)
 */

#include "includes/MmapTrigger.h"

MmapTrigger::MmapTrigger(char *dataPointer, size_t bufferSize) :
        _bufferPointer(dataPointer),
        _bufferSize(bufferSize),
        metaData(_bufferPointer, _bufferSize) {
    fileFlusher = new FileFlusher();
    if (metaData.isAvailable()) {
        //这一步数据指针已经加上了metadata的偏移
        _dataPointer = (char *) metaData.metaPointer();
        _writerPointer = (char *) metaData.writePointer();
        if (metaData.isCompressed()) {
            initCompressProperty();
        }
        char *logPath = metaData.getLogPath();
        if (logPath != nullptr) {
            openLogFile(logPath);
            //释放
            ::operator delete(logPath);
        }
    }
    //清空zStream
    memset(&zStream, 0, sizeof(zStream));
}

MmapTrigger::~MmapTrigger() {
    std::lock_guard<std::recursive_mutex> lck_release(logMutex);
    if (compressed && Z_NULL != zStream.state) {
        deflateEnd(&zStream);
    }
    if (enableMmap) {
        munmap(_bufferPointer, _bufferSize);
    } else {
        ::operator delete(_bufferPointer);
    }
    if (_logFile != nullptr) {
        fclose(_logFile);
    }
}

size_t MmapTrigger::getLogLength() {
    return _writerPointer - _dataPointer;
}

void MmapTrigger::updateLogLength(size_t length) {
    metaData.setLogLength(length);
}

void MmapTrigger::init(char *logPath, size_t logPathLength, bool compressed) {
    //简化上锁解锁代码
    std::lock_guard<std::recursive_mutex> lock_release(logMutex);
    memset(_bufferPointer, '\0', _bufferSize);
    space_mmap_writer::Meta meta;
    meta.magic = metaMagic;
    meta.logPathLength = logPathLength;
    meta.logPath = logPath;
    meta.logLength = 0;
    meta.compressed = compressed;
    metaData.init(meta);
    initCompressProperty();

    _dataPointer = (char *) metaData.metaPointer();
    _writerPointer = (char *) metaData.writePointer();

    openLogFile(logPath);
}

size_t MmapTrigger::append(const char *log, size_t length) {
    std::lock_guard<std::recursive_mutex> lock_append(logMutex);
    //没有要写入的内容
    if (getLogLength() == 0) {
        initCompressProperty();
    }

    size_t freeSize = emptySize();
    size_t writeSize = 0;
    if (compressed) {
        zStream.next_in = (Bytef *) log;
        zStream.avail_in = (uInt) length;
        zStream.next_out = (Bytef *) _writerPointer;
        zStream.avail_out = (uInt) freeSize;

        if (Z_OK != deflate(&zStream, Z_SYNC_FLUSH)) {
            return 0;
        }
        writeSize = freeSize - zStream.avail_out;
    } else {
        writeSize = length <= freeSize ? length : freeSize;
        memcpy(_writerPointer, log, writeSize);
    }
    _writerPointer += writeSize;
    updateLogLength(getLogLength());
    return writeSize;
}

/**
 * 剩余未写入的空间
 * @return
 */
size_t MmapTrigger::emptySize() {
    return _bufferSize - (_writerPointer - _bufferPointer);
}

void MmapTrigger::initCompressProperty() {
    if (compressed) {
        zStream.zalloc = Z_NULL;
        zStream.zfree = Z_NULL;
        zStream.opaque = Z_NULL;
        deflateInit2(&zStream, Z_BEST_COMPRESSION, Z_DEFLATED, -MAX_WBITS, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY);
    }
}

void MmapTrigger::asyncFlush(void *releaseThis) {
    if (fileFlusher == nullptr) {
        if (releaseThis != nullptr) {
            delete releaseThis;
        }
        return;
    }
    std::lock_guard<std::recursive_mutex> lck_clear(logMutex);
    if (getLogLength() > 0) {
        if (compressed && Z_NULL != zStream.state) {
            deflateEnd(&zStream);
        }
        BufferFlusher *bufferFlusher = new BufferFlusher(_logFile);
        bufferFlusher->write(_dataPointer, getLogLength());
        bufferFlusher->release(releaseThis);
        clear();
        fileFlusher->runAsync(bufferFlusher);
    } else if (releaseThis != nullptr) {
        delete releaseThis;
    }
}

void MmapTrigger::clear() {
    std::lock_guard<std::recursive_mutex> lock_clear(logMutex);
    _writerPointer = _dataPointer;
    memset(_writerPointer, '\0', emptySize());
    updateLogLength(getLogLength());
}

bool MmapTrigger::openLogFile(const char *log_path) {
    if (log_path != nullptr) {
        FILE *_file_log = fopen(log_path, "ab+");
        if (_file_log != NULL) {
            _logFile = _file_log;
            return true;
        }
    }
    return false;
}

