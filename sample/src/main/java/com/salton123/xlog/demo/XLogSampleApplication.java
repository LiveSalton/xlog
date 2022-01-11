package com.salton123.xlog.demo;

import android.app.Application;
import android.util.Log;

import com.salton123.log.Utils;
import com.salton123.log.XLog;
import com.salton123.log.XLogConfig;

import kotlin.Unit;
import kotlin.jvm.functions.Function2;

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
            .setLogDefaultSplitSize(5)  //split log size , unit m
            .setSavePath(Utils.getDefaultPath() + getPackageName()) //log save path
            .setLogDeleteDelayDay(10)   //log delete delay day
            .setCallback(new Function2<Integer, String, Unit>() {
                @Override
                public Unit invoke(Integer integer, String s) {
                    Log.i("log", s);
                    return null;
                }
            })
        );
    }
}
