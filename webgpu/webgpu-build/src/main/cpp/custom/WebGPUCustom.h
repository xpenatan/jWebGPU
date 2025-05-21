#pragma once

#include "IDLHelper.h"
#include "webgpu/webgpu.h"
#include <iostream>

#if _WIN32
    #include <windows.h>
    #include <glfw3.h>
    #include <glfw3native.h>
#endif

// ENUM

//using WGSLLanguageFeatureName = wgpu::WGSLLanguageFeatureName;

class JAdapter;
class JDevice;
class JDeviceDescriptor;
class JSurface;
class JQueue;
class JStringView;
class JSurfaceConfiguration;
class JBlendState;
class JColorTargetState;
class JDepthStencilState;

using IDLArrayJColorTargetState = IDLArray<JColorTargetState>;
using IDLArrayJDepthStencilState = IDLArray<JDepthStencilState>;

enum JPlatformType : int {
    WGPU_Unknown = 0,
    WGPU_Windows,
    WGPU_Mac,
    WGPU_Linux,
    WGPU_iOS,
    WGPU_Android,
    WGPU_Web
};

class RequestAdapterCallback {
public:
    virtual void OnCallback(WGPURequestAdapterStatus status, JAdapter* adapter) = 0;

};

class RequestDeviceCallback {
public:
    virtual void OnCallback(WGPURequestDeviceStatus status, JDevice* device) = 0;

};

class UncapturedErrorCallback {
public:
    virtual void OnCallback(WGPUErrorType errorType, const char* message) = 0;

};

class JQueue {
    private:

    public:
        WGPUQueue queue;

        ~JQueue() {
            wgpuQueueRelease(queue);
        }
};

class JChainedStruct {
    private:

    public:
        WGPUChainedStruct chainedStruct;
        WGPUChainedStruct* chainedStructPtr = NULL;

        JChainedStruct() {
            chainedStructPtr = &chainedStruct;
        }

        void SetNext(JChainedStruct* value) {
            chainedStructPtr->next = value != NULL ? &(value->chainedStruct) : NULL;
        }

        void SetSType(WGPUSType type) {
            chainedStructPtr->sType = type;
        }
};

class JStringView : public JChainedStruct {
    private:

    public:

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

class JShaderModule {
    private:

    public:
        WGPUShaderModule shaderModule;

        ~JShaderModule() {
            wgpuShaderModuleRelease(shaderModule);
        }
};

struct JConstantEntry {
    WGPUChainedStruct const * nextInChain;
    WGPUStringView key;
    double value;

    inline operator const WGPUConstantEntry&() const noexcept {
        return *reinterpret_cast<const WGPUConstantEntry*>(this);
    }
};

struct JVertexState {
    WGPUChainedStruct const * nextInChain;
    WGPUShaderModule module;
    WGPUStringView entryPoint;
    size_t constantCount;
    JConstantEntry const * constants;
    size_t bufferCount;
    WGPUVertexBufferLayout const * buffers;

    inline operator const WGPUVertexState&() const noexcept {
        return *reinterpret_cast<const WGPUVertexState*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct != NULL ? (chainedStruct->chainedStructPtr) : NULL;
    }

    void SetModule(JShaderModule* shaderModule) {
        module = shaderModule != NULL ? shaderModule->shaderModule : NULL;
    }

    void SetEntryPoint(const char* value) {
        WGPUStringView stringView = {};
        stringView.data = strdup(value);
        stringView.length = strlen(value);
        entryPoint = stringView;
    }

    void SetConstantCount(int value) {
        constantCount = value;
    }

    void SetConstants(JConstantEntry* value) {
        constants = value;
    }

    void SetBufferCount(int value) {
        bufferCount = value;
    }

//        void SetBuffers(WGPUVertexBufferLayout* value) {
//            vertexState.buffers = value;
//        }
};

class JShaderSourceWGSL : public JChainedStruct {

    private:

