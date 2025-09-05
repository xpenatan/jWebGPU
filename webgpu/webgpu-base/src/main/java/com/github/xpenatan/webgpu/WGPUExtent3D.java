package com.github.xpenatan.webgpu;

import idl.IDLBase;

public class WGPUExtent3D extends IDLBase {

    public final static WGPUExtent3D NULL = native_new();

    public static WGPUExtent3D native_new() {
        return new WGPUExtent3D((byte) 0, (char) 0);
    }

    private WGPUExtent3D(byte v, char c) {
    }
}