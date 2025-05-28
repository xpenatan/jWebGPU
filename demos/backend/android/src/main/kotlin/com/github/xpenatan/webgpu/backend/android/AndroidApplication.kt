package com.github.xpenatan.webgpu.backend.android

import android.app.Activity
import android.os.Bundle
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.github.xpenatan.jparser.loader.JParserLibraryLoaderListener
import com.github.xpenatan.webgpu.WebGPULoader
import com.github.xpenatan.webgpu.backend.core.ApplicationListener


open class AndroidApplication : Activity() {


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val surfaceView = SurfaceView(this)
        setContentView(surfaceView)

        surfaceView.getHolder().addCallback(object : SurfaceHolder.Callback {
            override fun surfaceCreated(holder: SurfaceHolder) {
                onSurfaceCreated(holder.surface)
            }

            override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
            }

            override fun surfaceDestroyed(holder: SurfaceHolder) {
//                nativeOnSurfaceDestroyed()
            }
        })
    }

    fun initialize(listener: ApplicationListener?) {

    }

    private fun onSurfaceCreated(surface: Surface) {
        WebGPULoader.init(JParserLibraryLoaderListener { isSuccess: Boolean, e: Exception? ->
            println("WebGPU Success: " + isSuccess)
            if (isSuccess) {
            } else {
                e!!.printStackTrace()
            }
        })
    }
}