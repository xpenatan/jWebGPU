package com.github.xpenatan.webgpu;

import idl.IDLBase;

public class WGPUAndroidWindow extends IDLBase {

    /*[-JNI;-NATIVE]
        #ifdef __ANDROID__
            #include <android/native_window_jni.h>
        #endif
    */

    public void createAndroidSurface(Object surface) {
        long surfacePtr = internal_getAndroidSurface(surface);
        internal_native_SetWindow(getNativeData().getCPointer(), surfacePtr);
    }

    /*[-TEAVM;-NATIVE]
        return 0;
    */
    /*[-JNI;-NATIVE]
        #ifdef __ANDROID__
            ANativeWindow* g_window = g_window = ANativeWindow_fromSurface(env, surface);
            return (jlong)g_window;
        #else
            return 0;
        #endif
    */
    private static native long internal_getAndroidSurface(Object surface);


    /*[-TEAVM;-NATIVE]
    */
    /*[-JNI;-NATIVE]
        WGPUAndroidWindow* nativeObject = (WGPUAndroidWindow*)this_addr;
        nativeObject->SetWindow((void*)surfaceAddr);
    */
    private static native void internal_native_SetWindow(long this_addr, long surfaceAddr);

}