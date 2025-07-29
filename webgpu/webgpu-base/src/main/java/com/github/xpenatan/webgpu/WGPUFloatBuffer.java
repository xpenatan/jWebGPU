package com.github.xpenatan.webgpu;

import idl.IDLBase;

public class WGPUFloatBuffer extends IDLBase {
    public void put(float [] values, int offset, int size) {
        internal_put(native_address, values, offset, size);
    }

    /*[-TEAVM;-NATIVE]
        var jsObj = [MODULE].wrapPointer(this_addr, [MODULE].WGPUFloatBuffer);
        jsObj.put_2(values, offset, size);
    */
    /*[-JNI;-NATIVE]
        WGPUFloatBuffer* nativeObject = (WGPUFloatBuffer*)this_addr;
        jfloat* criticalArray = (jfloat*)env->GetPrimitiveArrayCritical(values, nullptr);
        nativeObject->put(criticalArray, offset, size);
        env->ReleasePrimitiveArrayCritical(values, criticalArray, 0);
    */
    private static native void internal_put(long this_addr, float [] values, int offset, int size);
}
