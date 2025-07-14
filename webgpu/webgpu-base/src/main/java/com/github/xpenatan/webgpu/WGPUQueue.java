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
        void* bufferAddress = env->GetDirectBufferAddress(byteBuffer);
        nativeObject->WriteBuffer((JGPU::WGPUBuffer* )buffer_addr, (int)bufferOffset, bufferAddress, (int)dataSize);
    */
    private static native void internal_native_WriteBuffer(long this_addr, long buffer_addr, int bufferOffset, ByteBuffer byteBuffer, int dataSize);

    /*[-TEAVM;-REPLACE_BLOCK]
        {
            org.teavm.jso.typedarrays.Int8Array array = org.teavm.jso.typedarrays.Int8Array.fromJavaBuffer(byteBuffer);
            internal_native_WriteBuffer((int)getNativeData().getCPointer(), (int)buffer.getNativeData().getCPointer(), bufferOffset, array, dataSize, (int)dataLayout.getNativeData().getCPointer(), (int)writeSize.getNativeData().getCPointer());
        }
    */
    public void writeTexture(WGPUTexelCopyTextureInfo destination, ByteBuffer byteBuffer, int dataSize, WGPUTexelCopyBufferLayout dataLayout, WGPUExtent3D writeSize) {
        internal_native_WriteTexture((long) getNativeData().getCPointer(), (long) (destination != null ? destination.getNativeData().getCPointer() : 0), byteBuffer, dataSize, dataLayout.getNativeData().getCPointer(), writeSize.getNativeData().getCPointer());
    }

    /*[-TEAVM;-REPLACE]
        @org.teavm.jso.JSBody(params = {"this_addr", "destination_addr", "bytes_addr", "dataSize", "dataLayout_addr", "writeSize_addr"}, script = "var jsObj = jWebGPU.wrapPointer(this_addr, jWebGPU.WGPUQueue);jsObj.WriteTexture_1(destination_addr, bytes_addr, dataSize, dataLayout_addr, writeSize_addr);")
        private static native void internal_native_WriteTexture(int this_addr, int destination_addr, org.teavm.jso.JSObject bytes_addr, int dataSize, int dataLayout_addr, int writeSize_addr);
     */
    /*[-JNI;-NATIVE]
        JGPU::WGPUQueue* nativeObject = (JGPU::WGPUQueue*)this_addr;
        void* bufferAddress = env->GetDirectBufferAddress(byteBuffer);
        nativeObject->WriteTexture((JGPU::WGPUTexelCopyTextureInfo* )destination_addr, bufferAddress, (int)dataSize, (JGPU::WGPUTexelCopyBufferLayout* )dataLayout_addr, (JGPU::WGPUExtent3D* )writeSize_addr);
    */
    private static native void internal_native_WriteTexture(long this_addr, long destination_addr, ByteBuffer byteBuffer, int dataSize, long dataLayout_addr, long writeSize_addr);
}