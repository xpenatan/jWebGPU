package com.github.xpenatan.webgpu;

import idl.IDLBase;

public class WGPUTexelCopyBufferLayout extends IDLBase {

    public final static WGPUTexelCopyBufferLayout NULL = createInstance();

    public static WGPUTexelCopyBufferLayout createInstance() {
        return new WGPUTexelCopyBufferLayout((byte) 0, (char) 0);
    }

    private WGPUTexelCopyBufferLayout(byte v, char c) {
    }
}