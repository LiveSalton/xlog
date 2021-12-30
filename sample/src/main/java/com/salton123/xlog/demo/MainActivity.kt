package com.salton123.xlog.demo

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.salton123.log.Utils
import com.salton123.log.XLog
import java.io.File
import kotlinx.android.synthetic.main.aty_main.tvHello

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

    val permissionsUtil by lazy { PermissionsUtil.with(this) }
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.aty_main)
        tvHello.setOnClickListener(object : View.OnClickListener {
            override fun onClick(v: View?) {
                XLog.i(this, "hello")
                val file = File(Utils.getDefaultPath() + packageName)
                if (!file.exists()) {
                    val ret = file.parentFile.mkdirs()
                    XLog.i(this, "ret:$ret")
                }
            }
        })
        Log.i("MainActivity", "hello")

        val storage = arrayOf(
            PermissionsUtil.Permission.Storage.READ_EXTERNAL_STORAGE,
            PermissionsUtil.Permission.Storage.WRITE_EXTERNAL_STORAGE
        )
        permissionsUtil.requestCode(1).permissions(*storage).request()
        Thread {
            while (true) {
                XLog.i("MainActivity", "MainActivity hello")
//                Thread.sleep(1000)
                XLog.w(this, "MainActivityhello")
                XLog.v(this, "MainActivityhello")
                XLog.d(this, "MainActivityhello")
                XLog.e(this, "MainActivityhello")
            }
        }.start()
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
}