package com.github.xpenatan.webgpu.demo.app.demos.mesh;

import com.github.xpenatan.webgpu.backend.core.graphics.ParametricMeshGenerator;

public class RotatingLitCubeDemo extends RotatingLitMeshDemo {

    public RotatingLitCubeDemo() {
        super(ParametricMeshGenerator.cube(1.0f), 0.22f, 0.58f, 0.95f);
    }
}


