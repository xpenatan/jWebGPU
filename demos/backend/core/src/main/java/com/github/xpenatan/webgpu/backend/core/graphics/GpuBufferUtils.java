package com.github.xpenatan.webgpu.backend.core.graphics;

import com.github.xpenatan.webgpu.WGPUBuffer;
import com.github.xpenatan.webgpu.WGPUBufferDescriptor;
import com.github.xpenatan.webgpu.WGPUBufferUsage;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class GpuBufferUtils {

    public static WGPUBuffer createBuffer(WGPUApp app, int size, WGPUBufferUsage usage) {
        WGPUBufferDescriptor descriptor = WGPUBufferDescriptor.obtain();
        descriptor.setSize(size);
        descriptor.setUsage(usage);
        descriptor.setMappedAtCreation(false);
        return app.device.createBuffer(descriptor);
    }

    public static void upload(WGPUApp app, WGPUBuffer buffer, ByteBuffer data) {
        app.queue.writeBuffer(buffer, 0, data, data.limit());
    }

    public static ByteBuffer createFloatBuffer(float[] values) {
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(values.length * Float.BYTES);
        byteBuffer.order(ByteOrder.LITTLE_ENDIAN);
        FloatBuffer floatBuffer = byteBuffer.asFloatBuffer();
        floatBuffer.put(values);
        return byteBuffer;
    }

    public static ByteBuffer createShortBuffer(short[] values) {
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(values.length * Short.BYTES);
        byteBuffer.order(ByteOrder.LITTLE_ENDIAN);
        for(short value : values) {
            byteBuffer.putShort(value);
        }
        return byteBuffer;
    }
}


