#include <jni.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>

#include "includes/MetaData.h"
#include "MmapWriter.h"

static const char *const kClassDocScanner = "com/salton123/writer/MmapWriter";

static FileFlusher *fileFlush = nullptr;

static jlong createInstance(JNIEnv *env, jclass type, jstring save_path, jint capacity, jboolean compress_) {

    const char *savePath = env->GetStringUTFChars(save_path, 0);
    const char *bufferPrefix = "_buf";
    char *buffer_path = (char *) malloc((strlen(savePath) + strlen(bufferPrefix)) * sizeof(char));
    strcpy(buffer_path, savePath);
    strcat(buffer_path, bufferPrefix);
    size_t buffer_size = static_cast<size_t>(capacity);
    int buffer_fd = open(buffer_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    // buffer 的第一个字节会用于存储日志路径名称长度，后面紧跟日志路径，之后才是日志信息
    if (fileFlush == nullptr) {
        fileFlush = new FileFlusher();
    }
    // 加上头占用的大小
    buffer_size = buffer_size + MmapTrigger::calculateHeaderLen(strlen(savePath));
    char *buffer_ptr = openMMap(buffer_fd, buffer_size);
    bool map_buffer = true;
    //如果打开 mmap 失败，则降级使用内存缓存
    if (buffer_ptr == nullptr) {
        buffer_ptr = new char[buffer_size];
        map_buffer = false;
    }
    MetaData *logBuffer = new MetaData(buffer_ptr, buffer_size);
    logBuffer->setAsyncFileFlush(fileFlush);
    //将buffer内的数据清0， 并写入日志文件路径
    logBuffer->initData((char *) savePath, strlen(savePath), compress_);
    logBuffer->map_buffer = map_buffer;

    env->ReleaseStringUTFChars(save_path, savePath);
    return reinterpret_cast<long>(logBuffer);
}

static char *openMMap(int buffer_fd, size_t buffer_size) {
    char *map_ptr = nullptr;
    if (buffer_fd != -1) {
        // 写脏数据
        writeDirtyLogToFile(buffer_fd);
        // 根据 buffer size 调整 buffer 文件大小
        ftruncate(buffer_fd, static_cast<int>(buffer_size));
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
    if (fstat(buffer_fd, &fileInfo) >= 0) {
        size_t buffered_size = static_cast<size_t>(fileInfo.st_size);
        // buffer_size 必须是大于文件头长度的，否则会导致下标溢出
        if (buffered_size > MmapTrigger::calculateHeaderLen(0)) {
            char *buffer_ptr_tmp = (char *) mmap(0, buffered_size, PROT_WRITE | PROT_READ, MAP_SHARED, buffer_fd, 0);
            if (buffer_ptr_tmp != MAP_FAILED) {
                MetaData *tmp = new MetaData(buffer_ptr_tmp, buffered_size);
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

static void writeInfo(JNIEnv *env, jobject instance, jlong ptr,
                      jstring log_) {
    const char *log = env->GetStringUTFChars(log_, 0);
    jsize log_len = env->GetStringUTFLength(log_);
    MetaData *logBuffer = reinterpret_cast<MetaData *>(ptr);
    // 缓存写不下时异步刷新
    if (log_len >= logBuffer->emptySize()) {
        logBuffer->async_flush(fileFlush);
    }
    logBuffer->append(log, (size_t) log_len);
    env->ReleaseStringUTFChars(log_, log);
}

static void releaseInstance(JNIEnv *env, jobject instance, jlong ptr) {
    MetaData *logBuffer = reinterpret_cast<MetaData *>(ptr);
    logBuffer->async_flush(fileFlush, logBuffer);
    if (fileFlush != nullptr) {
        delete fileFlush;
    }
    fileFlush = nullptr;
}

static void changePath(JNIEnv *env, jobject instance, jlong ptr,
                       jstring logFilePath) {
    const char *log_path = env->GetStringUTFChars(logFilePath, 0);
    MetaData *logBuffer = reinterpret_cast<MetaData *>(ptr);
    logBuffer->changeLogPath(const_cast<char *>(log_path));
    env->ReleaseStringUTFChars(logFilePath, log_path);
}

static void flushInfo(JNIEnv *env, jobject instance, jlong ptr) {
    MetaData *logBuffer = reinterpret_cast<MetaData *>(ptr);
    logBuffer->async_flush(fileFlush);
}

//static jlong createInstance(JNIEnv *env, jclass type, jstring save_path, jint capacity, jboolean compress_) {
static JNINativeMethod gMethods[] = {

        {
                "createInstance",
                "(Ljava/lang/String;IZ)J",
                (void *) createInstance
        },

        {
                "writeInfo",
                "(JLjava/lang/String;)V",
                (void *) writeInfo
        },

        {
                "flushInfo",
                "(J)V",
                (void *) flushInfo
        },

        {
                "changePath",
                "(JLjava/lang/String;)V",
                (void *) changePath
        },

        {
                "releaseInstance",
                "(J)V",
                (void *) releaseInstance
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