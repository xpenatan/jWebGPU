package com.github.xpenatan.webgpu;

public class JWebGPU {

    /*[-JNI;-NATIVE]
        #ifdef __ANDROID__
            #include <android/native_window_jni.h>
        #endif
    */

    public long getAndroidSurface(Object surface) {
        return internal_getAndroidSurface(surface);
    }

    /*[-TEAVM;-NATIVE]
        return 0;
    */
    /*[-JNI;-NATIVE]
        #ifdef __ANDROID__
            return 1;
        #else
            return 0;
        #endif
    */
    private static native long internal_getAndroidSurface(Object surface);

}