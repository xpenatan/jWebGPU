package com.github.xpenatan.webgpu.demo.triangle;

import com.github.xpenatan.webgpu.JAdapter;
import com.github.xpenatan.webgpu.JAdapterInfo;
import com.github.xpenatan.webgpu.JDevice;
import com.github.xpenatan.webgpu.JDeviceDescriptor;
import com.github.xpenatan.webgpu.JInstance;
import com.github.xpenatan.webgpu.JRequestAdapterOptions;
import com.github.xpenatan.webgpu.RequestAdapterCallback;
import com.github.xpenatan.webgpu.RequestDeviceCallback;
import com.github.xpenatan.webgpu.WGPUAdapterType;
import com.github.xpenatan.webgpu.WGPUBackendType;
import com.github.xpenatan.webgpu.WGPUCallbackMode;
import com.github.xpenatan.webgpu.WGPURequestAdapterStatus;
import com.github.xpenatan.webgpu.WGPURequestDeviceStatus;
import com.github.xpenatan.webgpu.WebGPULoader;

public class TriangleApp {
    private boolean init = false;

    private JInstance instance;
    private JAdapter adapter;
    private JDevice device;

    public void create() {
        WebGPULoader.init((isSuccess, e) -> {
            init();
        });
    }

    private void init() {
        instance = new JInstance();
        JRequestAdapterOptions op = new JRequestAdapterOptions();
        WGPUCallbackMode mode = WGPUCallbackMode.WGPUCallbackMode_AllowProcessEvents;
        RequestAdapterCallback callback = new RequestAdapterCallback() {
            @Override
            protected void OnCallback(WGPURequestAdapterStatus status, JAdapter adapter) {
                TriangleApp.this.adapter = adapter;
                System.out.println("Callback Java: " + status);
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
                        TriangleApp.this.device = device;
                        System.out.println("RequestDevice: " + status);
                        init = true;
                    }
                });
            }
        };
        instance.RequestAdapter(op, mode, callback);
    }

    public void render() {
        if(init) {

        }
        if(instance != null) {
            instance.ProcessEvents();
        }
    }
}
