package com.github.xpenatan.webgpu;

public class RequestDeviceCallback {

    private void internal_onCallback(int status, long device) {
        WGPUDevice WGPUDevice_TEMP_STATIC_GEN_0 = new WGPUDevice((byte) 1, (char) 1);
        WGPUDevice_TEMP_STATIC_GEN_0.getNativeData().reset(device, true);
        onCallback(WGPURequestDeviceStatus.MAP.get(status), WGPUDevice_TEMP_STATIC_GEN_0);
    }

    native void onCallback(WGPURequestDeviceStatus status, WGPUDevice device);
}