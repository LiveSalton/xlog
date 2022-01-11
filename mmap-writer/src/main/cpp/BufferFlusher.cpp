/**
 * Time:2022/1/10 17:51
 * Author:
 * Description:
 */

#include "includes/BufferFlusher.h"

BufferFlusher::BufferFlusher(FILE *logFile, size_t blockSize) {
    _logFile = logFile;
    _blockSize = blockSize;
}

BufferFlusher::~BufferFlusher() {
    if (_dataPointer != nullptr) {
        ::operator delete(_dataPointer);
    }
    if (_releaseObj != nullptr) {
        ::operator delete(_releaseObj);
    }
}

size_t BufferFlusher::getLength() {
    if (_dataPointer != nullptr && _writerPointer != nullptr) {
        return _writerPointer - _dataPointer;
    }
    return 0;
}

void *BufferFlusher::dataPointer() {
    return _dataPointer;
}

size_t BufferFlusher::emptySize() {
    return _blockSize - getLength();
}

void BufferFlusher::write(void *data, size_t length) {
    if (_dataPointer == nullptr) {
        _blockSize = fmax(_blockSize, length);
        _dataPointer = new char[_blockSize]{0};
        _writerPointer = _dataPointer;
    }

    size_t empSize = emptySize();
    if (length < empSize) {
        memcpy(_writerPointer, data, length);
        _writerPointer += length;
    } else {
        //扩容
        size_t nowLength = getLength();
        size_t newBlockSize = nowLength + length;
        //指针头
        char *dataTmp = new char[newBlockSize]{0};
        //复制旧内容
        memcpy(dataTmp, _dataPointer, nowLength);
        //复制新内容
        memcpy(dataTmp + nowLength, data, length);
        _dataPointer = dataTmp;
        //新写入的进度
        _writerPointer += length;
    }
}

FILE *BufferFlusher::logFile() {
    return _logFile;
}

//等待数据写完之后，析构该实例的同时析构releaseObj
void BufferFlusher::release(void *releaseObj) {
    this->_releaseObj = releaseObj;
}