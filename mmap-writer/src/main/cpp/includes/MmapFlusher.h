//模版代码的写法 https://www.runoob.com/w3cnote/c-templates-detail.html
#ifndef MMAP_FLUSHER
#define MMAP_FLUSHER

#include <sys/types.h>

class MmapFlusher {
public:
    //构造函数
    MmapFlusher(MmapFlusher *buffer);

    //析构函数
    ~MmapFlusher();

    //开始写入缓存
    void start();

    //停止写入缓存
    void stop();

private:
    void createThread();

    ssize_t flush();


};


#endif