package com.salton123.xlog.demo

import android.content.Intent
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import com.salton123.log.XLog

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


        Log.i("MainActivity", "hello")

        val storage = arrayOf(PermissionsUtil.Permission.Storage.READ_EXTERNAL_STORAGE,
            PermissionsUtil.Permission.Storage.WRITE_EXTERNAL_STORAGE)
        permissionsUtil.requestCode(1).permissions(*storage).request()
        Thread {
            while (true) {
                XLog.i(this, "MainActivityMainActivityMainActivityMainActivityMainActivityhello")
                XLog.w(this, "MainActivityMainActivityMainActivityMainActivityMainActivityhello")
                XLog.v(this, "MainActivityMainActivityMainActivityMainActivityMainActivityhello")
                XLog.d(this, "MainActivityMainActivityMainActivityMainActivityMainActivityhello")
                XLog.e(this, "MainActivityMainActivityMainActivityMainActivityMainActivityhello")
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