    public:
        WGPUShaderSourceWGSL shaderSourceWGSL;
        JChainedStruct chainedCopy;

        JShaderSourceWGSL() {
            shaderSourceWGSL = WGPUShaderSourceWGSL{};
            chainedStructPtr = reinterpret_cast<WGPUChainedStruct*>(&shaderSourceWGSL);
        }

        JChainedStruct& GetChain() {
            // Obtain chain pointer from source to the copied struct.
            // This way I can return the Java object and it will update the original chain data
            chainedCopy.chainedStructPtr = &(shaderSourceWGSL.chain);
            return chainedCopy;
        }

        void SetCode(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            shaderSourceWGSL.code = stringView;
        }
};

class JShaderModuleDescriptor {

    private:

    public:
        WGPUShaderModuleDescriptor shaderModuleDescriptor;

        JShaderModuleDescriptor() {
            shaderModuleDescriptor = WGPUShaderModuleDescriptor{};
        }

        void SetNextInChain(JChainedStruct* chainedStruct) {
            shaderModuleDescriptor.nextInChain = chainedStruct != NULL ? (chainedStruct->chainedStructPtr) : NULL;
        }

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            shaderModuleDescriptor.label = stringView;
        }
};

struct JBlendComponent {
    WGPUBlendOperation operation;
    WGPUBlendFactor srcFactor;
    WGPUBlendFactor dstFactor;

    inline operator const WGPUBlendComponent&() const noexcept {
        return *reinterpret_cast<const WGPUBlendComponent*>(this);
    }

    void SetOperation(WGPUBlendOperation operation) {
        this->operation = operation;
    }

    WGPUBlendOperation GetOperation() {
        return operation;
    }

    void SetSrcFactor(WGPUBlendFactor factor) {
        srcFactor = factor;
    }

    WGPUBlendFactor GetSrcFactor() {
        return srcFactor;
    }

    void SetDstFactor(WGPUBlendFactor factor) {
        dstFactor = factor;
    }

    WGPUBlendFactor GetDstFactor() {
        return dstFactor;
    }
};

struct JBlendState {

    JBlendComponent color;
    JBlendComponent alpha;

    inline operator const WGPUBlendState&() const noexcept {
        return *reinterpret_cast<const WGPUBlendState*>(this);
    }

    void SetColor(JBlendComponent* color) {
        this->color = *color;
    }

    void SetAlpha(JBlendComponent* alpha) {
        this->alpha = *alpha;
    }

    JBlendComponent& GetColor() {
        return color;
    }

    JBlendComponent& GetAlpha() {
        return alpha;
    }
};

struct JColorTargetState {
    WGPUChainedStruct const * nextInChain = nullptr;
    WGPUTextureFormat format = WGPUTextureFormat_Undefined;
    JBlendState const * blend = nullptr;
    WGPUColorWriteMask writeMask = WGPUColorWriteMask_All;

    inline operator const WGPUColorTargetState&() const noexcept {
        return *reinterpret_cast<const WGPUColorTargetState*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct != NULL ? (chainedStruct->chainedStructPtr) : NULL;
    }

    void SetFormat(WGPUTextureFormat format) {
        this->format = format;
    }

    void SetBlend(JBlendState* blendState) {
        this->blend = blendState;
    }

    void SetWriteMask(WGPUColorWriteMask writeMask) {
        this->writeMask = writeMask;
    }
};

struct JFragmentState {
    WGPUChainedStruct const * nextInChain;
    WGPUShaderModule module;
    WGPUStringView entryPoint;
    size_t constantCount;
    JConstantEntry const * constants;
    size_t targetCount;
    JColorTargetState const * targets = nullptr;

    inline operator const WGPUFragmentState&() const noexcept {
        return *reinterpret_cast<const WGPUFragmentState*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct != NULL ? (chainedStruct->chainedStructPtr) : NULL;
    }

