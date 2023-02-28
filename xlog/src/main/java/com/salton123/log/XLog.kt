package com.salton123.log

import android.util.Log
import com.tencent.mars.xlog.Xlog

/**
 * Time:2022/1/11 15:54
 * Author:
 * Description:
 */
object XLog {
    open var sConfig = LogConfig()

    init {
        try {
            System.loadLibrary("c++_shared")
            System.loadLibrary("marsxlog")
        } catch (throwable: Throwable) {
            throwable.printStackTrace()
        }
    }

    @JvmStatic
    fun config(config: LogConfig) {
        sConfig = config
        var appendMode = Xlog.AppednerModeAsync
        var level = Xlog.LEVEL_ALL
        if (sConfig.isDebugable) {
            Xlog.setConsoleLogOpen(true)
            appendMode = Xlog.AppednerModeSync
            level = Xlog.LEVEL_INFO
        }
        Xlog.setMaxFileSize(maxFileSize())
        Xlog.setMaxAliveTime(maxAliveTime())
        Xlog.appenderOpen(
            level, appendMode,
            sConfig.savePath, sConfig.savePath,
            sConfig.prefix, sConfig.logDeleteDelayDay, ""
        )
        Log.i("salton", "init xlog:$sConfig")
    }

    fun maxFileSize(): Long {
        return (sConfig.logDefaultSplitSize * 1024 * 1024).toLong()
    }

    fun maxAliveTime(): Long {
        return (sConfig.logDeleteDelayDay * 24 * 60 * 60).toLong()
    }

    /**
     * print log
     */
    @JvmStatic
    fun v(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(objClassName(tag), fileName, line, msg, methodName)
        try {
//            Xlog.logWrite2(Xlog.LEVEL_VERBOSE, "", "", "", 0, 0, 0, 0, logText)
            Xlog.logWrite2(Xlog.LEVEL_VERBOSE, objClassName(tag), fileName, methodName, line, 0, 0, 0, msg)
        } catch (throwable: Throwable) {
            throwable.printStackTrace()
        }
    }

    @JvmStatic
    fun d(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(objClassName(tag), fileName, line, msg, methodName)
        try {
//            Xlog.logWrite2(Xlog.LEVEL_DEBUG, "", "", "", 0, 0, 0, 0, logText)
            Xlog.logWrite2(Xlog.LEVEL_DEBUG, objClassName(tag), fileName, methodName, line, 0, 0, 0, msg)
        } catch (throwable: Throwable) {
            throwable.printStackTrace()
        }
    }

    @JvmStatic
    fun i(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(objClassName(tag), fileName, line, msg, methodName)
        try {
//            Xlog.logWrite2(Xlog.LEVEL_INFO, "", "", "", 0, 0, 0, 0, logText)
            Xlog.logWrite2(Xlog.LEVEL_INFO, objClassName(tag), fileName, methodName, line, 0, 0, 0, msg)
        } catch (throwable: Throwable) {
            throwable.printStackTrace()
        }
    }

    @JvmStatic
    fun w(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(objClassName(tag), fileName, line, msg, methodName)
        try {
//            Xlog.logWrite2(Xlog.LEVEL_WARNING, "", "", "", 0, 0, 0, 0, logText)
            Xlog.logWrite2(Xlog.LEVEL_WARNING, objClassName(tag), fileName, methodName, line, 0, 0, 0, msg)
        } catch (throwable: Throwable) {
            throwable.printStackTrace()
        }
    }

    @JvmStatic
    fun e(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(objClassName(tag), fileName, line, msg, methodName)
        try {
//            Xlog.logWrite2(Xlog.LEVEL_ERROR, "", "", "", 0, 0, 0, 0, logText)
            Xlog.logWrite2(Xlog.LEVEL_ERROR, objClassName(tag), fileName, methodName, line, 0, 0, 0, msg)
        } catch (throwable: Throwable) {
            throwable.printStackTrace()
        }
    }

    /**
     * Returns the obj`s classname
     */
    private fun objClassName(obj: Any): String {
        return if (obj is String) {
            sConfig.prefix + "-" + obj
        } else {
            sConfig.prefix + "-" + obj.javaClass.toString()
        }
    }
}