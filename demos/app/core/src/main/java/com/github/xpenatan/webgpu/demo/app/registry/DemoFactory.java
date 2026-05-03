package com.github.xpenatan.webgpu.demo.app.registry;

import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.graphics.ParametricMeshGenerator;
import com.github.xpenatan.webgpu.demo.app.demos.mesh.RotatingLitCubeDemo;
import com.github.xpenatan.webgpu.demo.app.demos.mesh.RotatingLitMeshDemo;
import com.github.xpenatan.webgpu.demo.app.demos.mesh.StudioSceneDemo;
import com.github.xpenatan.webgpu.demo.app.demos.tutorial.AFirstVertexAttribute;
import com.github.xpenatan.webgpu.demo.app.demos.tutorial.HelloTriangle;
import com.github.xpenatan.webgpu.demo.app.demos.tutorial.PlayingWithBuffers;

public class DemoFactory {

    public static DemoId defaultDemo() {
        return DemoId.STUDIO;
    }

    public static DemoId resolveDemoId(String value) {
        if(value == null) {
            return defaultDemo();
        }
        String normalized = value.trim().toLowerCase();
        if(normalized.isEmpty()) {
            return defaultDemo();
        }

        switch(normalized) {
            case "cube":
                return DemoId.CUBE;
            case "cone":
                return DemoId.CONE;
            case "sphere":
                return DemoId.SPHERE;
            case "torus":
            case "donut":
                return DemoId.TORUS;
            case "studio":
            case "showcase":
                return DemoId.STUDIO;
            case "hello-triangle":
            case "triangle":
                return DemoId.HELLO_TRIANGLE;
            case "first-vertex-attribute":
            case "vertex-attribute":
            case "vertex":
                return DemoId.FIRST_VERTEX_ATTRIBUTE;
            case "playing-with-buffers":
            case "buffers":
                return DemoId.PLAYING_WITH_BUFFERS;
            default:
                return defaultDemo();
        }
    }

    public static ApplicationListener create(DemoId demoId) {
        switch(demoId) {
            case CUBE:
                return new RotatingLitCubeDemo();
            case CONE:
                return new RotatingLitMeshDemo(
                        ParametricMeshGenerator.cone(0.58f, 1.25f, 36, 1),
                        0.92f,
                        0.35f,
                        0.22f
                );
            case SPHERE:
                return new RotatingLitMeshDemo(
                        ParametricMeshGenerator.sphere(0.58f, 24, 32),
                        0.58f,
                        0.30f,
                        0.78f
                );
            case TORUS:
                return new RotatingLitMeshDemo(
                        ParametricMeshGenerator.torus(0.52f, 0.20f, 48, 24),
                        0.10f,
                        0.40f,
                        0.90f
                );
            case STUDIO:
                return new StudioSceneDemo();
            case HELLO_TRIANGLE:
                return new HelloTriangle();
            case FIRST_VERTEX_ATTRIBUTE:
                return new AFirstVertexAttribute();
            case PLAYING_WITH_BUFFERS:
                return new PlayingWithBuffers();
            default:
                return new StudioSceneDemo();
        }
    }
}