    void SetEntryPoint(const char* value) {
        WGPUStringView stringView = {};
        stringView.data = strdup(value);
        stringView.length = strlen(value);
        entryPoint = stringView;
    }

    void SetConstantCount(int value) {
        constantCount = value;
    }

    void SetTargetCount(int value) {
        targetCount = value;
    }

    void SetTargets(JColorTargetState* targets) {
        this->targets = targets;
    }

    void SetModule(JShaderModule* shaderModule) {
        this->module = shaderModule != NULL ? shaderModule->shaderModule : NULL;
    }

    void SetConstants(JConstantEntry* constant) {
        this->constants = constant;
    }
};

struct JPrimitiveState {

    WGPUChainedStruct const * nextInChain;
    WGPUPrimitiveTopology topology;
    WGPUIndexFormat stripIndexFormat;
    WGPUFrontFace frontFace;
    WGPUCullMode cullMode;
    WGPUBool unclippedDepth;

    inline operator const WGPUPrimitiveState&() const noexcept {
        return *reinterpret_cast<const WGPUPrimitiveState*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct != NULL ? (chainedStruct->chainedStructPtr) : NULL;
    }

    void SetTopology(WGPUPrimitiveTopology value) {
        topology = value;
    }

    void SetStripIndexFormat(WGPUIndexFormat value) {
        stripIndexFormat = value;
    }

    void SetFrontFace(WGPUFrontFace value) {
        frontFace = value;
    }

    void SetCullMode(WGPUCullMode value) {
        cullMode = value;
    }
};

struct JDepthStencilState {

    WGPUChainedStruct const * nextInChain;
    WGPUTextureFormat format;
    WGPUOptionalBool depthWriteEnabled;
    WGPUCompareFunction depthCompare;
    WGPUStencilFaceState stencilFront;
    WGPUStencilFaceState stencilBack;
    uint32_t stencilReadMask;
    uint32_t stencilWriteMask;
    int32_t depthBias;
    float depthBiasSlopeScale;
    float depthBiasClamp;

    inline operator const WGPUDepthStencilState&() const noexcept {
        return *reinterpret_cast<const WGPUDepthStencilState*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct != NULL ? (chainedStruct->chainedStructPtr) : NULL;
    }

    void SetFormat(WGPUTextureFormat format) {
        this->format = format;
    }

    void SetDepthWriteEnabled(WGPUOptionalBool depthWriteEnabled) {
        this->depthWriteEnabled = depthWriteEnabled;
    }

    void SetDepthCompare(WGPUCompareFunction depthCompare) {
        this->depthCompare = depthCompare;
    }

    void SetDepthBiasSlopeScale(float depthBiasSlopeScale) {
        this->depthBiasSlopeScale = depthBiasSlopeScale;
    }

    void SetDepthBiasClamp(float depthBiasClamp) {
        this->depthBiasClamp = depthBiasClamp;
    }
};

struct JMultisampleState {

    WGPUChainedStruct const * nextInChain;
    uint32_t count;
    uint32_t mask;
    WGPUBool alphaToCoverageEnabled;

    inline operator const WGPUMultisampleState&() const noexcept {
        return *reinterpret_cast<const WGPUMultisampleState*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct != NULL ? (chainedStruct->chainedStructPtr) : NULL;
    }

    void SetCount(int count) {
        this->count = count;
    }

    void SetMask(int mask) {
        this->mask = mask;
    }

    void SetAlphaToCoverageEnabled(int alphaToCoverageEnabled) {
        this->alphaToCoverageEnabled = alphaToCoverageEnabled;
    }
};

class JPipelineLayout {

    private:

    public:
        WGPUPipelineLayout pipelineLayout;

        JPipelineLayout() {
        }
};

struct JRenderPipelineDescriptor {

    WGPUChainedStruct const * nextInChain;
    WGPUStringView label;
    WGPU_NULLABLE WGPUPipelineLayout layout;
    JVertexState vertex;
    JPrimitiveState primitive;
    JDepthStencilState const * depthStencil;
    JMultisampleState multisample;
    JFragmentState const * fragment;

