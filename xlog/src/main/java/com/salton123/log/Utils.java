package com.salton123.log;

import android.os.Environment;

import java.io.File;

/**
 * User: newSalton@outlook.com
 * Date: 2019/4/26 10:18 PM
 * ModifyTime: 10:18 PM
 * Description:
 */
public class Utils {

    /**
     * Returns the default path for saving logs
     */
    public static String getDefaultPath() {
        return Environment.getExternalStorageDirectory()
                + File.separator + "xlog" + File.separator;
    }

    /**
     * Check whether sdcard is availale
     */
    public static boolean checkSDCardAvailable() {
        return Environment.getExternalStorageState()
                .equals(Environment.MEDIA_MOUNTED);
    }

    /**
     * Returns the line number of the method caller
     */
    public static int getCallerLineNumber() {
        return Thread.currentThread().getStackTrace()[4].getLineNumber();
    }

    /**
     * Returns the filename of the method caller
     */
    public static String getCallerFilename() {
        return Thread.currentThread().getStackTrace()[4].getFileName();
    }

    /**
     * Returns the method name of the caller
     */
    public static String getCallerMethodName() {
        return Thread.currentThread().getStackTrace()[4].getMethodName();
    }

    /**
     * Returns the format log String which prepare for save in file
     */
    public static String msgForTextLog(
            Object obj, String filename,
            int line, String msg,
            String methodName) {
        StringBuilder sb = new StringBuilder(msg);
        // sb.append("(P:");
        // sb.append(Process.myPid());
        // sb.append(")");
        // sb.append("(T:");
        // sb.append(Thread.currentThread().getId());
        // sb.append(")");
        // sb.append("(C:");
        // sb.append(objClassName(obj));
        // sb.append(")");
        sb.append(" (M:");
        sb.append(methodName);
        sb.append(")");
        sb.append(" (");
        sb.append(filename);
        sb.append(":");
        sb.append(line);
        sb.append(")");
        return sb.toString();
    }

    /**
     * Returns the obj`s classname
     */
    public static String objClassName(Object obj) {
        if (obj instanceof String) {
            return (String) obj;
        } else {
            return obj.getClass().getSimpleName();
        }
    }

}
