package com.salton123.log.printer;

import android.util.Log;

import com.salton123.io.FlushWriter;
import com.salton123.log.Utils;
import com.salton123.log.XLogConfig;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * User: newSalton@outlook.com
 * Date: 2019/4/26 10:47 PM
 * ModifyTime: 10:47 PM
 * Description:
 */
public class FilePrinter implements Printer {
    private static final String TAG = "xlog.FilePrinter";
    private String fileName = "xlog.txt";
    private String bakExt = ".bak";
    private Writer mWriter = new Writer();
    private String saveLogFilePath;
    private String path;
    private long logDefaultSplitSize;
    private long logDeleteDelayDay;

    public FilePrinter(XLogConfig config) {
        saveLogFilePath = config.getSavePath() + File.separator + fileName;
        path = config.getSavePath();
        logDefaultSplitSize = config.getLogDefaultSplitSize();
        if (logDefaultSplitSize == 0) {
            logDefaultSplitSize = 1;
        }
        logDeleteDelayDay = config.getLogDeleteDelayDay() * 24 * 60 * 60 * 1000;
        mWriter.open(path, fileName);
    }

    @Override
    public void println(int logLevel, String tag, String msg) {
        if (!Utils.checkSDCardAvailable()) {
            Log.e(TAG, "SDCard Not Available");
            return;
        }

        File dirFile = new File(path);
        if (!dirFile.exists()) {
            dirFile.mkdirs();
        }
        File logFile = new File(saveLogFilePath);
        if (!logFile.exists()) {
            try {
                logFile.createNewFile();
            } catch (Exception e) {
                e.printStackTrace();
                return;
            }
        } else {
            long fileLength = logFile.length();
            long fileSize = fileLength >>> 20;    //转为m
            Log.i(TAG, "fileLength:" + fileLength + ",fileSize:"
                    + fileSize + ",logDefaultSplitSize:" + logDefaultSplitSize);

            if (fileSize >= logDefaultSplitSize) {
                //删除大于10天的文件
                deleteOldLogs();
                if (mWriter.isOpened()) {
                    mWriter.close();
                }
                SimpleDateFormat simpleDateFormate = new SimpleDateFormat("-MM-dd-kk-mm-ss");
                String fileExt = simpleDateFormate.format(new Date());
                StringBuilder sb = new StringBuilder(path);
                sb.append(File.separator).append(fileName).append(fileExt)
                        .append(bakExt);
                File fileNameTo = new File(sb.toString());
                logFile.renameTo(fileNameTo);
                mWriter.open(path, fileName);
            }
        }
        String strLog = new SimpleDateFormat("yyyy:MM:dd kk:mm:ss.SSS").format(new Date());
        StringBuffer sb = new StringBuffer(strLog);
        sb.append(' ');
        sb.append(msg);
        sb.append('\n');
        strLog = sb.toString();
        if (!mWriter.isOpened()) {
            mWriter.open(path, fileName);
        }
        mWriter.appendLog(strLog);
    }

    private void deleteOldLogs() {
        File dirFile = new File(path);
        if (!dirFile.exists()) {
            return;
        }
        long now = System.currentTimeMillis();
        File[] files = dirFile.listFiles();
        if (files != null && files.length > 0) {
            for (File file : files) {
                if (file.getName().endsWith(bakExt)) {
                    long lastModifiedTime = file.lastModified();
                    if (now - lastModifiedTime > logDeleteDelayDay) {
                        file.delete();
                    }
                }
            }
        }
    }

    private static class Writer {
        String lastFileName = null;
        File file = null;
        FlushWriter logBuffer = null;

        public boolean isOpened() {
            return logBuffer != null;
        }

        private boolean open(String folderPath, String newFileName) {
            lastFileName = newFileName;
            file = new File(folderPath, newFileName);
            if (!file.exists()) {
                try {
                    File parent = file.getParentFile();
                    if (!parent.exists()) {
                        parent.mkdirs();
                    }
                    file.createNewFile();
                } catch (Exception e) {
                    e.printStackTrace();
                    lastFileName = null;
                    file = null;
                    return false;
                }
            }
            try {
                logBuffer = new FlushWriter(file.getAbsolutePath() + "_buf",
                        8192 * 2, file.getAbsolutePath(), false);
            } catch (Exception e) {
                e.printStackTrace();
                lastFileName = null;
                file = null;
                return false;
            }
            return true;
        }

        private boolean close() {
            if (logBuffer != null) {
                try {
                    logBuffer.release();
                } catch (Exception e) {
                    e.printStackTrace();
                    return false;
                } finally {
                    logBuffer = null;
                    lastFileName = null;
                    file = null;
                }
            }
            return true;
        }

        private void appendLog(String flattenedLog) {
            try {
                logBuffer.write(flattenedLog);
                logBuffer.flushAsync();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

    }


}
