package com.github.xpenatan.webgpu.backend.core.graphics;

import com.github.xpenatan.webgpu.JWebGPUBackend;
import com.github.xpenatan.webgpu.JWebGPULoader;
import com.github.xpenatan.webgpu.WGPU;
import com.github.xpenatan.webgpu.WGPUCompositeAlphaMode;
import com.github.xpenatan.webgpu.WGPUPlatformType;
import com.github.xpenatan.webgpu.WGPUPresentMode;
import com.github.xpenatan.webgpu.WGPUSurfaceCapabilities;
import com.github.xpenatan.webgpu.WGPUSurfaceConfiguration;
import com.github.xpenatan.webgpu.WGPUSurfaceTexture;
import com.github.xpenatan.webgpu.WGPUTexture;
import com.github.xpenatan.webgpu.WGPUTextureAspect;
import com.github.xpenatan.webgpu.WGPUTextureFormat;
import com.github.xpenatan.webgpu.WGPUTextureUsage;
import com.github.xpenatan.webgpu.WGPUTextureView;
import com.github.xpenatan.webgpu.WGPUTextureViewDescriptor;
import com.github.xpenatan.webgpu.WGPUTextureViewDimension;
import com.github.xpenatan.webgpu.WGPUVectorTextureFormat;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;

public class SurfaceFrameContext {

    private WGPUTextureFormat surfaceFormat;
    private WGPUTexture currentTexture;

    public WGPUTextureFormat configure(WGPUApp app, boolean vsyncEnabled) {
        WGPUSurfaceCapabilities capabilities = WGPUSurfaceCapabilities.obtain();
        app.surface.getCapabilities(app.adapter, capabilities);
        WGPUVectorTextureFormat formats = capabilities.getFormats();
        surfaceFormat = formats.get(0);

        WGPUSurfaceConfiguration config = WGPUSurfaceConfiguration.obtain();
        config.setWidth(app.width);
        config.setHeight(app.height);
        config.setFormat(surfaceFormat);
        config.setViewFormats(WGPUVectorTextureFormat.NULL);
        WGPUTextureUsage usage = WGPUTextureUsage.RenderAttachment;
        if(WGPU.getPlatformType() != WGPUPlatformType.WGPU_Android) {
            usage = usage.or(WGPUTextureUsage.CopySrc);
        }
        config.setUsage(usage);
        config.setDevice(app.device);
        config.setPresentMode(vsyncEnabled ? WGPUPresentMode.Fifo : WGPUPresentMode.Immediate);
        config.setAlphaMode(WGPUCompositeAlphaMode.Auto);
        app.surface.configure(config);
        return surfaceFormat;
    }

    public WGPUTextureView acquireCurrentView(WGPUApp app) {
        if(currentTexture != null) {
            if(JWebGPULoader.getBackend() == JWebGPUBackend.DAWN) {
                currentTexture.release();
            }
            currentTexture = null;
        }

        WGPUSurfaceTexture surfaceTexture = new WGPUSurfaceTexture();
        app.surface.getCurrentTexture(surfaceTexture);

        WGPUTexture texture = new WGPUTexture();
        surfaceTexture.getTexture(texture);
        WGPUTextureFormat textureFormat = texture.getFormat();

        WGPUTextureViewDescriptor viewDescriptor = WGPUTextureViewDescriptor.obtain();
        viewDescriptor.setLabel("Surface texture view");
        viewDescriptor.setFormat(textureFormat);
        viewDescriptor.setDimension(WGPUTextureViewDimension._2D);
        viewDescriptor.setBaseMipLevel(0);
        viewDescriptor.setMipLevelCount(1);
        viewDescriptor.setBaseArrayLayer(0);
        viewDescriptor.setArrayLayerCount(1);
        viewDescriptor.setAspect(WGPUTextureAspect.All);

        WGPUTextureView view = new WGPUTextureView();
        texture.createView(viewDescriptor, view);

        currentTexture = texture;
        return view;
    }

    public WGPUTexture getCurrentTexture() {
        return currentTexture;
    }

    public void present(WGPUApp app) {
        if(WGPU.getPlatformType() != WGPUPlatformType.WGPU_Web) {
            app.surface.present();
        }
        if(currentTexture != null) {
            if(JWebGPULoader.getBackend() == JWebGPUBackend.DAWN) {
                currentTexture.release();
            }
            currentTexture = null;
        }
    }
}


