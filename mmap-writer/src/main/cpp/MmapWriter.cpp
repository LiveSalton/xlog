/**
 * Time:2022/1/6 11:01
 * Author:
 * Description: 基于mmap的文本写入器
 */

#include <jni.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <FileBuffer.h>

static size_t calculateHeaderLength(size_t strlen);

static void writeDirtyDataToFile(int description);

static char *openMmap(int bufferFileDescription, size_t bufferSize);

void flushInfo(jlong buffer_pointer_);

static FileDifferential *fileFlush = nullptr;

static char *openMmap(int bufferFileDescription, size_t bufferSize) {
    char *mapPointer = nullptr;
    if (bufferFileDescription != -1) {
        //写入之前的未曾写入脏数据
        writeDirtyDataToFile(bufferFileDescription);
        //根据bufferSize调整buffer文件大小
        /**
         * ftruncate()会将参数fd 指定的文件大小改为参数length 指定的大小。
         * 参数fd 为已打开的文件描述词，而且必须是以写入模式打开的文件。
         * 如果原来的文件大小比参数length 大，则超过的部分会被删去。
         */
        ftruncate(bufferFileDescription, static_cast<int>(bufferSize));
        /**
         * 每一个已打开的文件都有一个读写位置, 当打开文件时通常其读写位置是指向文件开头,
         * 若是以附加的方式打开文件(如O_APPEND), 则读写位置会指向文件尾.
         * 当read()或write()时, 读写位置会随之增加,lseek()便是用来控制该文件的读写位置.
         * 参数fildes 为已打开的文件描述词, 参数offset 为根据参数whence来移动读写位置的位移数.
         *  SEEK_SET 参数offset 即为新的读写位置.
         *  SEEK_CUR 以目前的读写位置往后增加offset 个位移量.
         *  SEEK_END 将读写位置指向文件尾后再增加offset 个位移量.
         *  当whence 值为SEEK_CUR 或 SEEK_END 时, 参数offet 允许负值的出现.
         */
        lseek(bufferFileDescription, 0, SEEK_SET);
        mapPointer = (char *) mmap(0, bufferSize, PROT_WRITE | PROT_READ, MAP_SHARED, bufferFileDescription, 0);
        if (mapPointer == MAP_FAILED) {
            mapPointer = nullptr;
        }
    }
    return mapPointer;
}

/**
 * 将缓存中的内容写入到文件中
 * @param bufferFileDescription
 */
static void writeDirtyDataToFile(int bufferFileDescription) {
    struct stat fileInfo;
    /**
     *  fstat 用来将参数buffer_fd所指的文件状态，复制到参数fileInfo所指的结构中(struct stat)。
     *  读取文件基本信息
     */
    if (fstat(bufferFileDescription, &fileInfo) >= 0) {
        //获取文件大小
        size_t bufferSize = static_cast<size_t>(fileInfo.st_size);
        // buffer_size 必须是大于文件头长度的，否则会导致下标溢出
        // 检查文件的大小和记录在文件头的大小信息对比
        if (bufferSize > calculateHeaderLength(0)) {
            char *bufferPointerTmp = (char *) mmap(
                    0, bufferSize,
                    PROT_WRITE | PROT_READ, MAP_SHARED,
                    bufferFileDescription, 0);
            if (bufferPointerTmp != MAP_FAILED) {
                //写入脏数据
                FileBuffer *logBufferTmp = new FileBuffer(bufferPointerTmp, bufferSize);
                //实际写入的数据要比文件限制小
                size_t dataSize = logBufferTmp->getLogLength();
                if (dataSize > 0) {
                    logBufferTmp->asyncFlush(fileFlush, logBufferTmp);
                } else {
                    delete logBufferTmp;
                }
            }
        }
    }
}

size_t calculateHeaderLength(size_t strlen) {
    return sizeof(char) + sizeof(size_t) + sizeof(size_t) + strlen + sizeof(char);
}

void flushInfo(jlong buffer_pointer_) {
    FileBuffer *logBuffer = reinterpret_cast<FileBuffer *>(buffer_pointer_);
    logBuffer->asyncFlush(fileFlush, nullptr);
}

