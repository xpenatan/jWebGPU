package com.github.xpenatan.webgpu;

import com.github.xpenatan.jParser.api.NativeObject;

public class WGPUTexelCopyTextureInfo extends NativeObject {

    public final static WGPUTexelCopyTextureInfo NULL = native_new();

    public static WGPUTexelCopyTextureInfo native_new() {
        return new WGPUTexelCopyTextureInfo((byte) 0, (char) 0);
    }

    private WGPUTexelCopyTextureInfo(byte v, char c) {
    }
}