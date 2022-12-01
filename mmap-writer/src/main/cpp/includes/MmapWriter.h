/**
 * Time:2022/1/10 17:25
 * Author:
 * Description:
 */

#ifndef XLOG_SDK_MMAPWRITER_H
#define XLOG_SDK_MMAPWRITER_H


static char *openMMap(int buffer_fd, size_t buffer_size);

static void writeDirtyLogToFile(int buffer_fd);



#endif //XLOG_SDK_MMAPWRITER_H
