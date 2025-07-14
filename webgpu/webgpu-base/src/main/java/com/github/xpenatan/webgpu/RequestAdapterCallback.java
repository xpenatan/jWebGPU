package com.github.xpenatan.webgpu;

import idl.IDLBase;

public class RequestAdapterCallback extends IDLBase {

    private void internal_onCallback(int status, long adapter) {
        WGPUAdapter WGPUAdapter_TEMP_STATIC_GEN_0 = new WGPUAdapter((byte) 1, (char) 1);
        WGPUAdapter_TEMP_STATIC_GEN_0.getNativeData().reset(adapter, true);
        onCallback(WGPURequestAdapterStatus.MAP.get(status), WGPUAdapter_TEMP_STATIC_GEN_0);
    }

    native void onCallback(WGPURequestAdapterStatus status, WGPUAdapter adapter);
}