package com.salton123.xlog.demo

import android.content.Context
import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.salton123.log.Utils.defaultPath
import com.salton123.log.XLog
import java.io.File
import kotlin.system.exitProcess
import kotlinx.android.synthetic.main.aty_main.tvEffectiveWrite
import kotlinx.android.synthetic.main.aty_main.tvMultiInstance

/**
 * User: newSalton@outlook.com
 * Date: 2018/11/15 11:43 AM
 * ModifyTime: 11:43 AM
 * Description:
 */

class MainActivity : AppCompatActivity(), PermissionsUtil.IPermissionsCallback {

    override fun onPermissionsGranted(requestCode: Int, vararg permission: String?) {
    }

    override fun onPermissionsDenied(requestCode: Int, vararg permission: String?) {
    }

    var index = 0L
    val permissionsUtil by lazy { PermissionsUtil.with(this) }
    var isStartWrite = false
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.aty_main)
        val sharedPreferences = getSharedPreferences("test", Context.MODE_PRIVATE)
        index = sharedPreferences.getLong("index", 0)
        tvEffectiveWrite.setOnClickListener {
            if (!isStartWrite) {
                XLog.i("MainActivity", "hello ${index++}")
//                Thread {
//                    while (true) {
//                        XLog.i("MainActivity", "hello ${index++}")
//                        XLog.w("MainActivity", "hello ${index++}")
//                        XLog.e("MainActivity", "hello ${index++}")
//                        XLog.v("MainActivity", "hello ${index++}")
//                        XLog.d("MainActivity", "hello ${index++}")
//                        Thread.sleep(100)
//                    }
//                }.start()
                isStartWrite = true;
            } else {
                val sharedPreferences = getSharedPreferences("test", Context.MODE_PRIVATE)
                sharedPreferences.edit().apply {
                    this.putLong("index", index)
                    commit()
                    XLog.e("MainActivity", "exit $index")
                }
                exitProcess(0)
            }
        }
        tvMultiInstance.setOnClickListener {
            testMmapWriter()
        }
        val storage = arrayOf(
            PermissionsUtil.Permission.Storage.READ_EXTERNAL_STORAGE,
            PermissionsUtil.Permission.Storage.WRITE_EXTERNAL_STORAGE
        )
        permissionsUtil.requestCode(1).permissions(*storage).request()
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        //需要调用onRequestPermissionsResult
        permissionsUtil.onRequestPermissionsResult(requestCode, permissions, grantResults)
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        //监听跳转到权限设置界面后再回到应用
        permissionsUtil.onActivityResult(requestCode, resultCode, data)
        super.onActivityResult(requestCode, resultCode, data)
    }

    private fun testMmapWriter() {
        for (i in 1..10) {
//            val pointer = MmapWriter.create(defaultPath + File.separator + i + ".txt", 1024, false)
//            MmapWriter.write(pointer, "hello")
//            MmapWriter.flush(pointer)
            //因为是异步写入，所以立即调用destroy存在写入不到数据的问题
//            MmapWriter.destory(pointer)
        }
    }
}