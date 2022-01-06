/**
 * Time:2022/1/5 16:03
 * Author:
 * Description:
 */

#ifndef XLOG_SDK_METADATA_H
#define XLOG_SDK_METADATA_H

#include <string>
#include <string.h>

namespace log_header {
    static const char metaMagic = '\x11';

    struct Meta {
        char magic = metaMagic;
        size_t logLength;
        size_t logPathLength;
        char *logPath;
        bool compressed;
    };

    class MetaData {
    public:
        MetaData(void *bufferData, size_t bufferSize);

        ~MetaData();

        void init(Meta &meta);

        void *metaPointer();

        void *writePointer();

        size_t getMetaLength();

        size_t getLogLength();

        size_t getLogPathLength();

        char *getLogPath();

        void setLogLength(size_t _logLength);

        bool isCompressed();

        bool isAvailable();

        static size_t metaLength(size_t _logPathLength);

    private:
        char *_bufferData;
        size_t _bufferSize;
    };
}
#endif //XLOG_SDK_METADATA_H
