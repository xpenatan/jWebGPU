package com.github.xpenatan.webgpu.demo.triangle;

import com.github.xpenatan.webgpu.AdapterInfo;
import com.github.xpenatan.webgpu.BackendType;
import com.github.xpenatan.webgpu.CallbackMode;
import com.github.xpenatan.webgpu.JAdapter;
import com.github.xpenatan.webgpu.JDevice;
import com.github.xpenatan.webgpu.JInstance;
import com.github.xpenatan.webgpu.RequestAdapterCallback;
import com.github.xpenatan.webgpu.RequestAdapterStatus;
import com.github.xpenatan.webgpu.RequestDeviceCallback;
import com.github.xpenatan.webgpu.RequestDeviceStatus;
import com.github.xpenatan.webgpu.Status;
import com.github.xpenatan.webgpu.WDeviceDescriptor;
import com.github.xpenatan.webgpu.WInstance;
import com.github.xpenatan.webgpu.WRequestAdapterOptions;
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
        WRequestAdapterOptions op = new WRequestAdapterOptions();
        CallbackMode mode = CallbackMode.AllowProcessEvents;
        RequestAdapterCallback callback = new RequestAdapterCallback() {
            @Override
            protected void OnCallback(RequestAdapterStatus status, JAdapter adapter) {
                TriangleApp.this.adapter = adapter;
                System.out.println("Callback Java: " + status);

                WDeviceDescriptor deviceDescriptor = new WDeviceDescriptor();
                adapter.RequestDevice(deviceDescriptor, mode, new RequestDeviceCallback() {
                    @Override
                    protected void OnCallback(RequestDeviceStatus status, JDevice device) {
                        TriangleApp.this.device = device;
                        AdapterInfo info = new AdapterInfo();
                        Status getInfoStatus = device.GetAdapterInfo(info);
                        System.out.println("getInfoStatus: " + getInfoStatus);

                        BackendType backendType = info.get_backendType();
                        System.out.println("backendType: " + backendType);

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
            WInstance get = instance.Get();
            get.ProcessEvents();
        }
    }
}
