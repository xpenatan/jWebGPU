#pragma once

#include "IDLHelper.h"
#include "webgpu/webgpu.h"
#include <iostream>

// ENUM

//using WGSLLanguageFeatureName = wgpu::WGSLLanguageFeatureName;

class JAdapter;
class JDevice;
class JDeviceDescriptor;
class JSurface;
class JQueue;
class JStringView;

enum JPlatformType : int {
    WGPU_Windows = 0,
    WGPU_Mac,
    WGPU_Linux,
    WGPU_iOS,
    WGPU_Android,
    WGPU_Web,
    WGPU_Unknown
};

class RequestAdapterCallback {
public:
    virtual void OnCallback(WGPURequestAdapterStatus status, JAdapter* adapter) = 0;

};

class RequestDeviceCallback {
public:
    virtual void OnCallback(WGPURequestDeviceStatus status, JDevice* device) = 0;

};

class JQueue {
    private:

    public:
        WGPUQueue queue;
};

class JChainedStruct {
    private:

    public:
        WGPUChainedStruct chainedStruct;
};

class JStringView : public JChainedStruct{
    private:

    public:
        WGPUStringView stringView;
};

class JDeviceDescriptor {
    private:

    public:
        WGPUDeviceDescriptor descriptor;
};

class JRequestAdapterOptions {
    private:

    public:
        WGPURequestAdapterOptions options;
};

class JAdapterInfo {
    private:

    public:
        WGPUAdapterInfo adapterInfo;

        std::string GetVendor() {
            WGPUStringView stringView = adapterInfo.vendor;
            return stringView.data == NULL ? "" : stringView.data;
        }

        std::string GetArchitecture() {
            WGPUStringView stringView = adapterInfo.architecture;
            return stringView.data == NULL ? "" : stringView.data;
        }

        std::string GetDevice() {
            WGPUStringView stringView = adapterInfo.device;
            return stringView.data == NULL ? "" : stringView.data;
        }

        std::string GetDescription() {
            WGPUStringView stringView = adapterInfo.description;
            return stringView.data == NULL ? "" : stringView.data;
        }

        WGPUBackendType GetBackendType() {
            return adapterInfo.backendType;
        }

        WGPUAdapterType GetAdapterType() {
            return adapterInfo.adapterType;
        }
};

class JSurface {
    private:

    public:
        WGPUSurface surface;
};

class JSurfaceConfiguration {
    private:

    public:
        WGPUSurfaceConfiguration surfaceConfiguration;
};

class JDevice {

    private:

    public:
        WGPUDevice device;

        JDevice() {
        }

//        bool GetAdapterInfo(JAdapterInfo* adapterInfo) {
//            #ifdef __EMSCRIPTEN__
//                WGPUStatus status = wgpuDeviceGetAdapterInfo(device, &(adapterInfo->adapterInfo));
//            #else
//                wgpuDeviceGetAdapterInfo(device); // Not implemented
//                WGPUStatus status = WGPUStatus_Error;
//            #endif
//            return status == WGPUStatus_Success;
//        }

        JQueue* GetQueue() {
            JQueue* queue = new JQueue();
            queue->queue = wgpuDeviceGetQueue(device);
            return queue;
        }
};

class JAdapter {
    private:

    public:
        WGPUAdapter adapter;

        JAdapter() {
        }

        void RequestDevice(JDeviceDescriptor* descriptor, WGPUCallbackMode mode, RequestDeviceCallback* callback) {
            WGPURequestDeviceCallbackInfo callbackInfo = {};
            callbackInfo.mode = mode;
            callbackInfo.callback = [](WGPURequestDeviceStatus status, WGPUDevice dev, WGPUStringView message, void* callback_param, void* userdata_param) {
                RequestDeviceCallback* cback = reinterpret_cast<RequestDeviceCallback*>(callback_param);
                JDevice* device = new JDevice();
                device->device = dev;
                cback->OnCallback(status, device);
            };
            callbackInfo.userdata1 = reinterpret_cast<void*>(callback);
            callbackInfo.userdata2 = NULL;
            auto result = wgpuAdapterRequestDevice(adapter, &(descriptor->descriptor), callbackInfo);
        }

