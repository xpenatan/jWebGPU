package com.github.xpenatan.webgpu.demo.app;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.SurfaceView;
import com.github.xpenatan.webgpu.backend.android.AndroidApplication;
import com.github.xpenatan.webgpu.demo.databinding.MainLayoutBinding;

public class MainAndroid extends AndroidApplication {

    private MainLayoutBinding binding;

    private WebGPUApp webgpuApp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        webgpuApp = new WebGPUApp();
        binding = MainLayoutBinding.inflate(LayoutInflater.from(this));
        SurfaceView surfaceView = binding.viewSurface;
        initialize(webgpuApp, surfaceView, binding.getRoot());
        setupButtons();
    }

    private void setupButtons() {
        binding.btnRed.setOnClickListener(v -> webgpuApp.setColor(1.0f, 0, 0));
        binding.btnGreen.setOnClickListener(v -> webgpuApp.setColor(0, 1.0f, 0));
        binding.btnBlue.setOnClickListener(v -> webgpuApp.setColor(0, 0, 1.0f));
    }
}
