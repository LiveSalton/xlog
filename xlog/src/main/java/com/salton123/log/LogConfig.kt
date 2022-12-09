package com.salton123.log

/**
 * Time:2022/1/11 15:54
 * Author:
 * Description:
 * @param isDebugable 日志开关
 * @param whetherToSaveLog 是否保存日志
 * @param savePath 保存位置
 * @param logDeleteDelayDay 默认10天后删除
 * @param logDefaultSplitSize 日志拆分大小阈值，每1M拆分
 */
class LogConfig {

    var isDebugable: Boolean = true
        private set
    var savePath: String = ""
        private set
    var logDeleteDelayDay: Int = 10
        private set
    var logDefaultSplitSize: Int = 5
        private set
    var callback: ((code: Int, message: String) -> Unit)? = null
        private set
    var prefix = "salton"
        private set


    fun setDebugable(debugable: Boolean): LogConfig {
        isDebugable = debugable
        return this
    }

    fun setSavePath(savePath: String): LogConfig {
        this.savePath = savePath
        return this
    }

    fun setPrefix(prefix: String): LogConfig {
        this.prefix = prefix
        return this
    }

    fun setLogDeleteDelayDay(logDeleteDelayDay: Int): LogConfig {
        this.logDeleteDelayDay = logDeleteDelayDay
        return this
    }

    fun setLogDefaultSplitSize(logDefaultSplitSize: Int): LogConfig {
        this.logDefaultSplitSize = logDefaultSplitSize
        return this
    }

    fun setCallback(callback: ((code: Int, message: String) -> Unit)?): LogConfig {
        this.callback = callback
        return this
    }
}