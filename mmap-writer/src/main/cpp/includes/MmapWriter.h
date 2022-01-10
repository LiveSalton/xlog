/**
 * Time:2022/1/10 17:25
 * Author:
 * Description:
 */

#ifndef XLOG_SDK_MMAPWRITER_H
#define XLOG_SDK_MMAPWRITER_H


static void writeDirtyDataToFile(int description);

static char *openMmap(int bufferFileDescription, size_t bufferSize);

void asyncFlush(jlong buffer_pointer_);


#endif //XLOG_SDK_MMAPWRITER_H