        bool GetInfo(JAdapterInfo* adapterInfo) {
            WGPUStatus status = wgpuAdapterGetInfo(adapter, &(adapterInfo->adapterInfo));
            return status == WGPUStatus_Success;
        }
};

class JInstance {
    private:

    public:
        WGPUInstance instance;

        JInstance() {
            instance = wgpuCreateInstance(NULL);
        }

        void RequestAdapter(JRequestAdapterOptions* options, WGPUCallbackMode mode, RequestAdapterCallback* callback) {
            WGPURequestAdapterCallbackInfo callbackInfo = {};
            callbackInfo.mode = mode;
            callbackInfo.callback = [](WGPURequestAdapterStatus status, WGPUAdapter ad, WGPUStringView message, void* callback_param, void*) {
                RequestAdapterCallback* cback = reinterpret_cast<RequestAdapterCallback*>(callback_param);
                JAdapter* adapter = new JAdapter();
                adapter->adapter = ad;
                cback->OnCallback(status, adapter);
            };
            callbackInfo.userdata1 = reinterpret_cast<void*>(callback);
            callbackInfo.userdata2 = NULL;
            auto result = wgpuInstanceRequestAdapter(instance, &(options->options), callbackInfo);
        }

        JSurface* CreateWebSurface(const char* canvas) {
            #ifdef __EMSCRIPTEN__
                WGPUEmscriptenSurfaceSourceCanvasHTMLSelector canvasDesc {};
                WGPUStringView stringView = {
                    .data = canvas,
                    .length = strlen(canvas)
                };
                canvasDesc.chain.sType = WGPUSType_EmscriptenSurfaceSourceCanvasHTMLSelector;
                canvasDesc.selector = stringView;
                WGPUSurfaceDescriptor surfDesc{};
                surfDesc.nextInChain = (WGPUChainedStruct*)&canvasDesc;
                JSurface* surface = new JSurface();
                surface->surface = wgpuInstanceCreateSurface(instance, &surfDesc);
            #else
                JSurface* surface = NULL;
            #endif
            return surface;
        }

        JSurface* CreateWindowsSurface(void * hwnd) {
            return NULL;
        }

        void ProcessEvents() {
            wgpuInstanceProcessEvents(instance);
        }
};

class JWebGPU {
    private:

    public:
        static JPlatformType GetPlatformType() {

            #ifdef _WIN32
                #ifdef _WIN64
                    return JPlatformType::WGPU_Windows;
                #else
                    return JPlatformType::WGPU_Windows;
                #endif
            #elif defined(__linux__)
                return JPlatformType::WGPU_Linux;
            #elif defined(__APPLE__)
                #if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
                    return JPlatformType::WGPU_iOS;
                #elif defined(TARGET_OS_MAC) && TARGET_OS_MAC
                    return JPlatformType::WGPU_Mac;
                #else
                    return JPlatformType::WGPU_Unknown;
                #endif
            #elif defined(__ANDROID__)
                 return JPlatformType::WGPU_Android;
            #elif defined(__EMSCRIPTEN__)
                return JPlatformType::WGPU_Web;
            #else
                return JPlatformType::WGPU_Unknown;
            #endif
        }

        static void Set() {
//            wgpu::Instance instance = wgpu::CreateInstance();
//            wgpu::RequestAdapterOptions options = {};
//            instance.RequestAdapter(&options, wgpu::CallbackMode::WaitAnyOnly,
//                    [&](wgpu::RequestAdapterStatus status, wgpu::Adapter ad, wgpu::StringView message) {
//                         std::cout << "CallBack Set: " << std::endl;
//                    });
        }

//        static JInstance* CreateInstance() {
//            JInstance* instance = new JInstance();
//            auto result = wgpu::CreateInstance();
//            instance->instance_ = result;
//            return instance;
//        }
};

