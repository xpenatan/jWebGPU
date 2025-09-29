package com.github.xpenatan.webgpu;

import com.github.xpenatan.jParser.idl.IDLBase;
import java.nio.ByteBuffer;

public class WGPUBuffer extends IDLBase {

    public final static WGPUBuffer NULL = native_new();

    public static WGPUBuffer native_new() {
        return new WGPUBuffer((byte) 0, (char) 0);
    }

    private WGPUBuffer(byte v, char c) {
    }

    /*[-TEAVM;-ADD_RAW]
        @Override
        protected void onNativeAddressChanged() {
            int cPointer = native_address;
            native_object = internal_native_getJsObject(cPointer);
        }

        @org.teavm.jso.JSBody(params = { "this_addr" }, script = "" +
            "var jsObj = [MODULE].wrapPointer(this_addr, [MODULE].WGPUBuffer);" +
            "var jsAddr = jsObj.Get();" +
            "var nativeObject = [MODULE].WebGPU.getJsObject(jsAddr);" +
            "return nativeObject;"
        )
        private static native org.teavm.jso.JSObject internal_native_getJsObject(int this_addr);
    */


    /*[-TEAVM;-REPLACE_BLOCK]
        {
            org.teavm.jso.typedarrays.Int8Array array = org.teavm.jso.typedarrays.Int8Array.fromJavaBuffer(out);
            internal_native_getConstMappedRange((int)native_address, offset, size, array);
        }
    */
    public void getConstMappedRange(int offset, int size, ByteBuffer out) {
        internal_native_getConstMappedRange(native_address, offset, size, out);
    }

    /*[-TEAVM;-REPLACE]
        @org.teavm.jso.JSBody(params = {"this_addr", "offset", "size", "bytes_addr"}, script = "var jsObj = [MODULE].wrapPointer(this_addr, [MODULE].WGPUBuffer); var ptr = [MODULE]._malloc(size); jsObj.GetConstMappedRange(offset, size, ptr); var dataOut = [MODULE].HEAPU8.subarray(ptr, ptr + size); bytes_addr.set(dataOut); [MODULE]._free(ptr);")
        private static native void internal_native_getConstMappedRange(int this_addr, int offset, int size, org.teavm.jso.JSObject bytes_addr);
    */
    /*[-JNI;-NATIVE]
        JGPU::WGPUBuffer* nativeObject = (JGPU::WGPUBuffer*)this_addr;
        void* bufferAddress = env->GetDirectBufferAddress(out);
        nativeObject->GetConstMappedRange((int)offset, (int)size, bufferAddress);
    */
    private static native void internal_native_getConstMappedRange(long this_addr, int offset, int size, ByteBuffer out);
}
