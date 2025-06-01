package com.github.xpenatan.webgpu.demo.triangle.android;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.SurfaceView;
import com.github.xpenatan.webgpu.backend.android.AndroidApplication;
import com.github.xpenatan.webgpu.demo.databinding.MainLayoutBinding;
import com.github.xpenatan.webgpu.demo.triangle.TriangleApp;

public class MainActivity extends AndroidApplication {

    private MainLayoutBinding binding;

    private TriangleApp triangleApp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        triangleApp = new TriangleApp();
        binding = MainLayoutBinding.inflate(LayoutInflater.from(this));
        SurfaceView surfaceView = binding.viewSurface;
        initialize(triangleApp, surfaceView, binding.getRoot());
        setupButtons();
    }

    private void setupButtons() {
        binding.btnRed.setOnClickListener(v -> triangleApp.setColor(1.0f, 0, 0));
        binding.btnGreen.setOnClickListener(v -> triangleApp.setColor(0, 1.0f, 0));
        binding.btnBlue.setOnClickListener(v -> triangleApp.setColor(0, 0, 1.0f));
    }
}
