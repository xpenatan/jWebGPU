package com.github.xpenatan.webgpu.demo.triangle;

import com.github.xpenatan.webgpu.JSurfaceCapabilities;
import com.github.xpenatan.webgpu.JSurfaceConfiguration;
import com.github.xpenatan.webgpu.WGPUCompositeAlphaMode;
import com.github.xpenatan.webgpu.WGPUPresentMode;
import com.github.xpenatan.webgpu.WGPUTextureFormat;
import com.github.xpenatan.webgpu.WGPUTextureUsage;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;

public class TriangleApp implements ApplicationListener {

    private WGPUTextureFormat surfaceFormat;

    @Override
    public void create(WGPUApp wgpu) {
        if(wgpu.surface != null) {
            System.out.println("Surface created");

            JSurfaceCapabilities surfaceCapabilities = new JSurfaceCapabilities();
            wgpu.surface.GetCapabilities(wgpu.adapter, surfaceCapabilities);
            surfaceFormat = surfaceCapabilities.GetFormats(0);
            System.out.println("surfaceFormat: " + surfaceFormat);

            boolean vsyncEnabled = true;

            JSurfaceConfiguration config = new JSurfaceConfiguration();
//            config.SetNextInChain();
            config.SetWidth(wgpu.width);
            config.SetHeight(wgpu.height);
            config.SetFormat(surfaceFormat);
            config.SetViewFormatCount(0);
//            config.SetViewFormats(JavaWebGPU.createNullPointer());
            config.SetUsage(WGPUTextureUsage.WGPUTextureUsage_RenderAttachment);
            config.SetDevice(wgpu.device);
            config.SetPresentMode(vsyncEnabled ? WGPUPresentMode.WGPUPresentMode_Fifo : WGPUPresentMode.WGPUPresentMode_Immediate);
            config.SetAlphaMode(WGPUCompositeAlphaMode.WGPUCompositeAlphaMode_Auto);

        }
        else {
            System.out.println("Surface not created");
        }
    }

    @Override
    public void render(WGPUApp wgpu) {

    }
}