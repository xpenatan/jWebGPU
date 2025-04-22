package com.github.xpenatan.webgpu.demo.triangle;

import com.github.xpenatan.webgpu.Adapter;
import com.github.xpenatan.webgpu.ConvertibleStatus;
import com.github.xpenatan.webgpu.Surface;
import com.github.xpenatan.webgpu.SurfaceCapabilities;
import com.github.xpenatan.webgpu.WebGPU;
import com.github.xpenatan.webgpu.WebGPULoader;

public class TriangleApp {

    public void create() {
        WebGPULoader.init((isSuccess, e) -> {
            if(isSuccess) {
                init();
            }
        });
    }

    private void init() {
        Surface surface = new Surface();
        Adapter adapter = new Adapter();
        SurfaceCapabilities capabilities = new SurfaceCapabilities();
        ConvertibleStatus convertibleStatus = surface.GetCapabilities(adapter, capabilities);


        WebGPU.native_set();
    }

    public void render() {

    }
}
