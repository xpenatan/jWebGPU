package com.github.xpenatan.webgpu.demo.triangle;

import com.github.xpenatan.webgpu.JWebGPU;
import com.github.xpenatan.webgpu.WebGPULoader;

public class TriangleApp {

    public void create() {
        WebGPULoader.init((isSuccess, e) -> {
            if(isSuccess) {
                init();
            }
        });
    }

    private void init() {
        JWebGPU.Set();
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

    }
}
