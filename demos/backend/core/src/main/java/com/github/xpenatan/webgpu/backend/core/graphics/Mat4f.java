package com.github.xpenatan.webgpu.backend.core.graphics;

public class Mat4f {
    public static void identity(float[] out) {
        for(int i = 0; i < 16; i++) {
            out[i] = 0.0f;
        }
        out[0] = 1.0f;
        out[5] = 1.0f;
        out[10] = 1.0f;
        out[15] = 1.0f;
    }

    public static void multiply(float[] out, float[] a, float[] b) {
        for(int col = 0; col < 4; col++) {
            int colOffset = col * 4;
            for(int row = 0; row < 4; row++) {
                out[colOffset + row] =
                        a[row] * b[colOffset] +
                        a[4 + row] * b[colOffset + 1] +
                        a[8 + row] * b[colOffset + 2] +
                        a[12 + row] * b[colOffset + 3];
            }
        }
    }

    public static void perspective(float[] out, float fovyRad, float aspect, float near, float far) {
        float f = (float)(1.0 / Math.tan(fovyRad * 0.5));
        for(int i = 0; i < 16; i++) {
            out[i] = 0.0f;
        }
        out[0] = f / aspect;
        out[5] = f;
        out[10] = far / (near - far);
        out[11] = -1.0f;
        out[14] = (far * near) / (near - far);
    }

    public static void orthographic(float[] out, float left, float right, float bottom, float top, float near, float far) {
        for(int i = 0; i < 16; i++) {
            out[i] = 0.0f;
        }
        out[0] = 2.0f / (right - left);
        out[5] = 2.0f / (top - bottom);
        out[10] = 1.0f / (near - far);
        out[12] = (left + right) / (left - right);
        out[13] = (bottom + top) / (bottom - top);
        out[14] = near / (near - far);
        out[15] = 1.0f;
    }

    public static void rotateX(float[] out, float radians) {
        identity(out);
        float c = (float)Math.cos(radians);
        float s = (float)Math.sin(radians);
        out[5] = c;
        out[6] = s;
        out[9] = -s;
        out[10] = c;
    }

    public static void rotateY(float[] out, float radians) {
        identity(out);
        float c = (float)Math.cos(radians);
        float s = (float)Math.sin(radians);
        out[0] = c;
        out[2] = -s;
        out[8] = s;
        out[10] = c;
    }

    public static void rotateZ(float[] out, float radians) {
        identity(out);
        float c = (float)Math.cos(radians);
        float s = (float)Math.sin(radians);
        out[0] = c;
        out[1] = s;
        out[4] = -s;
        out[5] = c;
    }

    public static void lookAt(float[] out, float eyeX, float eyeY, float eyeZ, float cx, float cy, float cz, float upX, float upY, float upZ) {
        float zx = eyeX - cx;
        float zy = eyeY - cy;
        float zz = eyeZ - cz;
        float zLen = (float)Math.sqrt(zx * zx + zy * zy + zz * zz);
        zx /= zLen;
        zy /= zLen;
        zz /= zLen;

        float xx = upY * zz - upZ * zy;
        float xy = upZ * zx - upX * zz;
        float xz = upX * zy - upY * zx;
        float xLen = (float)Math.sqrt(xx * xx + xy * xy + xz * xz);
        xx /= xLen;
        xy /= xLen;
        xz /= xLen;

        float yx = zy * xz - zz * xy;
        float yy = zz * xx - zx * xz;
        float yz = zx * xy - zy * xx;

        identity(out);
        out[0] = xx;
        out[1] = yx;
        out[2] = zx;
        out[4] = xy;
        out[5] = yy;
        out[6] = zy;
        out[8] = xz;
        out[9] = yz;
        out[10] = zz;
        out[12] = -(xx * eyeX + xy * eyeY + xz * eyeZ);
        out[13] = -(yx * eyeX + yy * eyeY + yz * eyeZ);
        out[14] = -(zx * eyeX + zy * eyeY + zz * eyeZ);
    }
}


