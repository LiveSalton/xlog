#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jlong JNICALL
Java_com_salton123_writer_MmapWriter_create(
        JNIEnv *env, jobject thiz,
        jstring save_path, jint capacity,
        jboolean compress) {
}

extern "C"
JNIEXPORT void JNICALL
Java_com_salton123_writer_MmapWriter_write(JNIEnv *env, jobject thiz, jlong buffer_pointer, jstring info) {

}

extern "C"
JNIEXPORT void JNICALL
Java_com_salton123_writer_MmapWriter_flush(JNIEnv *env, jobject thiz, jlong buffer_pointer) {

}

extern "C"
JNIEXPORT void JNICALL
Java_com_salton123_writer_MmapWriter_destory(JNIEnv *env, jobject thiz, jlong buffer_pointer) {

}
