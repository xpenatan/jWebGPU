package com.github.xpenatan.webgpu;

import com.github.xpenatan.jParser.api.NativeObject;

public class WGPUTexelCopyBufferLayout extends NativeObject {

    public final static WGPUTexelCopyBufferLayout NULL = native_new();

    public static WGPUTexelCopyBufferLayout native_new() {
        return new WGPUTexelCopyBufferLayout((byte) 0, (char) 0);
    }

    private WGPUTexelCopyBufferLayout(byte v, char c) {
    }
}