extern "C"
JNIEXPORT jlong JNICALL
/**
 *
 * @param env
 * @param thiz
 * @param save_path 文件保存目录
 * @param capacity 缓存文件大小
 * @param compress 是否进行压缩
 * @return
 */
Java_com_salton123_writer_MmapWriter_create(
        JNIEnv *env, jobject thiz,
        jstring save_path, jint capacity,
        jboolean compress) {
    const char *savePath = env->GetStringUTFChars(save_path, 0);;
    const char *bufferPrefix = "_buf";
    char *bufferPath = (char *) malloc((strlen(savePath) + strlen(bufferPrefix)) * sizeof(char));
    strcpy(bufferPath, savePath);
    strcat(bufferPath, bufferPrefix);
    size_t bufferSize = static_cast<size_t>(capacity);
    /**
     * O_RDONLY 只读打开。
     * O_WRONLY 只写打开。
     * O_RDWR 读、写打开。
     * O_APPEND 每次写时都加到文件的尾端。
     * O_CREAT 若此文件不存在则创建它。使用此选择项时，需同时说明第三个参数mode，用其说明该新文件的存取许可权位。
     * O_EXCL 如果同时指定了O_CREAT，而文件已经存在，则出错。这可测试一个文件是否存在，如果不存在则创建此文件成为一个原子操作。
     * O_TRUNC 如果此文件存在，而且为只读或只写成功打开，则将其长度截短为0。
     * O_NOCTTY 如果p a t h n a m e指的是终端设备，则不将此设备分配作为此进程的控制终端。
     * O_NONBLOCK 如果p a t h n a m e指的是一个F I F O、一个块特殊文件或一个字符特殊文件，则此选择项为此文件的本次打开操作和后续的I / O操作设置非阻塞方式。
     * O_SYNC 使每次w r i t e都等到物理I / O操作完成。
     * S_IRUSR：用户读权限
     * S_IWUSR：用户写权限
     * S_IRGRP：用户组读权限
     * S_IWGRP：用户组写权限
     * S_IROTH：其他组都权限
     * S_IWOTH：其他组写权限
     */
    int bufferFileDescription = open(bufferPath, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    // buffer 的第一个字节会用于存储日志路径名称长度，后面紧跟日志路径，之后才是日志信息
    bufferSize = bufferSize + calculateHeaderLength(strlen(savePath));
    char *mMap = openMmap(bufferFileDescription, bufferSize);
    bool enableMmap = true;
    //如果打开 mmap 失败，则降级使用内存缓存
    if (mMap == nullptr) {
        mMap = new char[bufferSize];
        enableMmap = false;
    }
    if (fileFlush == nullptr) {
        fileFlush = new FileDifferential();
    }
    FileBuffer *logBuffer = new FileBuffer(mMap, bufferSize);
    logBuffer->setAsyncFileFlush(fileFlush);
    //将buffer内的数据清0， 并写入日志文件路径
    logBuffer->init((char *) savePath, strlen(savePath), compress);
    logBuffer->enableMmap = enableMmap;
    env->ReleaseStringUTFChars(save_path, savePath);
    return reinterpret_cast<long>(logBuffer);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_salton123_writer_MmapWriter_write(JNIEnv *env, jobject thiz, jlong buffer_pointer_, jstring info_) {
    const char *info = env->GetStringUTFChars(info_, 0);
    jsize infoLength = env->GetStringUTFLength(info_);
    FileBuffer *logBuffer = reinterpret_cast<FileBuffer *>(buffer_pointer_);
    if (infoLength >= logBuffer->emptySize()) {
        logBuffer->asyncFlush(fileFlush, nullptr);
    }
    logBuffer->append(info, infoLength);
    env->ReleaseStringUTFChars(info_, info);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_salton123_writer_MmapWriter_flush(JNIEnv *env, jobject thiz, jlong buffer_pointer) {
    flushInfo(buffer_pointer);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_salton123_writer_MmapWriter_destory(JNIEnv *env, jobject thiz, jlong buffer_pointer) {
    flushInfo(buffer_pointer);
    if (fileFlush != nullptr) {
        delete fileFlush;
    }
    fileFlush = nullptr;
}



