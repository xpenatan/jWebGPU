package com.github.xpenatan.webgpu.demo.triangle;

import com.github.xpenatan.webgpu.Adapter;
import com.github.xpenatan.webgpu.CallbackMode;
import com.github.xpenatan.webgpu.JInstance;
import com.github.xpenatan.webgpu.JWebGPU;
import com.github.xpenatan.webgpu.RequestAdapterCallback;
import com.github.xpenatan.webgpu.RequestAdapterOptions;
import com.github.xpenatan.webgpu.RequestAdapterStatus;
import com.github.xpenatan.webgpu.WebGPULoader;
import java.util.Timer;

public class TriangleApp {
    private boolean isSuccess;

    long previousTime;

    private boolean init = false;

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

            JInstance instance = new JInstance();
            RequestAdapterOptions op = new RequestAdapterOptions();
            CallbackMode mode = CallbackMode.WaitAnyOnly;
            RequestAdapterCallback callback = new RequestAdapterCallback() {
                @Override
                protected void OnCallback(RequestAdapterStatus status, Adapter adapter) {
                    System.out.println("Callback Java");
                }
            };
            instance.RequestAdapter(op, mode, callback);
        }
    }
}
