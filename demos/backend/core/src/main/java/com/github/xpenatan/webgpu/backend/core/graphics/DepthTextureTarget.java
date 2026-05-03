package com.github.xpenatan.webgpu.backend.core.graphics;

import com.github.xpenatan.webgpu.WGPUDepthStencilState;
import com.github.xpenatan.webgpu.WGPUDevice;
import com.github.xpenatan.webgpu.WGPUExtent3D;
import com.github.xpenatan.webgpu.WGPULoadOp;
import com.github.xpenatan.webgpu.WGPUOptionalBool;
import com.github.xpenatan.webgpu.WGPURenderPassDepthStencilAttachment;
import com.github.xpenatan.webgpu.WGPUStoreOp;
import com.github.xpenatan.webgpu.WGPUTexture;
import com.github.xpenatan.webgpu.WGPUTextureDescriptor;
import com.github.xpenatan.webgpu.WGPUTextureFormat;
import com.github.xpenatan.webgpu.WGPUTextureUsage;
import com.github.xpenatan.webgpu.WGPUTextureView;
import com.github.xpenatan.webgpu.WGPUTextureViewDescriptor;
import com.github.xpenatan.webgpu.WGPUTextureViewDimension;
import com.github.xpenatan.webgpu.WGPUCompareFunction;
import com.github.xpenatan.webgpu.WGPUTextureAspect;
import com.github.xpenatan.webgpu.WGPUVectorTextureFormat;

public class DepthTextureTarget {

    private WGPUTexture texture;
    private WGPUTextureView view;
    private WGPUTextureFormat format;

    public void create(WGPUDevice device, int width, int height, WGPUTextureFormat format) {
        this.format = format;
        if(texture != null) {
            texture.destroy();
            texture.release();
            texture = null;
        }
        if(view != null) {
            view.release();
            view = null;
        }

        WGPUTextureDescriptor depthDescriptor = WGPUTextureDescriptor.obtain();
        depthDescriptor.setLabel("depth texture");
        depthDescriptor.setFormat(format);
        depthDescriptor.setUsage(WGPUTextureUsage.RenderAttachment);
        depthDescriptor.setDimension(com.github.xpenatan.webgpu.WGPUTextureDimension._2D);
        depthDescriptor.setMipLevelCount(1);
        depthDescriptor.setSampleCount(1);
        depthDescriptor.setViewFormats(WGPUVectorTextureFormat.NULL);

        WGPUExtent3D size = depthDescriptor.getSize();
        size.setWidth(width);
        size.setHeight(height);
        size.setDepthOrArrayLayers(1);

        texture = new WGPUTexture();
        device.createTexture(depthDescriptor, texture);

        WGPUTextureViewDescriptor depthViewDescriptor = WGPUTextureViewDescriptor.obtain();
        depthViewDescriptor.setLabel("depth texture view");
        depthViewDescriptor.setFormat(format);
        depthViewDescriptor.setDimension(WGPUTextureViewDimension._2D);
        depthViewDescriptor.setBaseMipLevel(0);
        depthViewDescriptor.setMipLevelCount(1);
        depthViewDescriptor.setBaseArrayLayer(0);
        depthViewDescriptor.setArrayLayerCount(1);
        depthViewDescriptor.setAspect(WGPUTextureAspect.All);

        view = new WGPUTextureView();
        texture.createView(depthViewDescriptor, view);
    }

    public WGPUDepthStencilState createDepthStencilState() {
        WGPUDepthStencilState depthStencilState = WGPUDepthStencilState.obtain();
        depthStencilState.setFormat(format);
        depthStencilState.setDepthWriteEnabled(WGPUOptionalBool.True);
        depthStencilState.setDepthCompare(WGPUCompareFunction.Less);
        depthStencilState.setDepthBias(0);
        depthStencilState.setDepthBiasClamp(0.0f);
        depthStencilState.setDepthBiasSlopeScale(0.0f);
        depthStencilState.setStencilReadMask(0xFFFFFFFF);
        depthStencilState.setStencilWriteMask(0xFFFFFFFF);
        return depthStencilState;
    }

    public WGPURenderPassDepthStencilAttachment createAttachment() {
        WGPURenderPassDepthStencilAttachment attachment = WGPURenderPassDepthStencilAttachment.obtain();
        attachment.setView(view);
        attachment.setDepthLoadOp(WGPULoadOp.Clear);
        attachment.setDepthStoreOp(WGPUStoreOp.Store);
        attachment.setDepthClearValue(1.0f);
        attachment.setDepthReadOnly(false);
        attachment.setStencilLoadOp(WGPULoadOp.Undefined);
        attachment.setStencilStoreOp(WGPUStoreOp.Undefined);
        attachment.setStencilClearValue(0);
        attachment.setStencilReadOnly(true);
        return attachment;
    }

    public void dispose() {
        if(view != null) {
            view.release();
            view = null;
        }
        if(texture != null) {
            texture.destroy();
            texture.release();
            texture = null;
        }
    }
}


