#include <jni.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>

#include "includes/LogBuffer.h"

static const char *const kClassDocScanner = "com/salton123/io/FlushWriter";

static char *openMMap(int buffer_fd, size_t buffer_size);

static void writeDirtyLogToFile(int buffer_fd);

static AsyncFileFlush *fileFlush = nullptr;

/**
 * 初始化
 * @param env
 * @param type
 * @param buffer_path_ 缓存路径
 * @param capacity 单个文件的缓存大小
 * @param log_path_ log路径
 * @param compress_ 是否进行压缩
 * @return
 */
static jlong initNative(JNIEnv *env, jclass type, jstring buffer_path_,
                        jint capacity, jstring log_path_, jboolean compress_) {
    const char *buffer_path = env->GetStringUTFChars(buffer_path_, 0);
    const char *log_path = env->GetStringUTFChars(log_path_, 0);
    size_t buffer_size = static_cast<size_t>(capacity);
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
    int buffer_fd = open(buffer_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    // buffer 的第一个字节会用于存储日志路径名称长度，后面紧跟日志路径，之后才是日志信息
    if (fileFlush == nullptr) {
        fileFlush = new AsyncFileFlush();
    }
    // 加上头占用的大小
    buffer_size = buffer_size + LogBufferHeader::calculateHeaderLen(strlen(log_path));
    char *buffer_ptr = openMMap(buffer_fd, buffer_size);
    bool map_buffer = true;
    //如果打开 mmap 失败，则降级使用内存缓存
    if (buffer_ptr == nullptr) {
        buffer_ptr = new char[buffer_size];
        map_buffer = false;
    }
    LogBuffer *logBuffer = new LogBuffer(buffer_ptr, buffer_size);
    logBuffer->setAsyncFileFlush(fileFlush);
    //将buffer内的数据清0， 并写入日志文件路径
    logBuffer->initData((char *) log_path, strlen(log_path), compress_);
    logBuffer->map_buffer = map_buffer;

    env->ReleaseStringUTFChars(buffer_path_, buffer_path);
    env->ReleaseStringUTFChars(log_path_, log_path);
    return reinterpret_cast<long>(logBuffer);
}

static char *openMMap(int buffer_fd, size_t buffer_size) {
    char *map_ptr = nullptr;
    if (buffer_fd != -1) {
        // 写脏数据
        writeDirtyLogToFile(buffer_fd);
        // 根据 buffer size 调整 buffer 文件大小
        /**
         * ftruncate()会将参数fd 指定的文件大小改为参数length 指定的大小。
         * 参数fd 为已打开的文件描述词，而且必须是以写入模式打开的文件。
         * 如果原来的文件大小比参数length 大，则超过的部分会被删去。
         */
        ftruncate(buffer_fd, static_cast<int>(buffer_size));
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
        lseek(buffer_fd, 0, SEEK_SET);
        map_ptr = (char *) mmap(0, buffer_size, PROT_WRITE | PROT_READ, MAP_SHARED, buffer_fd, 0);
        if (map_ptr == MAP_FAILED) {
            map_ptr = nullptr;
        }
    }
    return map_ptr;
}

static void writeDirtyLogToFile(int buffer_fd) {
    struct stat fileInfo;
    /**
     *  fstat 用来将参数buffer_fd所指的文件状态，复制到参数fileInfo所指的结构中(struct stat)。
     */
    if (fstat(buffer_fd, &fileInfo) >= 0) {
        size_t buffered_size = static_cast<size_t>(fileInfo.st_size);
        // buffer_size 必须是大于文件头长度的，否则会导致下标溢出
        if (buffered_size > LogBufferHeader::calculateHeaderLen(0)) {
            char *buffer_ptr_tmp = (char *) mmap(0, buffered_size, PROT_WRITE | PROT_READ, MAP_SHARED, buffer_fd, 0);
            if (buffer_ptr_tmp != MAP_FAILED) {
                LogBuffer *tmp = new LogBuffer(buffer_ptr_tmp, buffered_size);
                size_t data_size = tmp->length();
                if (data_size > 0) {
                    tmp->async_flush(fileFlush, tmp);
                } else {
                    delete tmp;
                }
            }
        }
    }
}

static void writeNative(JNIEnv *env, jobject instance, jlong ptr,
                        jstring log_) {
    const char *log = env->GetStringUTFChars(log_, 0);
    jsize log_len = env->GetStringUTFLength(log_);
    LogBuffer *logBuffer = reinterpret_cast<LogBuffer *>(ptr);
    // 缓存写不下时异步刷新
    if (log_len >= logBuffer->emptySize()) {
        logBuffer->async_flush(fileFlush);
    }
    logBuffer->append(log, (size_t) log_len);
    env->ReleaseStringUTFChars(log_, log);
}

static void releaseNative(JNIEnv *env, jobject instance, jlong ptr) {
    LogBuffer *logBuffer = reinterpret_cast<LogBuffer *>(ptr);
    logBuffer->async_flush(fileFlush, logBuffer);
    if (fileFlush != nullptr) {
        delete fileFlush;
    }
    fileFlush = nullptr;
}

static void changeLogPathNative(JNIEnv *env, jobject instance, jlong ptr,
                                jstring logFilePath) {
    const char *log_path = env->GetStringUTFChars(logFilePath, 0);
    LogBuffer *logBuffer = reinterpret_cast<LogBuffer *>(ptr);
    logBuffer->changeLogPath(const_cast<char *>(log_path));
    env->ReleaseStringUTFChars(logFilePath, log_path);
}

static void flushAsyncNative(JNIEnv *env, jobject instance, jlong ptr) {
    LogBuffer *logBuffer = reinterpret_cast<LogBuffer *>(ptr);
    logBuffer->async_flush(fileFlush);
}

static JNINativeMethod gMethods[] = {

        {
                "initNative",
                "(Ljava/lang/String;ILjava/lang/String;Z)J",
                (void *) initNative
        },

        {
                "writeNative",
                "(JLjava/lang/String;)V",
                (void *) writeNative
        },

        {
                "flushAsyncNative",
                "(J)V",
                (void *) flushAsyncNative
        },

        {
                "changeLogPathNative",
                "(JLjava/lang/String;)V",
                (void *) changeLogPathNative
        },

        {
                "releaseNative",
                "(J)V",
                (void *) releaseNative
        }

};

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return JNI_FALSE;
    }
    jclass classDocScanner = env->FindClass(kClassDocScanner);
    if (env->RegisterNatives(classDocScanner, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return JNI_FALSE;
    }
    return JNI_VERSION_1_4;
}