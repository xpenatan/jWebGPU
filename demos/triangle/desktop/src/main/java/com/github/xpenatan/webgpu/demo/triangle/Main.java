package com.github.xpenatan.webgpu.demo.triangle;

import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.glfw.GLFW;
import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.opengl.GL;
import org.lwjgl.opengl.GL11;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.system.MemoryUtil.NULL;

public class Main {
    public static void main(String[] args) {
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

        // Create the window
        int windowWidth = 800;
        int windowHeight = 600;
        long window = glfwCreateWindow(windowWidth, windowHeight, "LWJGL Window", NULL, NULL);
        if (window == NULL) {
            glfwTerminate();
            throw new RuntimeException("Failed to create GLFW window");
        }

        // Center the window
        var vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(
                window,
                (vidMode.width() - windowWidth) / 2,
                (vidMode.height() - windowHeight) / 2
        );

        // Make the OpenGL context current
        glfwMakeContextCurrent(window);
        // Enable v-sync
        glfwSwapInterval(1);

        // Make the window visible
        glfwShowWindow(window);

        // Initialize OpenGL capabilities
        GL.createCapabilities();

        // Set the clear color (black)
        GL11.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        boolean init = true;

        // Run the rendering loop until the window is closed
        while (!glfwWindowShouldClose(window)) {
            // Clear the framebuffer
            GL11.glClear(GL11.GL_COLOR_BUFFER_BIT | GL11.GL_DEPTH_BUFFER_BIT);
            if(init) {
                init = false;
                TriangleApp triangleApp = new TriangleApp();
                triangleApp.create();
            }

            // Swap the color buffers
            glfwSwapBuffers(window);

            // Poll for window events
            glfwPollEvents();
        }

        // Clean up
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}