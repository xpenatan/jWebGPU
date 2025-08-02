package com.github.xpenatan.webgpu;

import com.github.xpenatan.jparser.loader.JParserLibraryLoader;
import com.github.xpenatan.jparser.loader.JParserLibraryLoaderListener;
import com.github.xpenatan.jparser.loader.JParserLibraryLoaderOptions;

/**
 * @author xpenatan
 */
public class JWebGPULoader {

    /*[-JNI;-NATIVE]
        #include "jWebGPU.h"
    */

    private static JWebGPUBackend backend;

    public static void init(JParserLibraryLoaderListener listener) {
        init(JWebGPUBackend.WGPU, listener);
    }

    /*[-TEAVM;-REPLACE]
      public static void init(JWebGPUBackend backend, JParserLibraryLoaderListener listener) {
          JWebGPULoader.backend = JWebGPUBackend.DAWN;
          JParserLibraryLoader.load("jWebGPU", listener);
      }
    */
    public static void init(JWebGPUBackend backend, JParserLibraryLoaderListener listener) {
        JWebGPULoader.backend = backend;
        if(backend == JWebGPUBackend.DAWN) {
            // Load dawn first and then the bindings.
            JParserLibraryLoaderOptions options = new JParserLibraryLoaderOptions();
            options.autoAddSuffix = false;
            JParserLibraryLoader.load("native/dawn/webgpu_dawn", options, (isSuccess, e) -> {
                if(isSuccess) {
                    JParserLibraryLoader.load("native/dawn/jWebGPU", listener);
                }
                else {
                    listener.onLoad(false, e);
                }
            });
        }
        else {
            // WGPU do static link so it's a single library
            JParserLibraryLoader.load("native/wgpu/jWebGPU", listener);
        }
    }

    public static JWebGPUBackend getBackend() {
        return backend;
    }
}