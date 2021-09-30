### XLog
Simple logger with mmap.

### Setup

Download
```groovy
implementation 'com.salton123:log:1.0.0.0'
```

Initialize(optional)
```java
XLog.config(XLogConfig)
```
And use
```java
XLog.d(obj,"hello");
```

### Options
```java
XLog.d(this,"debug");
XLog.e(this,"error");
XLog.w(this,"warning");
XLog.v(this,"verbose");
XLog.i(this,"information");
```

### Advanced
```java
 XLog.config(new XLogConfig()
         .setDebugable(BuildConfig.DEBUG) //whether print info to logcat
         .setWhetherToSaveLog(true)  //whether save log to file
         .setLogDefaultSplitSize(1)  //split log size , unit m
         .setSavePath(Utils.getDefaultPath() + getPackageName() + File.separator) //log save path
         .setLogDeleteDelayDay(10)   //log delete delay day
 );
```
### License
<pre>
Copyright 2019

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
</pre>

### 混淆配置
```java
在app/proguard-rules.pro添加如下配置
-keep class com.salton123.io.**{*;}
-keep class com.salton123.log.**{*;}
```