package com.github.xpenatan.webgpu.demo.triangle.android;

import android.os.Bundle;
import com.github.xpenatan.webgpu.backend.android.AndroidApplication;
import com.github.xpenatan.webgpu.demo.triangle.TriangleApp;

public class MainActivity extends AndroidApplication {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        initialize(new TriangleApp());
    }
}
