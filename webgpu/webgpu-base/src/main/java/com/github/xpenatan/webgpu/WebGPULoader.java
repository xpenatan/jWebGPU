package com.github.xpenatan.webgpu;

import com.github.xpenatan.jparser.loader.JParserLibraryLoader;
import com.github.xpenatan.jparser.loader.JParserLibraryLoaderListener;

/**
 * @author xpenatan
 */
public class WebGPULoader {

    /*[-JNI;-NATIVE]
        #include "WebGPUCustom.h"
    */

    public static void init(JParserLibraryLoaderListener listener) {
        JParserLibraryLoader.load("webgpu", listener);
    }
}