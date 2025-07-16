package com.github.xpenatan.webgpu.demo.app;


import com.github.xpenatan.webgpu.backend.teavm.TeaVMApp;

public class MainTeaVM {
    public static void main(String[] args) {
//        new TeaVMApp(new HelloTriangle());
        new TeaVMApp(new PlayingWithBuffers());
//        new TeaVMApp(new AFirstVertexAttribute());
    }
}