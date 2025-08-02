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

    public static boolean IS_DAWN_WINDOWS = false;

    public static void init(JParserLibraryLoaderListener listener) {
        String osName = System.getProperty("os.name").toLowerCase();
        if(osName.contains("win") && IS_DAWN_WINDOWS) {
            JParserLibraryLoaderOptions options = new JParserLibraryLoaderOptions();
            options.autoAddSuffix = false;
            JParserLibraryLoader.load("webgpu_dawn", options, (isSuccess, e) -> {
                if(isSuccess) {
                    JParserLibraryLoader.load("jWebGPU", listener);
                }
                else {
                    listener.onLoad(false, e);
                }
            });
        }
        else {
            JParserLibraryLoader.load("jWebGPU", listener);
        }
    }
}