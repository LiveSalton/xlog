package com.salton123.xlog.demo;

import android.app.Application;

import com.salton123.log.XLog;
import com.salton123.log.XLogConfig;

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
                .setDebugable(true)
                .setWhetherToSaveLog(true));
    }
}
