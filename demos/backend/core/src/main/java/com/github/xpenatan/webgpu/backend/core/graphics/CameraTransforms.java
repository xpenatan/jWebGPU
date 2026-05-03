package com.github.xpenatan.webgpu.backend.core.graphics;

public class CameraTransforms {

    public static void buildModelViewProjection(
            float[] modelOut,
            float[] mvpOut,
            float angle,
            float aspect
    ) {
        float[] rotY = new float[16];
        float[] rotX = new float[16];
        float[] tmp = new float[16];
        float[] view = new float[16];
        float[] projection = new float[16];

        Mat4f.rotateY(rotY, angle);
        Mat4f.rotateX(rotX, angle * 0.65f);
        Mat4f.multiply(modelOut, rotY, rotX);

        Mat4f.lookAt(view, 1.8f, 1.3f, 2.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        Mat4f.perspective(projection, (float)Math.toRadians(45.0), aspect, 0.1f, 20.0f);

        Mat4f.multiply(tmp, view, modelOut);
        Mat4f.multiply(mvpOut, projection, tmp);
    }
}


