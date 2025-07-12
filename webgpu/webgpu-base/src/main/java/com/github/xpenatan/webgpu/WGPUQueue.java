package com.github.xpenatan.webgpu;

import idl.IDLBase;
import idl.helper.IDLByteArray;
import java.nio.ByteBuffer;

public class WGPUQueue extends IDLBase {


    /*[-TEAVM;-REPLACE_BLOCK]
        {
            org.teavm.jso.typedarrays.Int8Array array = org.teavm.jso.typedarrays.Int8Array.fromJavaBuffer(byteBuffer);
            internal_native_WriteBuffer((int)getNativeData().getCPointer(), (int)buffer.getNativeData().getCPointer(), bufferOffset, array, dataSize);
        }
    */
    public void writeBuffer(WGPUBuffer buffer, int bufferOffset, ByteBuffer byteBuffer, int dataSize) {
        internal_native_WriteBuffer((long) getNativeData().getCPointer(), (long) (buffer != null ? buffer.getNativeData().getCPointer() : 0), bufferOffset, byteBuffer, dataSize);
    }

    /*[-TEAVM;-REPLACE]
        @org.teavm.jso.JSBody(params = {"this_addr", "buffer_addr", "bufferOffset", "bytes_addr", "dataSize"}, script = "var jsObj = jWebGPU.wrapPointer(this_addr, jWebGPU.WGPUQueue);jsObj.WriteBuffer_1(buffer_addr, bufferOffset, bytes_addr, dataSize);")
        private static native void internal_native_WriteBuffer(int this_addr, int buffer_addr, int bufferOffset, org.teavm.jso.JSObject bytes_addr, int dataSize);
     */
    /*[-JNI;-NATIVE]
        JGPU::WGPUQueue* nativeObject = (JGPU::WGPUQueue*)this_addr;
        // Get the memory address of the direct buffer
        void* bufferAddress = env->GetDirectBufferAddress(byteBuffer);
        unsigned char* data = static_cast<unsigned char*>(bufferAddress);
        nativeObject->WriteBuffer((JGPU::WGPUBuffer* )buffer_addr, (int)bufferOffset, data, (int)dataSize);
    */
    private static native void internal_native_WriteBuffer(long this_addr, long buffer_addr, int bufferOffset, ByteBuffer byteBuffer, int dataSize);

    /*[-TEAVM;-REMOVE]*/
    /*[-JNI;-REMOVE]*/
    public void writeBuffer(WGPUBuffer buffer, int bufferOffset, IDLByteArray bytes, int dataSize) {
    }
}