/**
 * Time:2022/1/5 16:03
 * Author:
 * Description: 写入到文件的元数据，一个magic作为特征，记录上次写入的位置，日志路径及路径长度
 */

#include "includes/Metadata.h"

using namespace log_header;

MetaData::MetaData(void *bufferData, size_t bufferSize) {
    _bufferData = (char *) bufferData;
    _bufferSize = bufferSize;
}

MetaData::~MetaData() {}

size_t MetaData::getMetaLength() {
    if (isAvailable()) {
        return metaLength(getLogPathLength());
    }
    return 0;
}

size_t MetaData::getLogPathLength() {
    if (isAvailable()) {
        size_t log_path_len = 0;
        memcpy(&log_path_len, _bufferData + sizeof(char) + sizeof(size_t), sizeof(size_t));
        // logpath 的长度不能大于整个buffer 减去header 中其它数据的长度
        if (log_path_len < 0 || log_path_len > _bufferSize - metaLength(0)) {
            log_path_len = 0;
        }
        return log_path_len;
    }
    return 0;
}

char * MetaData::getLogPath() {
    if (isAvailable()) {
        size_t log_path_len = getLogPathLength();
        if (log_path_len > 0) {
            char *log_path = new char[log_path_len + 1];
            memset(log_path, 0, log_path_len + 1);
            memcpy(log_path, _bufferData + sizeof(char) + sizeof(size_t) + sizeof(size_t), log_path_len);
            return log_path;
        }
    }
    return nullptr;
}

void MetaData::setLogLength(size_t _logLength) {
    if (isAvailable()) {
        memcpy(_bufferData + sizeof(char), &_logLength, sizeof(size_t));
    }
}

void *MetaData::metaPointer() {
    return _bufferData + getMetaLength();
}

void *MetaData::writePointer() {
    return _bufferData + getMetaLength() + getLogLength();
}


/**
 * 初始化
 * @param meta
 */
void MetaData::init(Meta &meta) {
    if ((sizeof(char) + sizeof(size_t) + sizeof(size_t) + meta.logPathLength) > _bufferSize) {
        return;
    }
    memcpy(_bufferData, &meta.magic, sizeof(char));
    memcpy(_bufferData + sizeof(char), &meta.logLength, sizeof(size_t));
    memcpy(_bufferData + sizeof(char) + sizeof(size_t), &meta.logPathLength, sizeof(size_t));
    memcpy(_bufferData + sizeof(char) + sizeof(size_t) + sizeof(size_t), meta.logPath, meta.logPathLength);
    int tag = ((meta.compressed) ? 1 : 0);
    memcpy(_bufferData + sizeof(char) + sizeof(size_t) + sizeof(size_t) + meta.logPathLength,
           &tag, sizeof(char));
}

/**
 * 从init方法里面可以看到compressed是放在以下位置
 * _bufferData + sizeof(char) + sizeof(size_t) + sizeof(size_t) + meta.logPathLength
 * @return
 */
bool MetaData::isCompressed() {
    if (isAvailable()) {
        char isCompressed = (_bufferData + sizeof(char) + sizeof(size_t) + sizeof(size_t) + getLogLength())[0];
        return isCompressed == 1;
    }
    return false;
}

/**
 *
 * @return
 */
size_t MetaData::getLogLength() {
    size_t logLength = 0;
    if (isAvailable()) {
        memcpy(&logLength, _bufferData + sizeof(char), sizeof(size_t));
        // log长度总是大于 0 并小于 buffer_size 减去 header 长度的
        if (logLength < 0 || logLength > (_bufferSize - getMetaLength())) {
            logLength = 0;
        }
    }
    return logLength;
}

/**
 * 判断是否为
 * @return
 */
bool MetaData::isAvailable() {
    return _bufferData[0] == metaMagic;
}

/**
 * 计算meta需要的长度
 * @param _logPathLength 路径文件长度
 * @return meta占用字节数
 * sizeof(char) = 1
 * sizeof(size_t) = 8
 * 第一个放magic
 * 第二个放getLogLength()
 * 第三个放getLogPathLength()
 * 第四个放_logPathLength
 * 第五个放compressed
 */
 size_t MetaData::metaLength(size_t _logPathLength) {
    return sizeof(char) + sizeof(size_t) + sizeof(size_t) + _logPathLength + sizeof(char);
}




