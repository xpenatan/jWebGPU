package com.github.xpenatan.webgpu;

import java.nio.ByteBuffer;

public class WGPUBufferUtils {

    /*[-TEAVM;-REPLACE_BLOCK]
        {
            if(org.teavm.classlib.PlatformDetector.isWebAssemblyGC() && buffer.isDirect()) {
                org.teavm.classlib.impl.nio.Buffers.free(buffer);
            }
        }
    */
    public static void dispose(ByteBuffer buffer) {
    }
}