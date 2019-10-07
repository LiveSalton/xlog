package com.salton123.log;

import android.util.Log;

/**
 * User: newSalton@outlook.com
 * Date: 2019/4/26 10:12 PM
 * ModifyTime: 10:12 PM
 * Description:
 */
public class XLog {
    private static XLogConfig sConfig = new XLogConfig();

    public static void config(XLogConfig config) {
        sConfig = config;
    }

    /**
     * print log
     */
    public static void v(Object tag, String msg) {
        int line = Utils.getCallerLineNumber();
        String fileName = Utils.getCallerFilename();
        String methodName = Utils.getCallerMethodName();
        String logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName);
        if (sConfig.isDebugable()) {
            Log.v(Utils.objClassName(tag), logText);
        }
        saveLog(logText);
    }

    public static void d(Object tag, String msg) {
        int line = Utils.getCallerLineNumber();
        String fileName = Utils.getCallerFilename();
        String methodName = Utils.getCallerMethodName();
        String logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName);
        if (sConfig.isDebugable()) {
            Log.d(Utils.objClassName(tag), logText);
        }
        saveLog(logText);
    }

    public static void i(Object tag, String msg) {
        int line = Utils.getCallerLineNumber();
        String fileName = Utils.getCallerFilename();
        String methodName = Utils.getCallerMethodName();
        String logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName);
        if (sConfig.isDebugable()) {
            Log.i(Utils.objClassName(tag), logText);
        }
        saveLog(logText);
    }

    public static void w(Object tag, String msg) {
        int line = Utils.getCallerLineNumber();
        String fileName = Utils.getCallerFilename();
        String methodName = Utils.getCallerMethodName();
        String logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName);
        if (sConfig.isDebugable()) {
            Log.w(Utils.objClassName(tag), logText);
        }
        saveLog(logText);
    }

    public static void e(Object tag, String msg) {
        int line = Utils.getCallerLineNumber();
        String fileName = Utils.getCallerFilename();
        String methodName = Utils.getCallerMethodName();
        String logText = Utils.msgForTextLog(tag, fileName, line, msg, methodName);
        if (sConfig.isDebugable()) {
            Log.e(Utils.objClassName(tag), logText);
        }
        saveLog(logText);
    }

    private static void saveLog(String msg) {
        if (sConfig.isWhetherToSaveLog()) {

        }
    }
}
