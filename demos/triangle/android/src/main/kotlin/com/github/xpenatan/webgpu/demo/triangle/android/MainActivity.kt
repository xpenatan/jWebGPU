package com.github.xpenatan.webgpu.demo.triangle.android

import android.os.Bundle
import com.github.xpenatan.webgpu.backend.android.AndroidApplication
import com.github.xpenatan.webgpu.demo.triangle.TriangleApp

class MainActivity : AndroidApplication() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        initialize(TriangleApp())
    }
}