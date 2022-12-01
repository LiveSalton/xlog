package com.salton123.log.printer

import android.util.Log
import com.salton123.log.XLogConfig
import com.salton123.writer.MmapWriter
import java.io.File
import java.text.SimpleDateFormat
import java.util.*

/**
 * Time:2022/1/11 15:59
 * Author:
 * Description:
 */
class FilePrinter(val config: XLogConfig) : Printer {
    private val fileName = "xlog.txt"
    private val bakExt = ".bak"
    private val tag = "xlog.FilePrinter"
    private var saveFolder: String = config.savePath
    private var logDefaultSplitSize: Int = config.logDefaultSplitSize
    private var logDeleteDelayDay: Int = config.logDeleteDelayDay * 24 * 60 * 60 * 1000
    private var writer: Writer = Writer()

    init {
        writer.open(saveFolder, fileName)
    }

    override fun println(msg: String) {
        val saveLogFilePath: String = config.savePath + File.separator + fileName
        val logFile = File(saveLogFilePath)
        if (logFile.exists()) {
            val fileLength = logFile.length()
            val fileSize = fileLength ushr 20 //转为m
            Log.d(tag, "fileLength:$fileLength,fileSize:$fileSize,logDefaultSplitSize:$logDefaultSplitSize")
            if (fileSize >= logDefaultSplitSize) {
                //删除大于10天的文件
                deleteOldLogs()
                if (writer.isOpened()) {
                    writer.close()
                }
                val simpleDateFormat = SimpleDateFormat("-MM-dd-kk-mm-ss")
                val fileExt = simpleDateFormat.format(Date())
                val sb: StringBuilder = StringBuilder(saveFolder)
                sb.append(File.separator).append(fileName).append(fileExt)
                    .append(bakExt)
                val fileNameTo = File(sb.toString())
                logFile.renameTo(fileNameTo)
                writer.open(saveFolder, fileName)
            }
        } else {
            config.callback?.invoke(-5, "file not exist:$saveLogFilePath")
        }
        var strLog = SimpleDateFormat("yyyy:MM:dd kk:mm:ss.SSS").format(Date())
        val sb = StringBuffer(strLog)
        sb.append(' ')
        sb.append(msg)
        sb.append('\n')
        strLog = sb.toString()
        if (!writer.isOpened()) {
            writer.open(saveFolder, fileName)
        }
        writer.append(strLog)
    }

    private fun deleteOldLogs() {
        val dirFile = File(saveFolder)
        if (!dirFile.exists()) {
            return
        }
        val now = System.currentTimeMillis()
        val files: Array<File> = dirFile.listFiles()
        if (files.isNotEmpty()) {
            for (file in files) {
                if (file.name.endsWith(bakExt)) {
                    val lastModifiedTime = file.lastModified()
                    if (now - lastModifiedTime > logDeleteDelayDay) {
                        file.delete()
                    }
                }
            }
        }
    }

    inner class Writer {
        lateinit var file: File
        var mmapPointer: Long = 0L

        fun open(folderPath: String, fileName: String): Boolean {
            file = File(folderPath, fileName);
            if (!file.exists()) {
                try {
                    file.parentFile?.apply {
                        if (!exists()) {
                            mkdirs()
                        }
                    }
                    file.createNewFile();
                } catch (ex: Exception) {
                    ex.printStackTrace()
                    config.callback?.invoke(-1, ex.message.toString())
                    return false
                }
            }
            try {
                mmapPointer = MmapWriter.createInstance(file.absolutePath, 8192 * 2, file.absolutePath, false)
            } catch (ex: Exception) {
                ex.printStackTrace()
                config.callback?.invoke(-2, ex.message.toString())
                return false
            }
            return true
        }

        fun close() {
            try {
                MmapWriter.releaseInstance(mmapPointer)
                mmapPointer = 0L
            } catch (ex: Exception) {
                ex.printStackTrace()
                config.callback?.invoke(-4, ex.message.toString())
            }
        }

        fun append(logInfo: String) {
            try {
                MmapWriter.writeInfo(mmapPointer, logInfo)
                MmapWriter.flushInfo(mmapPointer)
            } catch (ex: Exception) {
                ex.printStackTrace()
                config.callback?.invoke(-4, ex.message.toString())
            }
        }

        fun isOpened(): Boolean {
            return mmapPointer != 0L
        }
    }
}