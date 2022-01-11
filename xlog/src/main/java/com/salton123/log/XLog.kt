package com.salton123.log

import android.util.Log
import com.salton123.log.printer.FilePrinter

/**
 * Time:2022/1/11 15:54
 * Author:
 * Description:
 */
object XLog {
    private var sConfig = XLogConfig()
    private var mFilePrinter = FilePrinter(sConfig)
    @JvmStatic
    fun config(config: XLogConfig) {
        sConfig = config
        mFilePrinter = FilePrinter(sConfig)
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
            Log.v(Utils.objClassName(tag), logText)
        }
        saveLog(logText)
    }

    @JvmStatic
    fun d(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName)
        if (sConfig.isDebugable) {
            Log.d(Utils.objClassName(tag), logText)
        }
        saveLog(logText)
    }

    @JvmStatic
    fun i(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName)
        if (sConfig.isDebugable) {
            Log.i(Utils.objClassName(tag), logText)
        }
        saveLog(logText)
    }

    @JvmStatic
    fun w(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName)
        if (sConfig.isDebugable) {
            Log.w(Utils.objClassName(tag), logText)
        }
        saveLog(logText)
    }

    @JvmStatic
    fun e(tag: Any, msg: String) {
        val line = Utils.callerLineNumber
        val fileName = Utils.callerFilename
        val methodName = Utils.callerMethodName
        val logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName)
        if (sConfig.isDebugable) {
            Log.e(Utils.objClassName(tag), logText)
        }
        saveLog(logText)
    }

    private fun saveLog(msg: String) {
        if (sConfig.whetherToSaveLog) {
            mFilePrinter.println(msg)
        }
    }
}