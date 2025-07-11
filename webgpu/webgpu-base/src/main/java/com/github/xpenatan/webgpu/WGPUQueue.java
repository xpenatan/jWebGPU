package com.github.xpenatan.webgpu;

import idl.IDLBase;
import java.nio.ByteBuffer;

public class WGPUQueue extends IDLBase {

    public void writeBuffer(WGPUBuffer buffer, int bufferOffset, ByteBuffer byteBuffer, int dataSize) {
        internal_native_WriteBuffer((long) getNativeData().getCPointer(), (long) (buffer != null ? buffer.getNativeData().getCPointer() : 0), bufferOffset, byteBuffer, dataSize);
    }

    /*[-JNI;-NATIVE]
        JGPU::WGPUQueue* nativeObject = (JGPU::WGPUQueue*)this_addr;
        // Get the memory address of the direct buffer
        void* bufferAddress = env->GetDirectBufferAddress(byteBuffer);
        unsigned char* data = static_cast<unsigned char*>(bufferAddress);
        nativeObject->WriteBuffer((JGPU::WGPUBuffer* )buffer_addr, (int)bufferOffset, data, (int)dataSize);
    */
    private static native void internal_native_WriteBuffer(long this_addr, long buffer_addr, int bufferOffset, ByteBuffer byteBuffer, int dataSize);
}