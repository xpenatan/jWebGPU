package com.github.xpenatan.webgpu.backend.android

import android.app.Activity
import android.os.Bundle
import com.github.xpenatan.webgpu.backend.core.ApplicationListener

open class AndroidApplication : Activity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    fun initialize(listener: ApplicationListener?) {

    }
}