    inline operator const WGPURenderPipelineDescriptor&() const noexcept {
        return *reinterpret_cast<const WGPURenderPipelineDescriptor*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct != NULL ? (chainedStruct->chainedStructPtr) : NULL;
    }

    void SetLabel(const char* value) {
        WGPUStringView stringView = {};
        stringView.data = strdup(value);
        stringView.length = strlen(value);
        label = stringView;
    }

    JVertexState& GetVertex() {
        return vertex;
    }

    JPrimitiveState& GetPrimitive() {
        return primitive;
    }

    void SetFragment(JFragmentState* fragment) {
        this->fragment = fragment;
    }

    void SetDepthStencil(JDepthStencilState* depthStencilState) {
        depthStencil = depthStencilState;
    }

    JMultisampleState& GetMultisample() {
        return multisample;
    }

    void SetLayout(JPipelineLayout* pipelineLayout) {
        layout = pipelineLayout != NULL ? pipelineLayout->pipelineLayout : NULL;
    }

};

class JRenderPipeline {

    private:

    public:
        WGPURenderPipeline renderPipeline;

        ~JRenderPipeline() {
            wgpuRenderPipelineRelease(renderPipeline);
        }
};

class JDevice {

    private:

    public:
        WGPUDevice device;

        JDevice() {
        }

        ~JDevice() {
            wgpuDeviceRelease(device);
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

        JRenderPipeline* CreateRenderPipeline(JRenderPipelineDescriptor* pipelineDescriptor) {
            JRenderPipeline* renderPipeline = new JRenderPipeline();
            renderPipeline->renderPipeline = wgpuDeviceCreateRenderPipeline(device, reinterpret_cast<WGPURenderPipelineDescriptor const * >(pipelineDescriptor));
            return renderPipeline;
        }

        JShaderModule* CreateShaderModule(JShaderModuleDescriptor* shaderModuleDescriptor) {
            JShaderModule* shaderModule = new JShaderModule();
            shaderModule->shaderModule = wgpuDeviceCreateShaderModule(device, &(shaderModuleDescriptor->shaderModuleDescriptor));
            return shaderModule;
        }
};

class JSurfaceConfiguration {
    private:

    public:
        WGPUSurfaceConfiguration surfaceConfiguration;

        JSurfaceConfiguration() {
            surfaceConfiguration = WGPUSurfaceConfiguration{};
        }

        void SetNextInChain(JChainedStruct* chainedStruct) {
            surfaceConfiguration.nextInChain = chainedStruct != NULL ? (chainedStruct->chainedStructPtr) : NULL;
        }

        void SetWidth(long width) {
            surfaceConfiguration.width = width;
        }

        void SetHeight(long height) {
            surfaceConfiguration.height = height;
        }

        void SetFormat(WGPUTextureFormat format) {
            surfaceConfiguration.format = format;
        }

        void SetViewFormatCount(long value) {
            surfaceConfiguration.viewFormatCount = value;
        }

        void SetViewFormats(void* viewFormats) {
            surfaceConfiguration.viewFormats = (WGPUTextureFormat*)viewFormats;
        }

        void SetUsage(WGPUTextureUsage usage) {
            surfaceConfiguration.usage = usage;
        }

        void SetDevice(JDevice* device) {
            surfaceConfiguration.device = device != NULL ? device->device : NULL;
        }

        void SetPresentMode(WGPUPresentMode presentMode) {
            surfaceConfiguration.presentMode = presentMode;
        }

        void SetAlphaMode(WGPUCompositeAlphaMode alphaMode) {
            surfaceConfiguration.alphaMode = alphaMode;
        }
};

class JSurfaceCapabilities {
    private:

    public:
        WGPUSurfaceCapabilities surfaceCapabilities;

