//
// Created by Administrator on 2021/12/10.
//

#ifndef MMAP_WRITER_MMAPBUFFER_H
#define MMAP_WRITER_MMAPBUFFER_H

class MmapBuffer {
public:
    MmapBuffer(FILE *_file, size_t _size = 128);

    ~MmapBuffer();

    void wirte(void *data, size_t len);

    void reset();

    size_t length();

    void *getDataPointer();

    FILE *getFile();

    void release(void *release);

    std::string getVersion();

private:
    char *dataPointer = nullptr;
    char *writerPointer = nullptr;
    FILE *file = nullptr;
    size_t capacity;

    size_t  remain_size();
};

#endif //MMAP_WRITER_MMAPBUFFER_H
