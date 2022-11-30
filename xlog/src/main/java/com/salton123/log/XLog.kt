package com.salton123.log

import android.util.Log
import com.tencent.mars.xlog.Xlog

/**
 * Time:2022/1/11 15:54
 * Author:
 * Description:
 */
object XLog {
    private var sConfig = LogConfig()

    init {
        try {
            System.loadLibrary("c++_shared")
            System.loadLibrary("marsxlog")
        } catch (throwable: Throwable) {
            throwable.printStackTrace()
        }
    }

    var mmapPointer: Long = 0
    val xlog = Xlog()

    @JvmStatic
    fun config(config: LogConfig) {
        sConfig = config
        val logConfig = Xlog.XLogConfig()
        logConfig.level = Xlog.LEVEL_ALL
        logConfig.mode = Xlog.AppednerModeAsync
        logConfig.logdir = config.savePath
        logConfig.nameprefix = config.prefix
        logConfig.pubkey = ""
        logConfig.compressmode = Xlog.ZLIB_MODE
        logConfig.compresslevel = 0
        logConfig.cachedir = config.savePath
        logConfig.cachedays = config.logDeleteDelayDay
        mmapPointer = xlog.newXlogInstance(logConfig)
    }

    /**
     * print log
     */
    @JvmStatic
    fun v(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName)
        if (sConfig.isDebugable) {
            Log.v(objClassName(tag), logText)
        }
        Xlog.logWrite2(mmapPointer, Xlog.LEVEL_VERBOSE, objClassName(tag), fileName, methodName, line, 0, 0, 0, msg)
    }

    @JvmStatic
    fun d(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName)
        if (sConfig.isDebugable) {
            Log.d(objClassName(tag), logText)
        }
        Xlog.logWrite2(mmapPointer, Xlog.LEVEL_DEBUG, objClassName(tag), fileName, methodName, line, 0, 0, 0, msg)
    }

    @JvmStatic
    fun i(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName)
        if (sConfig.isDebugable) {
            Log.i(objClassName(tag), logText)
        }
        Xlog.logWrite2(mmapPointer, Xlog.LEVEL_INFO, objClassName(tag), fileName, methodName, line, 0, 0, 0, msg)
    }

    @JvmStatic
    fun w(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName)
        if (sConfig.isDebugable) {
            Log.w(objClassName(tag), logText)
        }
        Xlog.logWrite2(mmapPointer, Xlog.LEVEL_WARNING, objClassName(tag), fileName, methodName, line, 0, 0, 0, msg)
    }

    @JvmStatic
    fun e(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName)
        if (sConfig.isDebugable) {
            Log.e(objClassName(tag), logText)
        }
        Xlog.logWrite2(mmapPointer, Xlog.LEVEL_ERROR, objClassName(tag), fileName, methodName, line, 0, 0, 0, msg)
    }

    /**
     * Returns the obj`s classname
     */
    private fun objClassName(obj: Any): String? {
        return if (obj is String) {
            sConfig.prefix + "-" + obj
        } else {
            sConfig.prefix + "-" + obj.javaClass.toString()
        }
    }
}