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

    public static ApiType apiType = ApiType.WGPU;

    /*[-TEAVM;-REPLACE]
        public static void init(JParserLibraryLoaderListener listener) {
            apiType = ApiType.DAWN;
            JParserLibraryLoader.load("jWebGPU", listener);
        }
    */
    public static void init(JParserLibraryLoaderListener listener) {
        if(apiType == ApiType.DAWN) {
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

    public enum ApiType {
        WGPU,
        DAWN
    }
}