package com.github.xpenatan.webgpu.demo.triangle;

import com.github.xpenatan.webgpu.Instance;
import com.github.xpenatan.webgpu.WebGPU;
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

//        WebGPU.Set();

        Instance instance = WebGPU.CreateInstance();
        System.out.println("PASS");
//        WGPURequestAdapterOptions options = new WGPURequestAdapterOptions();
//        int mode = WGPUCallbackMode.WGPUCallbackMode_WaitAnyOnly;
//        RequestAdapterCallback callback = new RequestAdapterCallback() {
//            @Override
//            protected void OnCallback(int status, Adapter adapter) {
//                System.out.println("Status: " + status);
//            }
//        };
//        instance.RequestAdapter(options, mode, callback);
    }

    public void render() {

    }
}
