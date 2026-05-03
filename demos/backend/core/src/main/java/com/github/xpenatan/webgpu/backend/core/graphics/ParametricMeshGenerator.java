package com.github.xpenatan.webgpu.backend.core.graphics;

import java.util.ArrayList;
import java.util.List;

public class ParametricMeshGenerator {

    public static MeshData plane(float width, float depth) {
        float hx = width * 0.5f;
        float hz = depth * 0.5f;

        float[] vertices = {
                -hx, 0.0f, -hz, 0.0f, 1.0f, 0.0f,
                hx, 0.0f, -hz, 0.0f, 1.0f, 0.0f,
                hx, 0.0f, hz, 0.0f, 1.0f, 0.0f,
                -hx, 0.0f, hz, 0.0f, 1.0f, 0.0f
        };

        short[] indices = {
                0, 1, 2,
                0, 2, 3
        };
        return new MeshData(vertices, indices);
    }

    public static MeshData cube(float size) {
        float h = size * 0.5f;
        float[] vertices = {
                h, -h, -h, 1.0f, 0.0f, 0.0f,
                h, h, -h, 1.0f, 0.0f, 0.0f,
                h, h, h, 1.0f, 0.0f, 0.0f,
                h, -h, h, 1.0f, 0.0f, 0.0f,

                -h, -h, h, -1.0f, 0.0f, 0.0f,
                -h, h, h, -1.0f, 0.0f, 0.0f,
                -h, h, -h, -1.0f, 0.0f, 0.0f,
                -h, -h, -h, -1.0f, 0.0f, 0.0f,

                -h, h, -h, 0.0f, 1.0f, 0.0f,
                h, h, -h, 0.0f, 1.0f, 0.0f,
                h, h, h, 0.0f, 1.0f, 0.0f,
                -h, h, h, 0.0f, 1.0f, 0.0f,

                -h, -h, h, 0.0f, -1.0f, 0.0f,
                h, -h, h, 0.0f, -1.0f, 0.0f,
                h, -h, -h, 0.0f, -1.0f, 0.0f,
                -h, -h, -h, 0.0f, -1.0f, 0.0f,

                -h, -h, h, 0.0f, 0.0f, 1.0f,
                h, -h, h, 0.0f, 0.0f, 1.0f,
                h, h, h, 0.0f, 0.0f, 1.0f,
                -h, h, h, 0.0f, 0.0f, 1.0f,

                h, -h, -h, 0.0f, 0.0f, -1.0f,
                -h, -h, -h, 0.0f, 0.0f, -1.0f,
                -h, h, -h, 0.0f, 0.0f, -1.0f,
                h, h, -h, 0.0f, 0.0f, -1.0f,
        };

        short[] indices = {
                0, 1, 2, 0, 2, 3,
                4, 5, 6, 4, 6, 7,
                8, 9, 10, 8, 10, 11,
                12, 13, 14, 12, 14, 15,
                16, 17, 18, 16, 18, 19,
                20, 21, 22, 20, 22, 23
        };
        return new MeshData(vertices, indices);
    }

    public static MeshData sphere(float radius, int stacks, int slices) {
        int vStacks = Math.max(3, stacks);
        int vSlices = Math.max(3, slices);

        List<Float> vertices = new ArrayList<>();
        List<Short> indices = new ArrayList<>();

        for(int stack = 0; stack <= vStacks; stack++) {
            float v = (float)stack / (float)vStacks;
            float phi = (float)Math.PI * v;
            float y = (float)Math.cos(phi);
            float ring = (float)Math.sin(phi);

            for(int slice = 0; slice <= vSlices; slice++) {
                float u = (float)slice / (float)vSlices;
                float theta = (float)(Math.PI * 2.0) * u;
                float x = (float)Math.cos(theta) * ring;
                float z = (float)Math.sin(theta) * ring;

                vertices.add(x * radius);
                vertices.add(y * radius);
                vertices.add(z * radius);
                vertices.add(x);
                vertices.add(y);
                vertices.add(z);
            }
        }

        int row = vSlices + 1;
        for(int stack = 0; stack < vStacks; stack++) {
            for(int slice = 0; slice < vSlices; slice++) {
                int i0 = stack * row + slice;
                int i1 = i0 + 1;
                int i2 = i0 + row;
                int i3 = i2 + 1;

                addTriangle(indices, i0, i2, i1);
                addTriangle(indices, i1, i2, i3);
            }
        }
        return new MeshData(toFloatArray(vertices), toShortArray(indices));
    }

