package com.github.xpenatan.webgpu;

import com.github.xpenatan.jParser.idl.IDLBase;
import java.nio.ByteBuffer;

public class WGPUQueue extends IDLBase {

    public final static WGPUQueue NULL = native_new();

    public static WGPUQueue native_new() {
        return new WGPUQueue((byte) 0, (char) 0);
    }

    private WGPUQueue(byte v, char c) {
    }


    /*[-TEAVM;-ADD_RAW]
        @Override
        protected void onNativeAddressChanged() {
            int cPointer = native_address;
            native_object = internal_native_getJsObject(cPointer);
        }

        @org.teavm.jso.JSBody(params = { "this_addr" }, script = "" +
            "var jsObj = [MODULE].wrapPointer(this_addr, [MODULE].WGPUQueue);" +
            "var nativeObject = [MODULE].WebGPU.getJsObject(jsObj.Get());" +
            "return nativeObject;"
        )
        private static native org.teavm.jso.JSObject internal_native_getJsObject(int this_addr);
    */

    /*[-TEAVM;-REPLACE_BLOCK]
        {
            org.teavm.jso.typedarrays.Int8Array array = org.teavm.jso.typedarrays.Int8Array.fromJavaBuffer(byteBuffer);
            internal_native_WriteBuffer(native_object, buffer.native_object, bufferOffset, array, dataSize);
        }
    */
    public void writeBuffer(WGPUBuffer buffer, int bufferOffset, ByteBuffer byteBuffer, int dataSize) {
        internal_native_WriteBuffer(native_address, (buffer != null ? buffer.native_address : 0), bufferOffset, byteBuffer, dataSize);
    }

    /*[-TEAVM;-REPLACE]
        @org.teavm.jso.JSBody(params = { "this_addr", "buffer_addr", "bufferOffset", "bytes_addr", "dataSize" }, script = "" +
            "this_addr.writeBuffer(buffer_addr, bufferOffset, bytes_addr, 0, dataSize);"
        )
        private static native void internal_native_WriteBuffer(org.teavm.jso.JSObject this_addr, org.teavm.jso.JSObject buffer_addr, int bufferOffset, org.teavm.jso.JSObject bytes_addr, int dataSize);
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
            internal_native_WriteTexture(native_object, destination.native_address, array, dataSize, dataLayout.native_address, writeSize.native_address);
        }
    */
    public void writeTexture(WGPUTexelCopyTextureInfo destination, ByteBuffer byteBuffer, int dataSize, WGPUTexelCopyBufferLayout dataLayout, WGPUExtent3D writeSize) {
        internal_native_WriteTexture(native_address, (destination != null ? destination.native_address : 0), byteBuffer, dataSize, dataLayout.native_address, writeSize.native_address);
    }

    /*[-TEAVM;-REPLACE]
        @org.teavm.jso.JSBody(params = {"this_addr", "destination_addr", "bytes_addr", "dataSize", "dataLayout_addr", "writeSize_addr"}, script = "" +
            "var destinationJS = [MODULE].wrapPointer(destination_addr, [MODULE].WGPUTexelCopyTextureInfo);" +
            "var dataLayoutJS = [MODULE].wrapPointer(dataLayout_addr, [MODULE].WGPUTexelCopyBufferLayout);" +
            "var writeSizeJS = [MODULE].wrapPointer(writeSize_addr, [MODULE].WGPUExtent3D);" +
            "var destination = [MODULE].WebGPU.makeTexelCopyTextureInfo(destinationJS.GetPtr());" +
            "var dataLayout = [MODULE].WebGPU.makeTexelCopyBufferLayout(dataLayoutJS.Get());" +
            "var writeSize = [MODULE].WebGPU.makeExtent3D(writeSizeJS.Get());" +
            "this_addr.writeTexture(destination, bytes_addr, dataLayout, writeSize);"
        )
        private static native void internal_native_WriteTexture(org.teavm.jso.JSObject this_addr, int destination_addr, org.teavm.jso.JSObject bytes_addr, int dataSize, int dataLayout_addr, int writeSize_addr);
     */
    /*[-JNI;-NATIVE]
        JGPU::WGPUQueue* nativeObject = (JGPU::WGPUQueue*)this_addr;
        void* bufferAddress = env->GetDirectBufferAddress(byteBuffer);
        nativeObject->WriteTexture((JGPU::WGPUTexelCopyTextureInfo* )destination_addr, bufferAddress, (int)dataSize, (JGPU::WGPUTexelCopyBufferLayout* )dataLayout_addr, (JGPU::WGPUExtent3D* )writeSize_addr);
    */
    private static native void internal_native_WriteTexture(long this_addr, long destination_addr, ByteBuffer byteBuffer, int dataSize, long dataLayout_addr, long writeSize_addr);
}