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

    /*[-FFM;-NATIVE]
        extern "C" {
        FFM_EXPORT void jparser_com_github_xpenatan_webgpu_WGPUBuffer_internal_1native_1getConstMappedRangeFFM__JIIJ(int64_t this_addr, int32_t offset, int32_t size, void* out_ptr) {
            JGPU::WGPUBuffer* nativeObject = (JGPU::WGPUBuffer*)this_addr;
            nativeObject->GetConstMappedRange((int)offset, (int)size, out_ptr);
        }
        }
    */

    /*[-FFM;-ADD]
        private static final class FFMBufferHandles {
            private static final java.lang.foreign.SymbolLookup LOOKUP = java.lang.foreign.SymbolLookup.loaderLookup();
            private static final java.lang.foreign.Linker LINKER = java.lang.foreign.Linker.nativeLinker();

            static final java.lang.invoke.MethodHandle getConstMappedRange = LINKER.downcallHandle(
                    LOOKUP.find("jparser_com_github_xpenatan_webgpu_WGPUBuffer_internal_1native_1getConstMappedRangeFFM__JIIJ").orElseThrow(),
                    java.lang.foreign.FunctionDescriptor.ofVoid(
                            java.lang.foreign.ValueLayout.JAVA_LONG,
                            java.lang.foreign.ValueLayout.JAVA_INT,
                            java.lang.foreign.ValueLayout.JAVA_INT,
                            java.lang.foreign.ValueLayout.ADDRESS
                    )
            );
        }
    */

    /*[-TEAVM;-ADD_RAW]
        @Override
        protected void onNativeAddressChanged() {
            int cPointer = native_address;
            native_object = internal_native_getJsObject(cPointer);
        }

        @org.teavm.jso.JSBody(params = { "this_addr" }, script = "" +
            "var jsObj = [MODULE].wrapPointer(this_addr, [MODULE].WGPUBuffer);" +
            "var jsAddr = jsObj.Get();" +
            "var nativeObject = idl.WebGPU.getJsObject(jsAddr);" +
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
    /*[-FFM;-REPLACE_BLOCK]
        {
            java.lang.foreign.MemorySegment seg = java.lang.foreign.MemorySegment.ofBuffer(out);
            internal_native_getConstMappedRange(native_address, offset, size, seg);
        }
    */
    public void getConstMappedRange(int offset, int size, ByteBuffer out) {
        internal_native_getConstMappedRange(native_address, offset, size, out);
    }

    /*[-TEAVM;-REPLACE]
        @org.teavm.jso.JSBody(params = {"this_addr", "offset", "size", "bytes_addr"}, script = "" +
            "var jsObj = [MODULE].wrapPointer(this_addr, [MODULE].WGPUBuffer);" +
            "var ptr = idl._malloc(size);" +
            "jsObj.GetConstMappedRange(offset, size, ptr);" +
            "var dataOut = idl.HEAPU8.subarray(ptr, ptr + size);" +
            "bytes_addr.set(dataOut);" +
            "idl._free(ptr);"
        )
        private static native void internal_native_getConstMappedRange(int this_addr, int offset, int size, org.teavm.jso.JSObject bytes_addr);
    */
    /*[-JNI;-NATIVE]
        JGPU::WGPUBuffer* nativeObject = (JGPU::WGPUBuffer*)this_addr;
        void* bufferAddress = env->GetDirectBufferAddress(out);
        nativeObject->GetConstMappedRange((int)offset, (int)size, bufferAddress);
    */
    /*[-FFM;-REPLACE]
        private static void internal_native_getConstMappedRange(long this_addr, int offset, int size, java.lang.foreign.MemorySegment out_ptr) {
            try {
                FFMBufferHandles.getConstMappedRange.invokeExact(this_addr, offset, size, out_ptr);
            }
            catch(Throwable e) {
                throw new RuntimeException(e);
            }
        }
    */
    private static native void internal_native_getConstMappedRange(long this_addr, int offset, int size, ByteBuffer out);
}
