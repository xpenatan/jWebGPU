package com.github.xpenatan.webgpu.backend.desktop;

import com.github.xpenatan.jParser.api.NativeObject;
import com.github.xpenatan.webgpu.JWebGPUBackend;
import com.github.xpenatan.webgpu.JWebGPULoader;
import com.github.xpenatan.webgpu.WGPUBackendType;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;
import java.util.Locale;
import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.glfw.GLFWVidMode;
import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.glfw.GLFWNativeCocoa.glfwGetCocoaWindow;
import static org.lwjgl.glfw.GLFWNativeWayland.glfwGetWaylandDisplay;
import static org.lwjgl.glfw.GLFWNativeWayland.glfwGetWaylandWindow;
import static org.lwjgl.glfw.GLFWNativeWin32.glfwGetWin32Window;
import static org.lwjgl.glfw.GLFWNativeX11.glfwGetX11Display;
import static org.lwjgl.glfw.GLFWNativeX11.glfwGetX11Window;
import static org.lwjgl.system.MemoryUtil.NULL;

public class GLFWApp {
    long window;
    long windowHandle;

    private WGPUApp wgpu;
    private int wGPUInit = 0;
    private final JWebGPUBackend backend;
    private final String bridgeName;

    int windowWidth = 800;
    int windowHeight = 600;

    public GLFWApp(ApplicationListener applicationInterface) {
        backend = resolveBackend();
        bridgeName = resolveBridgeName();
        openWindow();

        JWebGPULoader.init(backend, (isSuccess, e) -> {
            System.out.println("WebGPU Init Success: " + isSuccess);
            if(isSuccess) {
                wGPUInit = 1;
            }
            else {
                throw new RuntimeException(e);
            }
        });

        wgpu = new WGPUApp();

        boolean listenerStarted = false;
        boolean startupComplete = false;
        boolean surfaceStarting = false;
        try {
            while(!glfwWindowShouldClose(window)) {
                try {
                    if(wGPUInit == 3) {
                        applicationInterface.render(wgpu);
                        wgpu.checkStartupErrors();
                        wgpu.startupComplete();
                        startupComplete = true;
                    }
                    else if(wGPUInit > 0) {
                        if(wGPUInit == 1) {
                            wGPUInit = 2;
                            wgpu.width = windowWidth;
                            wgpu.height = windowHeight;
                            if(backend == JWebGPUBackend.WGPU) {
                                String os = System.getProperty("os.name", "").toLowerCase(Locale.ROOT);
                                if(os.contains("mac")) wgpu.init(WGPUBackendType.Metal);
                                else if(os.contains("win")) wgpu.init(WGPUBackendType.Vulkan, WGPUBackendType.D3D12);
                                else wgpu.init(WGPUBackendType.Vulkan, WGPUBackendType.OpenGL);
                            } else wgpu.init();
                        }
                        else if(wGPUInit == 2 && wgpu.isReady()) {
                            surfaceStarting = true;
                            wgpu.beginSurfaceStartup();
                            createSurface();
                            listenerStarted = true;
                            applicationInterface.create(wgpu);
                            wgpu.checkStartupErrors();
                            wGPUInit = 3;
                        }
                    }
                    wgpu.update();
                } catch(RuntimeException error) {
                    if(startupComplete || !surfaceStarting) throw error;
                    if(listenerStarted) {
                        listenerStarted = false;
                        applicationInterface.dispose();
                    }
                    wgpu.failStartup(error.getMessage());
                    surfaceStarting = false;
                    wGPUInit = 2;
                }
                glfwPollEvents();
            }
        } finally {
            try {
                if(listenerStarted) applicationInterface.dispose();
            } finally {
                wgpu.dispose();
                closeWindow();
            }
        }
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
        window = glfwCreateWindow(windowWidth, windowHeight, createWindowTitle(), NULL, NULL);
        if (window == NULL) {
            glfwTerminate();
            throw new RuntimeException("Failed to create GLFW window");
        }

        String osName = System.getProperty("os.name").toLowerCase();
        if(osName.contains("win")) {
            windowHandle = glfwGetWin32Window(window);
        }
        else if(osName.contains("linux")) {
            if(glfwGetPlatform() == org.lwjgl.glfw.GLFW.GLFW_PLATFORM_WAYLAND) {
                windowHandle = glfwGetWaylandWindow(window);
            }
            else {
                windowHandle = glfwGetX11Window(window);
            }
        }
        else if(osName.contains("mac")) {
            windowHandle = glfwGetCocoaWindow(window);
        }

        if(glfwGetPlatform() != GLFW_PLATFORM_WAYLAND) {
            GLFWVidMode vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwSetWindowPos(
                    window,
                    (vidMode.width() - windowWidth) / 2,
                    (vidMode.height() - windowHeight) / 2
            );
        }

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
        String osName = System.getProperty("os.name").toLowerCase();
        NativeObject voidHandle = NativeObject.native_new().native_setAddress(windowHandle);
        if(osName.contains("win")) {
            wgpu.surface = wgpu.instance.createWindowsSurface(voidHandle);
        }
        else if(osName.contains("linux")) {
            NativeObject displayVoid = NativeObject.native_new();
            if(glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
                long display = glfwGetWaylandDisplay();
                displayVoid.native_setAddress(display);
                wgpu.surface = wgpu.instance.createLinuxSurface(true, voidHandle, displayVoid);
            }
            else {
                long display = glfwGetX11Display();
                displayVoid.native_setAddress(display);
                wgpu.surface = wgpu.instance.createLinuxSurface(false, voidHandle, displayVoid);
            }
        }
        else if(osName.contains("mac")) {
            wgpu.surface = wgpu.instance.createMacSurface(voidHandle);
        }
    }

    private JWebGPUBackend resolveBackend() {
        String backendName = System.getProperty("jwebgpu.backend", "wgpu").trim();
        if(backendName.isEmpty()) {
            return JWebGPUBackend.WGPU;
        }
        try {
            return JWebGPUBackend.valueOf(backendName.toUpperCase(Locale.ROOT));
        }
        catch(IllegalArgumentException e) {
            throw new IllegalArgumentException("Unsupported jwebgpu.backend: " + backendName + ". Expected wgpu or dawn.", e);
        }
    }

    private String resolveBridgeName() {
        String bridge = System.getProperty("jwebgpu.bridge", "Desktop").trim();
        if(bridge.isEmpty()) {
            return "Desktop";
        }
        return bridge.toUpperCase(Locale.ROOT);
    }

    private String createWindowTitle() {
        return "jWebGPU Demo - " + bridgeName + " " + formatBackendName(backend);
    }

    private String formatBackendName(JWebGPUBackend backend) {
        if(backend == JWebGPUBackend.DAWN) {
            return "Dawn";
        }
        return "WGPU";
    }
}
