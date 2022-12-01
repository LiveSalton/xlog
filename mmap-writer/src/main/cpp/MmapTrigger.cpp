//
// Created by pqpo on 2018/2/10.
//

#include <ctime>
#include "MmapTrigger.h"

using namespace log_header;

//struct Header {
//    char magic;
//    size_t log_len;
//    size_t log_path_len;
//    char* log_path;
//    char isCompress;
//};

MmapTrigger::MmapTrigger(void *data, size_t size) : data_ptr((char *) data), data_size(size) {
}

MmapTrigger::~MmapTrigger() {
}

void *MmapTrigger::originPtr() {
    return data_ptr;
}

Header* MmapTrigger::getHeader() {
    Header* header = new Header();
    if (isAvailable()) {
        header->magic = kMagicHeader;
        size_t log_len = 0;
        memcpy(&log_len, data_ptr + sizeof(char), sizeof(size_t));
        header->log_len = log_len;
        size_t log_path_len = 0;
        memcpy(&log_path_len, data_ptr + sizeof(char) + sizeof(size_t), sizeof(size_t));
        header->log_path_len = log_path_len;
        char *log_path = new char[log_path_len + 1];
        memset(log_path, 0, log_path_len + 1);
        memcpy(log_path, data_ptr + sizeof(char) + sizeof(size_t) + sizeof(size_t), log_path_len);
        header->log_path = log_path;
        char isCompress = (data_ptr + sizeof(char) + sizeof(size_t) + sizeof(size_t) + log_path_len)[0];
        header->isCompress = isCompress == 1;
    }
    return header;
}

size_t MmapTrigger::getHeaderLen() {
    if (isAvailable()) {
        return calculateHeaderLen(getLogPathLen());
    }
    return 0;
}

void *MmapTrigger::ptr() {
    return data_ptr + getHeaderLen();
}

void *MmapTrigger::write_ptr() {
    return data_ptr + getHeaderLen() + getLogLen();
}

void MmapTrigger::initHeader(Header &header) {
    if ((sizeof(char) + sizeof(size_t) + sizeof(size_t) + header.log_path_len) > data_size) {
        return;
    }
    memcpy(data_ptr, &header.magic, sizeof(char));
    memcpy(data_ptr + sizeof(char), &header.log_len, sizeof(size_t));
    memcpy(data_ptr + sizeof(char) + sizeof(size_t), &header.log_path_len, sizeof(size_t));
    memcpy(data_ptr + sizeof(char) + sizeof(size_t) + sizeof(size_t), header.log_path, header.log_path_len);
    char isCompress = 0;
    if (header.isCompress) {
        isCompress = 1;
    }
    memcpy(data_ptr + sizeof(char) + sizeof(size_t) + sizeof(size_t) + header.log_path_len, &isCompress,
           sizeof(char));

}

size_t MmapTrigger::getLogLen() {
    if (isAvailable()) {
        size_t log_len = 0;
        memcpy(&log_len, data_ptr + sizeof(char), sizeof(size_t));
        // log长度总是大于 0 并小于 buffer_size 减去 header 长度的
        if (log_len <= 0 || log_len > (data_size - getHeaderLen())) {
            log_len = 0;
        }
        return log_len;
    }
    return 0;
}

size_t MmapTrigger::getLogPathLen() {
    if (isAvailable()) {
        size_t log_path_len = 0;
        memcpy(&log_path_len, data_ptr + sizeof(char) + sizeof(size_t), sizeof(size_t));
        // logpath 的长度不能大于整个buffer 减去header 中其它数据的长度
        if (log_path_len <= 0 || log_path_len > data_size - calculateHeaderLen(0)) {
            log_path_len = 0;
        }
        return log_path_len;
    }
    return 0;
}

char *MmapTrigger::getLogPath() {
    if (isAvailable()) {
        size_t log_path_len = getLogPathLen();
        if (log_path_len > 0) {
            char *log_path = new char[log_path_len + 1];
            memset(log_path, 0, log_path_len + 1);
            memcpy(log_path, data_ptr + sizeof(char) + sizeof(size_t) + sizeof(size_t), log_path_len);
            return log_path;
        }
    }
    return nullptr;
}

void MmapTrigger::setLogLen(size_t log_len) {
    if (isAvailable()) {
        memcpy(data_ptr + sizeof(char), &log_len, sizeof(size_t));
    }
}

bool MmapTrigger::isAvailable() {
    return data_ptr[0] == kMagicHeader;
}

bool MmapTrigger::getIsCompress() {
    if (isAvailable()) {
        char isCompress = (data_ptr + sizeof(char) + sizeof(size_t) + sizeof(size_t) + getLogPathLen())[0];
        return isCompress == 1;
    }
    return false;
}

size_t MmapTrigger::calculateHeaderLen(size_t log_path_len) {
    return sizeof(char) + sizeof(size_t) + sizeof(size_t) + log_path_len + sizeof(char);
}






