package com.github.xpenatan.webgpu;

import java.nio.ByteBuffer;

public class WGPUBufferUtils {

    /*[-TEAVM;-REPLACE_BLOCK]
        {
            org.teavm.classlib.impl.nio.Buffers.free(byteBuffer);
        }
    */
    public static void dispose(ByteBuffer byteBuffer) {
    }
}