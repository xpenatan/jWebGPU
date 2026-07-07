package com.github.xpenatan.webgpu;

import com.github.xpenatan.jParser.loader.JParserLibraryLoader;
import com.github.xpenatan.jParser.loader.JParserLibraryLoaderListener;
import com.github.xpenatan.jParser.loader.JParserLibraryLoaderOptions;
import com.github.xpenatan.jparser.runtime.RuntimeLoader;

/**
 * @author xpenatan
 */
public class JWebGPULoader {

    /*[-JNI;-NATIVE]
        #include "jWebGPU.h"
    */

    /*[-FFM;-NATIVE]
        #include "jWebGPU.h"
    */

    private static JWebGPUBackend backend;

    public static void init(JParserLibraryLoaderListener listener) {
        init(JWebGPUBackend.WGPU, listener);
    }

    public static void init(JWebGPUBackend backend, JParserLibraryLoaderListener listener) {
        RuntimeLoader.init(new JParserLibraryLoaderListener() {
            @Override
            public void onLoad(boolean idl_isSuccess, Throwable idl_t) {
                if(idl_isSuccess) {
                    initInternal(backend, listener);
                }
                else {
                    listener.onLoad(false, idl_t);
                }
            }
        });
    }

    /*[-TEAVM;-ADD_RAW]
      @org.teavm.jso.JSBody(script =
              "if(typeof jWebGPU === 'undefined') return;" +
              "function installCallbackDispatch(obj, ctor) {" +
              "  obj.OnCallback = function() {" +
              "    if(typeof this.onCallback === 'function') return this.onCallback.apply(this, arguments);" +
              "  };" +
              "  obj.__destroy__ = function() {" +
              "    if(this.__jwebgpuDestroying) return;" +
              "    this.__jwebgpuDestroying = true;" +
              "    try {" +
              "      var destroy = ctor.prototype.__destroy__;" +
              "      if(typeof destroy === 'function') return destroy.apply(this, arguments);" +
              "    } finally {" +
              "      this.__jwebgpuDestroying = false;" +
              "    }" +
              "  };" +
              "}" +
              "function wrapCallbackConstructor(key, ctor) {" +
              "  if(ctor.__jwebgpuWrapped) return;" +
              "  var Wrapped = function() {" +
              "    var obj = new ctor();" +
              "    installCallbackDispatch(obj, ctor);" +
              "    return obj;" +
              "  };" +
              "  Wrapped.prototype = ctor.prototype;" +
              "  var staticNames = Object.getOwnPropertyNames(ctor);" +
              "  for(var i = 0; i < staticNames.length; i++) {" +
              "    var staticName = staticNames[i];" +
              "    if(staticName === 'prototype' || staticName === 'name' || staticName === 'length') continue;" +
              "    Object.defineProperty(Wrapped, staticName, Object.getOwnPropertyDescriptor(ctor, staticName));" +
              "  }" +
              "  Wrapped.__jwebgpuWrapped = true;" +
              "  jWebGPU[key] = Wrapped;" +
              "}" +
              "function lowerAlias(name) {" +
              "  return name.charAt(0).toLowerCase() + name.substring(1);" +
              "}" +
              "function makeOverloadDispatcher(list) {" +
              "  return function() {" +
              "    for(var i = 0; i < list.length; i++) {" +
              "      if(list[i].fn.length === arguments.length) return list[i].fn.apply(this, arguments);" +
              "    }" +
              "    return list[0].fn.apply(this, arguments);" +
              "  };" +
              "}" +
              "for(var key in jWebGPU) {" +
              "  var ctor = jWebGPU[key];" +
              "  if(typeof ctor !== 'function' || !ctor.prototype) continue;" +
              "  var proto = ctor.prototype;" +
              "  var isCallbackImpl = key.indexOf('CallbackImpl') >= 0;" +
              "  var names = Object.getOwnPropertyNames(proto);" +
              "  var overloads = {};" +
              "  for(var i = 0; i < names.length; i++) {" +
              "    var name = names[i];" +
              "    if(!name || name === 'constructor') continue;" +
              "    if(isCallbackImpl && name === 'OnCallback') continue;" +
              "    var alias = lowerAlias(name);" +
              "    if(alias !== name && proto[alias] === undefined && typeof proto[name] === 'function') {" +
              "      proto[alias] = proto[name];" +
              "    }" +
              "    var overloadMatch = /^(.+)__(\\d+)$/.exec(name);" +
              "    if(overloadMatch && typeof proto[name] === 'function') {" +
              "      var overloadAlias = lowerAlias(overloadMatch[1]);" +
              "      if(overloads[overloadAlias] === undefined) overloads[overloadAlias] = [];" +
              "      overloads[overloadAlias].push({ name: name, fn: proto[name] });" +
              "    }" +
              "  }" +
              "  for(var overloadName in overloads) {" +
              "    if(proto[overloadName] === undefined) {" +
              "      proto[overloadName] = makeOverloadDispatcher(overloads[overloadName]);" +
              "    }" +
              "  }" +
              "  if(isCallbackImpl) {" +
              "    wrapCallbackConstructor(key, ctor);" +
              "  }" +
              "}"
      )
      private static native void installWebMethodAliases();
    */

    /*[-TEAVM;-REPLACE]
      private static void initInternal(JWebGPUBackend backend, JParserLibraryLoaderListener listener) {
          JWebGPULoader.backend = JWebGPUBackend.DAWN;
          JParserLibraryLoader.load("jWebGPU", new JParserLibraryLoaderListener() {
              @Override
              public void onLoad(boolean isSuccess, Throwable t) {
                  if(isSuccess) {
                      installWebMethodAliases();
                  }
                  listener.onLoad(isSuccess, t);
              }
          });
      }
    */
    private static void initInternal(JWebGPUBackend backend, JParserLibraryLoaderListener listener) {
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

        JParserLibraryLoaderOptions options = new JParserLibraryLoaderOptions();
        options.path = path;

        if(backend == JWebGPUBackend.DAWN) {
            // Load dawn first and then the bindings.
            JParserLibraryLoaderOptions dawnOptions = new JParserLibraryLoaderOptions();
            dawnOptions.path = path;
            dawnOptions.autoAddSuffix = false;
            dawnOptions.autoAddPrefix = false;

            JParserLibraryLoader.load("webgpu_dawn", dawnOptions, (isSuccess, e) -> {
                if(isSuccess) {
                    JParserLibraryLoader.load("jWebGPU", options, listener);
                }
                else {
                    listener.onLoad(false, e);
                }
            });
        }
        else {
            // WGPU do static link so it's a single library
            JParserLibraryLoader.load("jWebGPU", options, listener);
        }
    }

    public static JWebGPUBackend getBackend() {
        return backend;
    }
}
