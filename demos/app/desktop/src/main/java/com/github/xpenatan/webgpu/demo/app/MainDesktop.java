package com.github.xpenatan.webgpu.demo.app;

import com.github.xpenatan.webgpu.backend.desktop.GLFWApp;

public class MainDesktop {
    public static void main(String[] args) {
//        new GLFWApp(new HelloTriangle());
        new GLFWApp(new PlayingWithBuffers());
    }
}