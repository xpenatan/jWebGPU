package com.github.xpenatan.webgpu.demo.triangle;

import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;

public class TriangleApp implements ApplicationListener {
    @Override
    public void create(WGPUApp wgpu) {
        if(wgpu.surface != null) {
            System.out.println("Surface created");
        }
        else {
            System.out.println("Surface not created");
        }
    }

    @Override
    public void render(WGPUApp wgpu) {

    }
}