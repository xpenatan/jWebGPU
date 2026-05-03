package com.github.xpenatan.webgpu.backend.core.graphics;

public class MeshData {

    public static final int POSITION_FLOATS = 3;
    public static final int NORMAL_FLOATS = 3;
    public static final int VERTEX_FLOATS = POSITION_FLOATS + NORMAL_FLOATS;
    public static final int VERTEX_STRIDE_BYTES = VERTEX_FLOATS * Float.BYTES;

    public final float[] vertices;
    public final short[] indices;

    public MeshData(float[] vertices, short[] indices) {
        this.vertices = vertices;
        this.indices = indices;
    }
}


