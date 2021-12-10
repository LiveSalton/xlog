//
// Created by Administrator on 2021/12/10.
//


#include <string>
#include "headers/MmapBuffer.h"

std::string MmapBuffer::getVersion() {
    static const char *bufferPrefix = "1.0.0.0-snapshot";
    return std::string(bufferPrefix);
}

MmapBuffer::MmapBuffer(FILE *_file, size_t _size) {
    capacity = _size;
    file = _file;
}

MmapBuffer::~MmapBuffer() {
    if (dataPointer != nullptr) {
        delete[] dataPointer;
    }
}

/**
 *
 * @return
 */
size_t MmapBuffer::remain_size() {
    return capacity - length();
}


void MmapBuffer::wirte(void *data, size_t len) {
    if (dataPointer == nullptr) {
        capacity = (size_t) fmax(capacity, len);
        dataPointer = new char[capacity]{0};
        writerPointer = dataPointer;
    }
    size_t remainSize = remain_size();
    if (len < remainSize) {
        memcpy(writerPointer, data, len);
        writerPointer += len;
    } else {
        size_t nowLen = length();
        size_t newCapacity = nowLen + len;
        char *data_temp = new char[newCapacity]{0};
        memcpy(data_temp, dataPointer, nowLen);
        memcpy(data_temp + nowLen, data, len);
        char *old_data = dataPointer;
        dataPointer = data_temp;
        writerPointer = dataPointer + newCapacity;
        delete[]old_data;
    }

}

void MmapBuffer::reset() {
    if (dataPointer != nullptr) {
        memset(dataPointer, 0, capacity);
        writerPointer = dataPointer;
    }
}

size_t MmapBuffer::length() {
    if (dataPointer != nullptr && writerPointer != nullptr) {
        return writerPointer - dataPointer;
    }
    return 0;
}

void *MmapBuffer::getDataPointer() {
    return dataPointer;
}

FILE *MmapBuffer::getFile() {
    return file;
}




