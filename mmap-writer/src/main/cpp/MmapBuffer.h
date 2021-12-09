
#ifndef  MMAP_BUFFER_HEADER
#define  MMAP_BUFFER_HEADER

#include <stdio.h>

class MmapBuffer {

public:
    MmapBuffer(FILE *file, size_t size = 128);

    ~MmapBuffer();

    void write(void *data, size_t len);

    void reset();

    size_t length();

    void *pointer();

    FILE *file();

    void release(void *release);


private:
    FILE *log_file = nullptr;
    void *release = nullptr;
    char *data_ptr = nullptr;
    char *write_ptr = nullptr;
    size_t capacity;

    size_t emptySize();
};

#endif