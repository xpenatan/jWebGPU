package com.github.xpenatan.webgpu.demo.triangle;


import com.github.xpenatan.webgpu.backend.teavm.TeaVMApp;

public class MainTeaVM {
    public static void main(String[] args) {
        new TeaVMApp(new TriangleApp());
    }
}