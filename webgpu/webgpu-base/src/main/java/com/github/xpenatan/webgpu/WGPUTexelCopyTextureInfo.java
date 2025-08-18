package com.github.xpenatan.webgpu;

import idl.IDLBase;

public class WGPUTexelCopyTextureInfo extends IDLBase {

    public final static WGPUTexelCopyTextureInfo NULL = createInstance();

    public static WGPUTexelCopyTextureInfo createInstance() {
        return new WGPUTexelCopyTextureInfo((byte) 0, (char) 0);
    }

    private WGPUTexelCopyTextureInfo(byte v, char c) {
    }
}