    public static MeshData torus(float majorRadius, float minorRadius, int majorSegments, int minorSegments) {
        int mSeg = Math.max(3, majorSegments);
        int nSeg = Math.max(3, minorSegments);

        List<Float> vertices = new ArrayList<>();
        List<Short> indices = new ArrayList<>();

        for(int i = 0; i <= mSeg; i++) {
            float u = (float)i / (float)mSeg;
            float a = (float)(Math.PI * 2.0) * u;
            float cosA = (float)Math.cos(a);
            float sinA = (float)Math.sin(a);

            for(int j = 0; j <= nSeg; j++) {
                float v = (float)j / (float)nSeg;
                float b = (float)(Math.PI * 2.0) * v;
                float cosB = (float)Math.cos(b);
                float sinB = (float)Math.sin(b);

                float cx = majorRadius * cosA;
                float cz = majorRadius * sinA;

                float nx = cosA * cosB;
                float ny = sinB;
                float nz = sinA * cosB;

                float px = cx + nx * minorRadius;
                float py = ny * minorRadius;
                float pz = cz + nz * minorRadius;

                vertices.add(px);
                vertices.add(py);
                vertices.add(pz);
                vertices.add(nx);
                vertices.add(ny);
                vertices.add(nz);
            }
        }

        int row = nSeg + 1;
        for(int i = 0; i < mSeg; i++) {
            for(int j = 0; j < nSeg; j++) {
                int i0 = i * row + j;
                int i1 = i0 + 1;
                int i2 = i0 + row;
                int i3 = i2 + 1;
                addTriangle(indices, i0, i2, i1);
                addTriangle(indices, i1, i2, i3);
            }
        }
        return new MeshData(toFloatArray(vertices), toShortArray(indices));
    }

    public static MeshData cone(float radius, float height, int segments, int stacks) {
        int seg = Math.max(3, segments);
        int st = Math.max(1, stacks);

        List<Float> vertices = new ArrayList<>();
        List<Short> indices = new ArrayList<>();

        for(int y = 0; y <= st; y++) {
            float t = (float)y / (float)st;
            float currRadius = radius * (1.0f - t);
            float currY = -height * 0.5f + height * t;
            for(int i = 0; i <= seg; i++) {
                float u = (float)i / (float)seg;
                float a = (float)(Math.PI * 2.0) * u;
                float cos = (float)Math.cos(a);
                float sin = (float)Math.sin(a);

                float px = cos * currRadius;
                float pz = sin * currRadius;

                float ny = radius / height;
                float len = (float)Math.sqrt(cos * cos + ny * ny + sin * sin);
                float nx = cos / len;
                float nnY = ny / len;
                float nz = sin / len;

                vertices.add(px);
                vertices.add(currY);
                vertices.add(pz);
                vertices.add(nx);
                vertices.add(nnY);
                vertices.add(nz);
            }
        }

        int row = seg + 1;
        for(int y = 0; y < st; y++) {
            for(int i = 0; i < seg; i++) {
                int i0 = y * row + i;
                int i1 = i0 + 1;
                int i2 = i0 + row;
                int i3 = i2 + 1;
                addTriangle(indices, i0, i2, i1);
                addTriangle(indices, i1, i2, i3);
            }
        }

        int baseCenter = vertices.size() / MeshData.VERTEX_FLOATS;
        vertices.add(0.0f);
        vertices.add(-height * 0.5f);
        vertices.add(0.0f);
        vertices.add(0.0f);
        vertices.add(-1.0f);
        vertices.add(0.0f);

        for(int i = 0; i <= seg; i++) {
            float u = (float)i / (float)seg;
            float a = (float)(Math.PI * 2.0) * u;
            float cos = (float)Math.cos(a);
            float sin = (float)Math.sin(a);
            vertices.add(cos * radius);
            vertices.add(-height * 0.5f);
            vertices.add(sin * radius);
            vertices.add(0.0f);
            vertices.add(-1.0f);
            vertices.add(0.0f);
        }

        int baseRingStart = baseCenter + 1;
        for(int i = 0; i < seg; i++) {
            addTriangle(indices, baseCenter, baseRingStart + i + 1, baseRingStart + i);
        }

        return new MeshData(toFloatArray(vertices), toShortArray(indices));
    }

    private static void addTriangle(List<Short> indices, int a, int b, int c) {
        indices.add((short)a);
        indices.add((short)b);
        indices.add((short)c);
    }

    private static float[] toFloatArray(List<Float> values) {
        float[] out = new float[values.size()];
        for(int i = 0; i < values.size(); i++) {
            out[i] = values.get(i);
        }
        return out;
    }

    private static short[] toShortArray(List<Short> values) {
        short[] out = new short[values.size()];
        for(int i = 0; i < values.size(); i++) {
            out[i] = values.get(i);
        }
        return out;
    }
}