        JSurfaceCapabilities() {
            surfaceCapabilities = WGPUSurfaceCapabilities{};
        }

        WGPUTextureFormat GetFormats(int index) {
            return surfaceCapabilities.formats[index];
        }
};

class JAdapter {
    private:

    public:
        WGPUAdapter adapter;

        JAdapter() {
        }

        ~JAdapter() {
            wgpuAdapterRelease(adapter);
        }

        void RequestDevice(JDeviceDescriptor* desc, WGPUCallbackMode mode, RequestDeviceCallback* callback, UncapturedErrorCallback* errorCallback) {
            WGPUDeviceDescriptor* descriptor = &desc->descriptor;

            descriptor->uncapturedErrorCallbackInfo.callback = [](const WGPUDevice* device, WGPUErrorType type, WGPUStringView message, void* callback_param, void* userdata_param) {
                UncapturedErrorCallback* cback = reinterpret_cast<UncapturedErrorCallback*>(callback_param);
                cback->OnCallback(type, message.data);
           };
           descriptor->uncapturedErrorCallbackInfo.userdata1 = reinterpret_cast<void*>(errorCallback);
           descriptor->uncapturedErrorCallbackInfo.userdata2 = NULL;

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
            auto result = wgpuAdapterRequestDevice(adapter, descriptor, callbackInfo);
        }

        bool GetInfo(JAdapterInfo* adapterInfo) {
            WGPUStatus status = wgpuAdapterGetInfo(adapter, &(adapterInfo->adapterInfo));
            return status == WGPUStatus_Success;
        }
};

class JSurface {
    private:

    public:
        WGPUSurface surface;

        ~JSurface() {
            wgpuSurfaceUnconfigure(surface);
            wgpuSurfaceRelease(surface);
        }

        void Configure(JSurfaceConfiguration* config) {
            wgpuSurfaceConfigure(surface, &(config->surfaceConfiguration));
        }

        void GetCapabilities(JAdapter* adapter, JSurfaceCapabilities* surfaceCapabilities) {
            wgpuSurfaceGetCapabilities(surface, adapter->adapter, &(surfaceCapabilities->surfaceCapabilities));
        }
};

class JInstance {
    private:

    public:
        WGPUInstance instance;

        JInstance() {
            instance = wgpuCreateInstance(NULL);
        }

        ~JInstance() {
            wgpuInstanceRelease(instance);
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
                WGPUStringView stringView = {};
                stringView.data = strdup(canvas);
                stringView.length = strlen(canvas);
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
            JSurface* surface = NULL;
            #if _WIN32
                surface = new JSurface();
                HINSTANCE hinstance = GetModuleHandle(NULL);
                WGPUSurfaceSourceWindowsHWND fromWindowsHWND;
                fromWindowsHWND.chain.next = NULL;
                fromWindowsHWND.chain.sType = WGPUSType_SurfaceSourceWindowsHWND;
                fromWindowsHWND.hinstance = hinstance;
                fromWindowsHWND.hwnd = hwnd;
                WGPUSurfaceDescriptor surfaceDescriptor{};
                surfaceDescriptor.nextInChain = &fromWindowsHWND.chain;
                surface->surface = wgpuInstanceCreateSurface(instance, &surfaceDescriptor);
            #endif
            return surface;
        }

        void ProcessEvents() {
            wgpuInstanceProcessEvents(instance);
        }
};

class JWebGPU {
    private:

    public:
        static JPlatformType GetPlatformType() {
            #if _WIN32
                return JPlatformType::WGPU_Windows;
            #elif __linux__
                return JPlatformType::WGPU_Linux;
            #elif TARGET_OS_IPHONE
                return JPlatformType::WGPU_iOS;
            #elif TARGET_OS_MAC
                return JPlatformType::WGPU_Mac;
            #elif __ANDROID__
                 return JPlatformType::WGPU_Android;
            #elif __EMSCRIPTEN__
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

