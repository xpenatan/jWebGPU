package com.github.xpenatan.webgpu;

import com.github.xpenatan.jParser.loader.JParserLibraryLoader;
import com.github.xpenatan.jParser.loader.JParserLibraryLoaderListener;
import com.github.xpenatan.jParser.loader.JParserLibraryLoaderOptions;

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
        String vm = System.getProperty("java.runtime.name");
        boolean isAndroid = vm != null && vm.contains("Android Runtime");
        String osName = System.getProperty("os.name").toLowerCase();

        if(!osName.contains("win")) {
            // Force WGPU if not windows. There is no libs for linux, mac or android.
            backend = JWebGPUBackend.WGPU;
        }
        String subDir = null;
        if(backend == JWebGPUBackend.DAWN) {
            subDir = "native/dawn/";
        }
        else {
            subDir = "native/wgpu/";
        }
        if(isAndroid) {
            subDir = null; // path no supported
        }
        final String path = subDir;

        JWebGPULoader.backend = backend;
        if(backend == JWebGPUBackend.DAWN) {
            // Load dawn first and then the bindings.
            JParserLibraryLoaderOptions options = new JParserLibraryLoaderOptions();
            options.autoAddSuffix = false;
            options.autoAddPrefix = false;
            JParserLibraryLoader.loadSync("webgpu_dawn", path, options, (isSuccess, e) -> {
                if(isSuccess) {
                    JParserLibraryLoader.loadSync("jWebGPU", path, listener);
                }
                else {
                    listener.onLoad(false, e);
                }
            });
        }
        else {
            // WGPU do static link so it's a single library
            JParserLibraryLoader.loadSync("jWebGPU", path, listener);
        }
    }

    public static JWebGPUBackend getBackend() {
        return backend;
    }
}