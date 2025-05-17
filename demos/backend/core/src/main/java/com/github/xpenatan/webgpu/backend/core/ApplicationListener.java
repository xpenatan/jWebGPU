package com.github.xpenatan.webgpu.backend.core;

public interface ApplicationListener {

    void create(WGPUApp wgpu);

    void render(WGPUApp wgpu);
}
