// package com.salton123.io;
//
// import android.util.Log;
//
// import java.io.PrintWriter;
// import java.io.StringWriter;
// import java.net.UnknownHostException;
//
//
// public class FlushWriter {
//
//     private static final String TAG = "FlushWriter";
//
//     private long ptr = 0;
//     private String logPath;
//     private String bufferPath;
//     private int bufferSize;
//     private boolean compress;
//
//     public FlushWriter(String bufferPath, int capacity, String logPath, boolean compress) {
//         this.bufferPath = bufferPath;
//         this.bufferSize = capacity;
//         this.logPath = logPath;
//         this.compress = compress;
//         try {
//             ptr = initNative(bufferPath, capacity, logPath, compress);
//         } catch (Exception e) {
//             Log.e(TAG, getStackTraceString(e));
//         }
//     }
//
//     public void changeLogPath(String logPath) {
//         if (ptr != 0) {
//             try {
//                 changeLogPathNative(ptr, logPath);
//                 this.logPath = logPath;
//             } catch (Exception e) {
//                 Log.e(TAG, getStackTraceString(e));
//             }
//         }
//     }
//
//     public boolean isCompress() {
//         return compress;
//     }
//
//     public String getLogPath() {
//         return logPath;
//     }
//
//     public String getBufferPath() {
//         return bufferPath;
//     }
//
//     public int getBufferSize() {
//         return bufferSize;
//     }
//
//     public void write(String log) {
//         if (ptr != 0) {
//             try {
//                 writeNative(ptr, log);
//             } catch (Exception e) {
//                 Log.e(TAG, getStackTraceString(e));
//             }
//         }
//     }
//
//     public void flushAsync() {
//         if (ptr != 0) {
//             try {
//                 flushAsyncNative(ptr);
//             } catch (Exception e) {
//                 Log.e(TAG, getStackTraceString(e));
//             }
//         }
//     }
//
//     public void release() {
//         if (ptr != 0) {
//             try {
//                 releaseNative(ptr);
//             } catch (Exception e) {
//                 Log.e(TAG, getStackTraceString(e));
//             }
//             ptr = 0;
//         }
//     }
//
//     public static String getStackTraceString(Throwable tr) {
//         if (tr == null) {
//             return "";
//         }
//         Throwable t = tr;
//         while (t != null) {
//             if (t instanceof UnknownHostException) {
//                 return "";
//             }
//             t = t.getCause();
//         }
//
//         StringWriter sw = new StringWriter();
//         PrintWriter pw = new PrintWriter(sw);
//         tr.printStackTrace(pw);
//         pw.flush();
//         return sw.toString();
//     }
//
//     static {
//         System.loadLibrary("flushwriter");
//     }
//
//     private native static long initNative(String bufferPath, int capacity, String logPath, boolean compress);
//
//     private native void writeNative(long ptr, String log);
//
//     private native void flushAsyncNative(long ptr);
//
//     private native void releaseNative(long ptr);
//
//     private native void changeLogPathNative(long ptr, String logPath);
//
// }
