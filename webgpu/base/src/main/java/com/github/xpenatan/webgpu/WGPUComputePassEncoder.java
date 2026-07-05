package com.github.xpenatan.webgpu;

import com.github.xpenatan.jParser.api.NativeObject;
import java.nio.ByteBuffer;

public class WGPUComputePassEncoder extends NativeObject {

    static public final WGPUComputePassEncoder NULL = WGPUComputePassEncoder.native_new();

    public static WGPUComputePassEncoder native_new() {
        return new WGPUComputePassEncoder((byte)0, (char)0);
    }

    @Deprecated
    protected WGPUComputePassEncoder(byte b, char c) {
    }

    /*[-FFM;-NATIVE]
        extern "C" {
        FFM_EXPORT void jparser_com_github_xpenatan_webgpu_WGPUComputePassEncoder_internal_1native_1SetImmediatesFFM__JIJI(int64_t this_addr, int32_t offset, int64_t data_ptr, int32_t dataSize) {
            JGPU::WGPUComputePassEncoder* nativeObject = (JGPU::WGPUComputePassEncoder*)this_addr;
            nativeObject->SetImmediates(offset, (void*)data_ptr, dataSize);
        }
        }
    */

    /*[-FFM;-ADD]
        private static final class FFMImmediateHandles {
            private static final java.lang.foreign.SymbolLookup LOOKUP = java.lang.foreign.SymbolLookup.loaderLookup();
            private static final java.lang.foreign.Linker LINKER = java.lang.foreign.Linker.nativeLinker();

            static final java.lang.invoke.MethodHandle setImmediates = LINKER.downcallHandle(
                    LOOKUP.find("jparser_com_github_xpenatan_webgpu_WGPUComputePassEncoder_internal_1native_1SetImmediatesFFM__JIJI").orElseThrow(),
                    java.lang.foreign.FunctionDescriptor.ofVoid(
                            java.lang.foreign.ValueLayout.JAVA_LONG,
                            java.lang.foreign.ValueLayout.JAVA_INT,
                            java.lang.foreign.ValueLayout.JAVA_LONG,
                            java.lang.foreign.ValueLayout.JAVA_INT
                    )
            );
        }
    */

    /*[-TEAVM;-REPLACE_BLOCK]
        {
            org.teavm.jso.typedarrays.Int8Array array = org.teavm.jso.typedarrays.Int8Array.fromJavaBuffer(byteBuffer);
            internal_native_SetImmediates(native_address, offset, array, dataSize);
        }
    */
    /*[-FFM;-REPLACE_BLOCK]
        {
            long dataAddress = com.github.xpenatan.jparser.runtime.helper.NativeUtils.address(byteBuffer);
            internal_native_SetImmediates(native_address, offset, dataAddress, dataSize);
        }
    */
    public void setImmediates(int offset, ByteBuffer byteBuffer, int dataSize) {
        internal_native_SetImmediates(native_address, offset, byteBuffer, dataSize);
    }

    /*[-TEAVM;-REPLACE]
        @org.teavm.jso.JSBody(params = { "this_addr", "offset", "bytes_addr", "dataSize" }, script = "" +
            "var jsObj = [MODULE].wrapPointer(this_addr, [MODULE].WGPUComputePassEncoder);" +
            "var ptr = runtime._malloc(dataSize);" +
            "runtime.HEAPU8.set(bytes_addr.subarray(0, dataSize), ptr);" +
            "jsObj.setImmediates(offset, ptr, dataSize);" +
            "runtime._free(ptr);"
        )
        private static native void internal_native_SetImmediates(int this_addr, int offset, org.teavm.jso.JSObject bytes_addr, int dataSize);
    */
    /*[-JNI;-NATIVE]
        JGPU::WGPUComputePassEncoder* nativeObject = (JGPU::WGPUComputePassEncoder*)this_addr;
        void* bufferAddress = env->GetDirectBufferAddress(byteBuffer);
        nativeObject->SetImmediates((int)offset, bufferAddress, (int)dataSize);
    */
    /*[-FFM;-REPLACE]
        private static void internal_native_SetImmediates(long this_addr, int offset, long data_ptr, int dataSize) {
            try {
                FFMImmediateHandles.setImmediates.invokeExact(this_addr, offset, data_ptr, dataSize);
            }
            catch(Throwable e) {
                throw new RuntimeException(e);
            }
        }
    */
    private static native void internal_native_SetImmediates(long this_addr, int offset, ByteBuffer byteBuffer, int dataSize);
}
