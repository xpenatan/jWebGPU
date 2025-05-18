package com.github.xpenatan.webgpu.backend.core;

import com.github.xpenatan.webgpu.JAdapter;
import com.github.xpenatan.webgpu.JAdapterInfo;
import com.github.xpenatan.webgpu.JDevice;
import com.github.xpenatan.webgpu.JDeviceDescriptor;
import com.github.xpenatan.webgpu.JInstance;
import com.github.xpenatan.webgpu.JQueue;
import com.github.xpenatan.webgpu.JRequestAdapterOptions;
import com.github.xpenatan.webgpu.JSurface;
import com.github.xpenatan.webgpu.JWebGPU;
import com.github.xpenatan.webgpu.RequestAdapterCallback;
import com.github.xpenatan.webgpu.RequestDeviceCallback;
import com.github.xpenatan.webgpu.WGPUAdapterType;
import com.github.xpenatan.webgpu.WGPUBackendType;
import com.github.xpenatan.webgpu.WGPUCallbackMode;
import com.github.xpenatan.webgpu.WGPURequestAdapterStatus;
import com.github.xpenatan.webgpu.WGPURequestDeviceStatus;

public class WGPUApp {
    public int width;
    public int height;
    public JInstance instance;
    public JAdapter adapter;
    public JDevice device;
    public JSurface surface;
    public JQueue queue;

    private boolean isReady;

    public void init() {
        instance = new JInstance();
        JRequestAdapterOptions op = new JRequestAdapterOptions();
        WGPUCallbackMode mode = WGPUCallbackMode.WGPUCallbackMode_AllowProcessEvents;
        RequestAdapterCallback callback = new RequestAdapterCallback() {
            @Override
            protected void OnCallback(WGPURequestAdapterStatus status, JAdapter adapter) {
                WGPUApp.this.adapter = adapter;
                JAdapterInfo info = new JAdapterInfo();
                if(adapter.GetInfo(info)) {
                    WGPUBackendType backendType = info.GetBackendType();
                    System.out.println("BackendType: " + backendType);
                    WGPUAdapterType adapterType = info.GetAdapterType();
                    System.out.println("AdapterType: " + adapterType);
                    String vendor = info.GetVendor().c_str();
                    System.out.println("Vendor: " + vendor);
                    String architecture = info.GetArchitecture().c_str();
                    System.out.println("Architecture: " + architecture);
                    String description = info.GetDescription().c_str();
                    System.out.println("Description: " + description);
                    String device = info.GetDevice().c_str();
                    System.out.println("Device: " + device);
                }

                JDeviceDescriptor deviceDescriptor = new JDeviceDescriptor();
                adapter.RequestDevice(deviceDescriptor, mode, new RequestDeviceCallback() {
                    @Override
                    protected void OnCallback(WGPURequestDeviceStatus status, JDevice device) {
                        WGPUApp.this.device = device;
                        queue = device.GetQueue();
                        System.out.println("isReady: " + status);
                        System.out.println("Platform: " + JWebGPU.GetPlatformType());
                        isReady = true;
                    }
                });
            }
        };
        instance.RequestAdapter(op, mode, callback);
    }

    public void update() {
        instance.ProcessEvents();
    }

    public boolean isReady() {
        return isReady;
    }
}