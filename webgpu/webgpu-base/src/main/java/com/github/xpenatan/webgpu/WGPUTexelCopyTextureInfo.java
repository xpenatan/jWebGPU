package com.github.xpenatan.webgpu;

import com.github.xpenatan.jParser.idl.IDLBase;

public class WGPUTexelCopyTextureInfo extends IDLBase {

    public final static WGPUTexelCopyTextureInfo NULL = native_new();

    public static WGPUTexelCopyTextureInfo native_new() {
        return new WGPUTexelCopyTextureInfo((byte) 0, (char) 0);
    }

    private WGPUTexelCopyTextureInfo(byte v, char c) {
    }
}