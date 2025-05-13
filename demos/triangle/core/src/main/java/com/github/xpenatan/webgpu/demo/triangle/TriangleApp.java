package com.github.xpenatan.webgpu.demo.triangle;

import com.github.xpenatan.webgpu.CallbackMode;
import com.github.xpenatan.webgpu.JAdapter;
import com.github.xpenatan.webgpu.JDevice;
import com.github.xpenatan.webgpu.JInstance;
import com.github.xpenatan.webgpu.RequestAdapterCallback;
import com.github.xpenatan.webgpu.RequestAdapterStatus;
import com.github.xpenatan.webgpu.RequestDeviceCallback;
import com.github.xpenatan.webgpu.RequestDeviceStatus;
import com.github.xpenatan.webgpu.WDeviceDescriptor;
import com.github.xpenatan.webgpu.WInstance;
import com.github.xpenatan.webgpu.WRequestAdapterOptions;
import com.github.xpenatan.webgpu.WebGPULoader;

public class TriangleApp {
    private boolean isSuccess;

    long previousTime;

    private boolean init = false;

    private JInstance instance;
    private JAdapter adapter;
    private JDevice device;

    public void create() {
        WebGPULoader.init((isSuccess, e) -> {
            this.isSuccess = isSuccess;
            previousTime = System.currentTimeMillis();
            init = true;
        });
    }

    private void init() {
//        JWebGPU.Set();
//        Instance instance = JWebGPU.CreateInstance();
//        WGPURequestAdapterOptions options = new WGPURequestAdapterOptions();
//        WGPUCallbackMode mode = WGPUCallbackMode.WGPUCallbackMode_WaitAnyOnly;
//        RequestAdapterCallback callback = new RequestAdapterCallback() {
//            @Override
//            protected void OnCallback(WGPURequestAdapterStatus status, Adapter adapter) {
//                System.out.println("Status: " + status);
//            }
//        };
//        instance.RequestAdapter(options, mode, callback);
    }

    public void render() {
        long timeNow = System.currentTimeMillis();

        long now = timeNow - previousTime;

        if(now > 4000 && init) {
            init = false;
//            JWebGPU.Set();
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
                            System.out.println("Callback Java 2: " + status);
                        }
                    });
                }
            };
            instance.RequestAdapter(op, mode, callback);
        }

        if(instance != null) {
            WInstance get = instance.Get();
            get.ProcessEvents();
        }
    }
}
