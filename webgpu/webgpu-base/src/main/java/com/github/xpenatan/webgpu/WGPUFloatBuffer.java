package com.github.xpenatan.webgpu;

import com.github.xpenatan.jParser.idl.IDLBase;

public class WGPUFloatBuffer extends IDLBase {

    public final static WGPUFloatBuffer NULL;

    static {
        NULL = native_new();
    }

    public static WGPUFloatBuffer native_new() {
        return new WGPUFloatBuffer((byte) 0, (char) 0);
    }

    private WGPUFloatBuffer(byte b, char c) {
    }

    /*[-FFM;-NATIVE]
        extern "C" {
        FFM_EXPORT void jparser_com_github_xpenatan_webgpu_WGPUFloatBuffer_internal_1putFFM__JJII(int64_t this_addr, float* values_ptr, int32_t offset, int32_t size) {
            WGPUFloatBuffer* nativeObject = (WGPUFloatBuffer*)this_addr;
            nativeObject->put(values_ptr, offset, size);
        }
        }
    */

    /*[-FFM;-ADD]
        private static final class FFMBufferHandles {
            private static final java.lang.foreign.SymbolLookup LOOKUP = java.lang.foreign.SymbolLookup.loaderLookup();
            private static final java.lang.foreign.Linker LINKER = java.lang.foreign.Linker.nativeLinker();

            static final java.lang.invoke.MethodHandle put = LINKER.downcallHandle(
                    LOOKUP.find("jparser_com_github_xpenatan_webgpu_WGPUFloatBuffer_internal_1putFFM__JJII").orElseThrow(),
                    java.lang.foreign.FunctionDescriptor.ofVoid(
                            java.lang.foreign.ValueLayout.JAVA_LONG,
                            java.lang.foreign.ValueLayout.ADDRESS,
                            java.lang.foreign.ValueLayout.JAVA_INT,
                            java.lang.foreign.ValueLayout.JAVA_INT
                    )
            );
        }
    */

    /*[-FFM;-REPLACE_BLOCK]
        {
            java.lang.foreign.MemorySegment seg = java.lang.foreign.MemorySegment.ofArray(values);
            internal_put(native_address, seg, offset, size);
        }
    */
    public void put(float [] values, int offset, int size) {
        internal_put(native_address, values, offset, size);
    }

    /*[-TEAVM;-NATIVE]
        var jsObj = [MODULE].wrapPointer(this_addr, [MODULE].WGPUFloatBuffer);
        jsObj.put__2(values, offset, size);
    */
    /*[-JNI;-NATIVE]
        WGPUFloatBuffer* nativeObject = (WGPUFloatBuffer*)this_addr;
        jfloat* criticalArray = (jfloat*)env->GetPrimitiveArrayCritical(values, nullptr);
        nativeObject->put(criticalArray, offset, size);
        env->ReleasePrimitiveArrayCritical(values, criticalArray, 0);
    */
    /*[-FFM;-REPLACE]
        private static void internal_put(long this_addr, java.lang.foreign.MemorySegment values_ptr, int offset, int size) {
            try {
                FFMBufferHandles.put.invokeExact(this_addr, values_ptr, offset, size);
            }
            catch(Throwable e) {
                throw new RuntimeException(e);
            }
        }
    */
    private static native void internal_put(long this_addr, float [] values, int offset, int size);
}
