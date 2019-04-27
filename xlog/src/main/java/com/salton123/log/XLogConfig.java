package com.salton123.log;

/**
 * User: newSalton@outlook.com
 * Date: 2019/4/26 10:37 PM
 * ModifyTime: 10:37 PM
 * Description:
 */
public class XLogConfig {
    private boolean isDebugable = true;  //日志开关
    private String  savePath = Utils.getDefaultPath() ;  //保存位置
    private boolean whetherToSaveLog = false; //是否保存日志
    private int  logDeleteDelayDay = 10 ;  //默认10天后删除
    private int logDefaultSplitSize = 5;  //日志拆分大小阈值，每1M拆分

    public boolean isDebugable() {
        return isDebugable;
    }

    public String getSavePath() {
        return savePath;
    }

    public boolean isWhetherToSaveLog() {
        return whetherToSaveLog;
    }

    public int getLogDeleteDelayDay() {
        return logDeleteDelayDay;
    }

    public int getLogDefaultSplitSize() {
        return logDefaultSplitSize;
    }

    public XLogConfig setDebugable(boolean debugable) {
        isDebugable = debugable;
        return this;
    }

    public XLogConfig setSavePath(String savePath) {
        this.savePath = savePath;
        return this;
    }

    public XLogConfig setWhetherToSaveLog(boolean whetherToSaveLog) {
        this.whetherToSaveLog = whetherToSaveLog;
        return this;
    }

    public XLogConfig setLogDeleteDelayDay(int logDeleteDelayDay) {
        this.logDeleteDelayDay = logDeleteDelayDay;
        return this;
    }

    public XLogConfig setLogDefaultSplitSize(int logDefaultSplitSize) {
        this.logDefaultSplitSize = logDefaultSplitSize;
        return this;
    }
}
