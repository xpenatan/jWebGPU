package com.github.xpenatan.webgpu.backend.desktop;

import com.github.xpenatan.webgpu.WebGPULoader;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;
import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.glfw.GLFWNativeWin32;
import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.system.MemoryUtil.NULL;

public class GLFWApp {
    long window;
    long windowHandle;

    private WGPUApp wgpu;
    private int wGPUInit = 0;

    public GLFWApp(ApplicationListener applicationInterface) {
        openWindow();

        WebGPULoader.init((isSuccess, e) -> {
            System.out.println("WebGPU Success: " + isSuccess);
            if(isSuccess) {
                wGPUInit = 1;
            }
            else {
                e.printStackTrace();
            }
        });


        while(!glfwWindowShouldClose(window)) {
            if(wGPUInit > 0) {
                if(wGPUInit == 1) {
                    wGPUInit = 2;
                    wgpu = new WGPUApp();
                    wgpu.init();
                }
                if(wGPUInit == 2 && wgpu.isReady()) {
                    wGPUInit = 3;
                    createSurface();
                    applicationInterface.create(wgpu);
                }
                if(wGPUInit == 3) {
                    applicationInterface.render(wgpu);
                }
                wgpu.update();
            }
            glfwPollEvents();
        }
        closeWindow();
    }

    public void openWindow() {
        // Set up an error callback
        GLFWErrorCallback.createPrint(System.err).set();

        // Initialize GLFW
        if (!glfwInit()) {
            throw new IllegalStateException("Unable to initialize GLFW");
        }

        // Configure GLFW
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Hide window until ready
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // Make window resizable
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // because we will use webgpu

        // Create the window
        int windowWidth = 800;
        int windowHeight = 600;
        window = glfwCreateWindow(windowWidth, windowHeight, "LWJGL Window", NULL, NULL);
        if (window == NULL) {
            glfwTerminate();
            throw new RuntimeException("Failed to create GLFW window");
        }

        windowHandle = GLFWNativeWin32.glfwGetWin32Window(window);

        // Center the window
        var vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(
                window,
                (vidMode.width() - windowWidth) / 2,
                (vidMode.height() - windowHeight) / 2
        );


        // Make the window visible
        glfwShowWindow(window);
    }

    public void closeWindow () {

        // Free the window callbacks and destroy the window
        glfwFreeCallbacks(window);
        glfwDestroyWindow(window);

        // Terminate GLFW and free the error callback
        glfwTerminate();
        glfwSetErrorCallback(null).free();
    }

    private void createSurface() {
//        wgpu.surface = wgpu.instance.CreateWebSurface(TeaVMApp.canvasWGPU);
    }
}