package com.salton123.writer

/**
 * Time:2021/12/7 5:56 下午
 * Author:
 * Description:
 */
object MmapWriter {
    init {
        System.loadLibrary("mmap-writer")
    }

    /**
     * 创建一个mmap读写对象
     * @param savePath 文件保存路径
     * @param savePath mmap缓存容量，大于容量就会写入到文件中。
     * @param compress 是否用zlib压缩
     */
    external fun create(savePath: String, capacity: Int, compress: Boolean): Long

    /**
     * 写入内容
     */
    external fun write(bufferPointer: Long, info: String)

    /**
     * 异步写入内容到文件中
     */
    external fun flush(bufferPointer: Long)

    /**
     * 销毁mmap对象
     */
    external fun destory(bufferPointer: Long)
}