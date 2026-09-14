package com.github.xpenatan.webgpu.backend.core;

public interface ApplicationListener {

    void create(WGPUApp wgpu);

    void render(WGPUApp wgpu);

    /** Releases example-owned resources, including after a partial create. WGPUApp owns the device and surface. */
    void dispose();
}
