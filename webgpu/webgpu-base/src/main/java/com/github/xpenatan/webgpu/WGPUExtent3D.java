package com.github.xpenatan.webgpu;

import idl.IDLBase;

public class WGPUExtent3D extends IDLBase {

    public final static WGPUExtent3D NULL = createInstance();

    public static WGPUExtent3D createInstance() {
        return new WGPUExtent3D((byte) 0, (char) 0);
    }

    private WGPUExtent3D(byte v, char c) {
    }
}