package com.salton123.xlog.demo;

import android.app.Application;

import com.salton123.log.Utils;
import com.salton123.log.XLog;
import com.salton123.log.XLogConfig;

import java.io.File;

/**
 * User: newSalton@outlook.com
 * Date: 2018/11/15 11:45 AM
 * ModifyTime: 11:45 AM
 * Description:
 */
public class XLogSampleApplication extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        XLog.config(new XLogConfig()
                .setDebugable(BuildConfig.DEBUG) //whether print info to logcat
                .setWhetherToSaveLog(true)  //whether save log to file
                .setLogDefaultSplitSize(1)  //split log size , unit m
                .setSavePath(Utils.getDefaultPath() + getPackageName()) //log save path
                .setLogDeleteDelayDay(10)   //log delete delay day
        );
    }
}
