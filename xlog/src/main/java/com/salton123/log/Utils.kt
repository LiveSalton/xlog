package com.salton123.log

import android.os.Environment
import java.io.File
import java.lang.StringBuilder
import com.salton123.log.Utils
import android.text.TextUtils

/**
 * User: newSalton@outlook.com
 * Date: 2019/4/26 10:18 PM
 * ModifyTime: 10:18 PM
 * Description:
 */
object Utils {
    /**
     * Returns the default path for saving logs
     */
    @JvmStatic
    val defaultPath: String
        get() = Environment.getExternalStorageDirectory()
            .toString() + File.separator + "xlog" + File.separator

    /**
     * Check whether sdcard is availale
     */
    @JvmStatic
    fun checkSDCardAvailable(): Boolean {
        return Environment.getExternalStorageState() == Environment.MEDIA_MOUNTED
    }

    /**
     * Returns the line number of the method caller
     */
    @JvmStatic
    val callerLineNumber: Int
        get() = Thread.currentThread().stackTrace[4].lineNumber

    /**
     * Returns the filename of the method caller
     */
    @JvmStatic
    val callerFilename: String
        get() = Thread.currentThread().stackTrace[4].fileName

    /**
     * Returns the method name of the caller
     */
    @JvmStatic
    val callerMethodName: String
        get() = Thread.currentThread().stackTrace[4].methodName

    /**
     * Returns the format log String which prepare for save in file
     */
    @JvmStatic
    fun msgForTextLog(
        obj: Any?, filename: String?,
        line: Int, msg: String?,
        methodName: String?
    ): String {
        val sb = StringBuilder(msg)
        // sb.append("(P:");
        // sb.append(Process.myPid());
        // sb.append(")");
        // sb.append("(T:");
        // sb.append(Thread.currentThread().getId());
        // sb.append(")");
        // sb.append("(C:");
        // sb.append(objClassName(obj));
        // sb.append(")");
        sb.append(" (M:")
        sb.append(methodName)
        sb.append(")")
        sb.append(" (")
        sb.append(filename)
        sb.append(":")
        sb.append(line)
        sb.append(")")
        return sb.toString()
    }

    /**
     * Returns the obj`s classname
     */
    @JvmStatic
    fun objClassName(obj: Any): String {
        return if (obj is String) {
            obj
        } else {
            obj.javaClass.simpleName
        }
    }

    fun makeDirs(filePath: String): Boolean {
        val folderName = getFolderName(filePath)
        if (isEmpty(folderName)) {
            return false
        }
        val folder = File(folderName)
        return if (folder.exists() && folder.isDirectory) true else folder.mkdirs()
    }

    fun isEmpty(str: CharSequence?): Boolean {
        return str == null || str.length == 0
    }

    /**
     * get folder name from path
     *
     * @param filePath 文件路径
     * @return 文件夹名称
     */
    fun getFolderName(filePath: String): String {
        if (TextUtils.isEmpty(filePath)) {
            return filePath
        }
        val filePosi = filePath.lastIndexOf(File.separator)
        return if (filePosi == -1) "" else filePath.substring(0, filePosi)
    }
}