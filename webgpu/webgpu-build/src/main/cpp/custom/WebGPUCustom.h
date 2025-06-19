#pragma once

#include "IDLHelper.h"
#include "webgpu/webgpu.h"
#include <iostream>
#include <cstring>

#if _WIN32
    #include <windows.h>
    #include <glfw3.h>
    #include <glfw3native.h>
#endif

#ifdef __ANDROID__

#include <android/native_window.h>
#include <android/log.h>
#include <sstream>
#include <streambuf>

#define LOG_TAG "MyApp"

// Custom streambuf to redirect std::cout to Logcat
class LogcatStreamBuf : public std::streambuf {
protected:
    std::string buffer;

    int overflow(int c) override {
        if (c == EOF) {
            return !EOF;
        }
        buffer += static_cast<char>(c);
        if (c == '\n') {
            sync();
        }
        return c;
    }

    int sync() override {
        if (!buffer.empty()) {
            if (buffer.back() == '\n') {
                buffer.pop_back();
            }
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", buffer.c_str());
            buffer.clear();
        }
        return 0;
    }
};

// Singleton to manage Logcat redirection
class LogcatRedirector {
public:
    static void initialize() {
        static LogcatRedirector instance; // Lazy initialization
        if (!instance.isInitialized) {
            instance.original_buffer = std::cout.rdbuf();
            std::cout.rdbuf(&instance.logcat_buffer);
            instance.isInitialized = true;
        }
    }

private:
    LogcatRedirector() : original_buffer(nullptr), isInitialized(false) {}
    ~LogcatRedirector() {
        if (isInitialized) {
            std::cout.rdbuf(original_buffer);
        }
    }

    LogcatStreamBuf logcat_buffer;
    std::streambuf* original_buffer;
    bool isInitialized;

    // Prevent copying
    LogcatRedirector(const LogcatRedirector&) = delete;
    LogcatRedirector& operator=(const LogcatRedirector&) = delete;
};

#endif // __ANDROID__

class WebGPUAdapter;
class WebGPUDevice;
class WebGPUDeviceDescriptor;
class WebGPUSurface;
class WebGPUQueue;
class WebGPUStringView;
class WebGPUSurfaceConfiguration;
class WebGPUBlendState;
class WebGPUColorTargetState;
class WebGPUDepthStencilState;
class WebGPURenderPassColorAttachment;
class WebGPUConstantEntry;
class WebGPUVertexBufferLayout;
class WebGPURenderBundle;

#ifdef __EMSCRIPTEN__

using WGPURenderPassTimestampWrites = WGPUPassTimestampWrites; // dawn version TODO remove when both are the same

#else

using WGPURenderPassTimestampWrites = WGPURenderPassTimestampWrites;  // wgpu-native version TODO remove when both are the same

#endif //__EMSCRIPTEN__

enum WGPUPlatformType : int {
    WGPU_Unknown = 0,
    WGPU_Windows,
    WGPU_Mac,
    WGPU_Linux,
    WGPU_iOS,
    WGPU_Android,
    WGPU_Web
};

class WebGPUVectorColorTargetState {
    private:
        std::vector<WebGPUColorTargetState> vector;
    public:
        static WebGPUVectorColorTargetState Obtain() {
            WebGPUVectorColorTargetState obj;
            return obj;
        }
        int size() { return vector.size(); }
        void push_back(const WebGPUColorTargetState& attachment) { vector.push_back(attachment); }
        const WebGPUColorTargetState* data() { return vector.data(); }
};

class WebGPUVectorFeatureName {
    private:
        std::vector<WGPUFeatureName> vector;
    public:
        static WebGPUVectorFeatureName Obtain() {
            WebGPUVectorFeatureName obj;
            return obj;
        }
        int size() { return vector.size(); }
        void push_back(const WGPUFeatureName& attachment) { vector.push_back(attachment); }
        const WGPUFeatureName* data() { return vector.data(); }
};

class WebGPUVectorConstantEntry {
    private:
        std::vector<WebGPUConstantEntry> vector;
    public:
        static WebGPUVectorConstantEntry Obtain() {
            WebGPUVectorConstantEntry obj;
            return obj;
        }
        int size() { return vector.size(); }
        void push_back(const WebGPUConstantEntry& attachment) { vector.push_back(attachment); }
        const WebGPUConstantEntry* data() { return vector.data(); }
};

class WebGPUVectorVertexBufferLayout {
    private:
        std::vector<WebGPUVertexBufferLayout> vector;
    public:
        static WebGPUVectorVertexBufferLayout Obtain() {
            WebGPUVectorVertexBufferLayout obj;
            return obj;
        }
        int size() { return vector.size(); }
        void push_back(const WebGPUVertexBufferLayout& attachment) { vector.push_back(attachment); }
        const WebGPUVertexBufferLayout* data() { return vector.data(); }
};

class WebGPUVectorTextureFormat {
    private:
        std::vector<WGPUTextureFormat> vector;
    public:
        static WebGPUVectorTextureFormat Obtain() {
            WebGPUVectorTextureFormat obj;
            return obj;
        }
        int size() { return vector.size(); }
        void push_back(const WGPUTextureFormat& attachment) { vector.push_back(attachment); }
        const WGPUTextureFormat* data() { return vector.data(); }
};

class WebGPUVectorRenderBundle {
    private:
        std::vector<WebGPURenderBundle> vector;
    public:
        static WebGPUVectorRenderBundle Obtain() {
            WebGPUVectorRenderBundle obj;
            return obj;
        }
        int size() { return vector.size(); }
        void push_back(const WebGPURenderBundle& attachment) { vector.push_back(attachment); }
        const WebGPURenderBundle* data() { return vector.data(); }
};

class WebGPUVectorInt {
    private:
        std::vector<int> vector;
    public:
        static WebGPUVectorInt Obtain() {
            WebGPUVectorInt obj;
            return obj;
        }
        int size() { return vector.size(); }
        void push_back(int attachment) { vector.push_back(attachment); }
        const int* data() { return vector.data(); }
};

class WebGPUVectorRenderPassColorAttachment {
    private:
        std::vector<WebGPURenderPassColorAttachment> vector;
    public:
        static WebGPUVectorRenderPassColorAttachment Obtain() {
            WebGPUVectorRenderPassColorAttachment obj;
            return obj;
        }
        int size() { return vector.size(); }
        void push_back(const WebGPURenderPassColorAttachment& attachment) { vector.push_back(attachment); }
        const WebGPURenderPassColorAttachment* data() { return vector.data(); }
};

class WGPUAndroidWindow {
    public:
        #ifdef __ANDROID__
            ANativeWindow* g_window = nullptr;
        #endif

        ~WGPUAndroidWindow() {
            #ifdef __ANDROID__
                if(g_window != nullptr) {
                    ANativeWindow_release(g_window);
                }
            #endif
        }

        void SetWindow(void * window) {
            #ifdef __ANDROID__
                g_window = reinterpret_cast<ANativeWindow * >(window);
            #endif
        }

        void* GetWindow() {
            #ifdef __ANDROID__
                return g_window;
            #else
                return NULL;
            #endif
        }

        void InitLogcat() {
            #ifdef __ANDROID__
                __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Initializing Logcat redirection");
                LogcatRedirector::initialize();
            #endif

        }
};

class RequestAdapterCallback {
public:
    virtual void OnCallback(WGPURequestAdapterStatus status, WebGPUAdapter* adapter) = 0;

};

class RequestDeviceCallback {
public:
    virtual void OnCallback(WGPURequestDeviceStatus status, WebGPUDevice* device) = 0;

};

class UncapturedErrorCallback {
public:
    virtual void OnCallback(WGPUErrorType errorType, const char* message) = 0;

};

template<typename Derived, typename CType>
class WebGPUObjectBase {
    private:
        template<typename T>
        static T InitializeHandle() {
            if constexpr (std::is_pointer<T>::value) {
                return nullptr;
            } else {
                return T{};
            }
        }
    protected:
        CType mHandle = InitializeHandle<CType>();

        void AddRefInternal() {
            // MUST BE IMPLEMENTED if needed
        }

        void ReleaseInternal() {
             // MUST BE IMPLEMENTED if needed
        }

    public:

    using ReturnType = CType&;

    void AddRef() {
        if constexpr (std::is_pointer<CType>::value) {
            if (mHandle != nullptr) AddRefInternal();
        }
    }

    void Release() {
        if constexpr (std::is_pointer<CType>::value) {
            if (mHandle != nullptr) ReleaseInternal();
            mHandle = nullptr;
        }
    }

    ReturnType Get() {
        return mHandle;
    }

    void Set(CType handle) {
        mHandle = handle;
    }

    void Set(Derived& other) {
        mHandle = other.mHandle;
    }
};

class WebGPUStringView : public WebGPUObjectBase<WebGPUStringView, WGPUStringView> {
    public:

        WebGPUStringView() {
        }

        WebGPUStringView(WGPUStringView stringView) {
            Set(stringView);
        }
        WebGPUStringView(const char* value) {
            Get().data = strdup(value);
            Get().length = strlen(value);
        }

        const std::string GetString() {
            return std::string(Get().data, Get().length);
        }
};

class WebGPUCommandBuffer : public WebGPUObjectBase<WebGPUCommandBuffer, WGPUCommandBuffer> {
    protected:

        void AddRefInternal() {
            wgpuCommandBufferAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuCommandBufferRelease(Get());
        }

    public:

        static WebGPUCommandBuffer Obtain() {
            WebGPUCommandBuffer obj;
            return obj;
        }

};

class WebGPUQueue : public WebGPUObjectBase<WebGPUQueue, WGPUQueue> {
    protected:

        void AddRefInternal() {
            wgpuQueueAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuQueueRelease(Get());
        }

    public:

        void SetLabel(const char* value) {
            WebGPUStringView stringView(value);
            wgpuQueueSetLabel(Get(), stringView.Get());
        }

        void Submit(int commandCount, WebGPUCommandBuffer* commandBuffer) {
            wgpuQueueSubmit(Get(), commandCount, &(commandBuffer->Get()));
        }
};

class WebGPUChainedStruct : public WebGPUObjectBase<WebGPUChainedStruct, WGPUChainedStruct*> {
    private:

    public:
        void SetNext(WebGPUChainedStruct* value) {
            Get()->next = value->Get();
        }

        void SetSType(WGPUSType type) {
            Get()->sType = type;
        }
};

class WebGPULimits : public WebGPUObjectBase<WebGPULimits, WGPULimits> {
    public: // TODO wgpu-native limits is different from emscripten dawn limits

        static WebGPULimits Obtain() {
            WebGPULimits obj;
            return obj;
        }

        void SetMaxTextureDimension1D(int value) {
            Get().maxTextureDimension1D = value;
        }
        int GetMaxTextureDimension1D() {
            return static_cast<int>(Get().maxTextureDimension1D);
        }

        void SetMaxTextureDimension2D(int value) {
            Get().maxTextureDimension2D = value;
        }
        int GetMaxTextureDimension2D() {
            return static_cast<int>(Get().maxTextureDimension2D);
        }

        void SetMaxTextureDimension3D(int value) {
            Get().maxTextureDimension3D = value;
        }
        int GetMaxTextureDimension3D() {
            return static_cast<int>(Get().maxTextureDimension3D);
        }

        void SetMaxTextureArrayLayers(int value) {
            Get().maxTextureArrayLayers = value;
        }
        int GetMaxTextureArrayLayers() {
            return static_cast<int>(Get().maxTextureArrayLayers);
        }

        void SetMaxBindGroups(int value) {
            Get().maxBindGroups = value;
        }
        int GetMaxBindGroups() {
            return static_cast<int>(Get().maxBindGroups);
        }

        void SetMaxBindGroupsPlusVertexBuffers(int value) {
            Get().maxBindGroupsPlusVertexBuffers = value;
        }
        int GetMaxBindGroupsPlusVertexBuffers() {
            return static_cast<int>(Get().maxBindGroupsPlusVertexBuffers);
        }

        void SetMaxBindingsPerBindGroup(int value) {
            Get().maxBindingsPerBindGroup = value;
        }
        int GetMaxBindingsPerBindGroup() {
            return static_cast<int>(Get().maxBindingsPerBindGroup);
        }

        void SetMaxDynamicUniformBuffersPerPipelineLayout(int value) {
            Get().maxDynamicUniformBuffersPerPipelineLayout = value;
        }
        int GetMaxDynamicUniformBuffersPerPipelineLayout() {
            return static_cast<int>(Get().maxDynamicUniformBuffersPerPipelineLayout);
        }

        void SetMaxDynamicStorageBuffersPerPipelineLayout(int value) {
            Get().maxDynamicStorageBuffersPerPipelineLayout = value;
        }
        int GetMaxDynamicStorageBuffersPerPipelineLayout() {
            return static_cast<int>(Get().maxDynamicStorageBuffersPerPipelineLayout);
        }

        void SetMaxSampledTexturesPerShaderStage(int value) {
            Get().maxSampledTexturesPerShaderStage = value;
        }
        int GetMaxSampledTexturesPerShaderStage() {
            return static_cast<int>(Get().maxSampledTexturesPerShaderStage);
        }

        void SetMaxSamplersPerShaderStage(int value) {
            Get().maxSamplersPerShaderStage = value;
        }
        int GetMaxSamplersPerShaderStage() {
            return static_cast<int>(Get().maxSamplersPerShaderStage);
        }

        void SetMaxStorageBuffersPerShaderStage(int value) {
            Get().maxStorageBuffersPerShaderStage = value;
        }
        int GetMaxStorageBuffersPerShaderStage() {
            return static_cast<int>(Get().maxStorageBuffersPerShaderStage);
        }

        void SetMaxStorageTexturesPerShaderStage(int value) {
            Get().maxStorageTexturesPerShaderStage = value;
        }
        int GetMaxStorageTexturesPerShaderStage() {
            return static_cast<int>(Get().maxStorageTexturesPerShaderStage);
        }

        void SetMaxUniformBuffersPerShaderStage(int value) {
            Get().maxUniformBuffersPerShaderStage = value;
        }
        int GetMaxUniformBuffersPerShaderStage() {
            return static_cast<int>(Get().maxUniformBuffersPerShaderStage);
        }

        void SetMaxUniformBufferBindingSize(int value) {
            Get().maxUniformBufferBindingSize = value;
        }
        int GetMaxUniformBufferBindingSize() {
            return static_cast<int>(Get().maxUniformBufferBindingSize);
        }

        void SetMaxStorageBufferBindingSize(int value) {
            Get().maxStorageBufferBindingSize = value;
        }
        int GetMaxStorageBufferBindingSize() {
            return static_cast<int>(Get().maxStorageBufferBindingSize);
        }

        void SetMinUniformBufferOffsetAlignment(int value) {
            Get().minUniformBufferOffsetAlignment = value;
        }
        int GetMinUniformBufferOffsetAlignment() {
            return static_cast<int>(Get().minUniformBufferOffsetAlignment);
        }

        void SetMinStorageBufferOffsetAlignment(int value) {
            Get().minStorageBufferOffsetAlignment = value;
        }
        int GetMinStorageBufferOffsetAlignment() {
            return static_cast<int>(Get().minStorageBufferOffsetAlignment);
        }

        void SetMaxVertexBuffers(int value) {
            Get().maxVertexBuffers = value;
        }
        int GetMaxVertexBuffers() {
            return static_cast<int>(Get().maxVertexBuffers);
        }

        void SetMaxBufferSize(int value) {
            Get().maxBufferSize = value;
        }
        int GetMaxBufferSize() {
            return static_cast<int>(Get().maxBufferSize);
        }

        void SetMaxVertexAttributes(int value) {
            Get().maxVertexAttributes = value;
        }
        int GetMaxVertexAttributes() {
            return static_cast<int>(Get().maxVertexAttributes);
        }

        void SetMaxVertexBufferArrayStride(int value) {
            Get().maxVertexBufferArrayStride = value;
        }
        int GetMaxVertexBufferArrayStride() {
            return static_cast<int>(Get().maxVertexBufferArrayStride);
        }

        void SetMaxInterStageShaderVariables(int value) {
            Get().maxInterStageShaderVariables = value;
        }
        int GetMaxInterStageShaderVariables() {
            return static_cast<int>(Get().maxInterStageShaderVariables);
        }

        void SetMaxColorAttachments(int value) {
            Get().maxColorAttachments = value;
        }
        int GetMaxColorAttachments() {
            return static_cast<int>(Get().maxColorAttachments);
        }

        void SetMaxColorAttachmentBytesPerSample(int value) {
            Get().maxColorAttachmentBytesPerSample = value;
        }
        int GetMaxColorAttachmentBytesPerSample() {
            return static_cast<int>(Get().maxColorAttachmentBytesPerSample);
        }

        void SetMaxComputeWorkgroupStorageSize(int value) {
            Get().maxComputeWorkgroupStorageSize = value;
        }
        int GetMaxComputeWorkgroupStorageSize() {
            return static_cast<int>(Get().maxComputeWorkgroupStorageSize);
        }

        void SetMaxComputeInvocationsPerWorkgroup(int value) {
            Get().maxComputeInvocationsPerWorkgroup = value;
        }
        int GetMaxComputeInvocationsPerWorkgroup() {
            return static_cast<int>(Get().maxComputeInvocationsPerWorkgroup);
        }

        void SetMaxComputeWorkgroupSizeX(int value) {
            Get().maxComputeWorkgroupSizeX = value;
        }
        int GetMaxComputeWorkgroupSizeX() {
            return static_cast<int>(Get().maxComputeWorkgroupSizeX);
        }

        void SetMaxComputeWorkgroupSizeY(int value) {
            Get().maxComputeWorkgroupSizeY = value;
        }
        int GetMaxComputeWorkgroupSizeY() {
            return static_cast<int>(Get().maxComputeWorkgroupSizeY);
        }

        void SetMaxComputeWorkgroupSizeZ(int value) {
            Get().maxComputeWorkgroupSizeZ = value;
        }
        int GetMaxComputeWorkgroupSizeZ() {
            return static_cast<int>(Get().maxComputeWorkgroupSizeZ);
        }

        void SetMaxComputeWorkgroupsPerDimension(int value) {
            Get().maxComputeWorkgroupsPerDimension = value;
        }
        int GetMaxComputeWorkgroupsPerDimension() {
            return static_cast<int>(Get().maxComputeWorkgroupsPerDimension);
        }
};

class WebGPUQueueDescriptor : public WebGPUObjectBase<WebGPUQueueDescriptor, WGPUQueueDescriptor*> {
        public:

        void SetLabel(const char* value) {
            WebGPUStringView stringView(value);
            Get()->label = stringView.Get();
        }
};

class WebGPUDeviceDescriptor : public WebGPUObjectBase<WebGPUDeviceDescriptor, WGPUDeviceDescriptor> {
    public:

        static WebGPUDeviceDescriptor Obtain() {
            WebGPUDeviceDescriptor obj;
            return obj;
        }

        void SetLabel(const char* value) {
            WebGPUStringView stringView(value);
            Get().label = stringView.Get();
        }

        void SetRequiredLimits(WebGPULimits* limits) {
            Get().requiredLimits = &(limits->Get());
        }

        void SetRequiredFeatures(WebGPUVectorFeatureName* features) {
            if(features != NULL) {
                Get().requiredFeatureCount = features->size();
                Get().requiredFeatures = features->data();
            }
            else {
                Get().requiredFeatureCount = 0;
                Get().requiredFeatures = NULL;
            }
        }

        WebGPUQueueDescriptor GetDefaultQueue() {
            WebGPUQueueDescriptor temp;
            temp.Set(&Get().defaultQueue);
            return temp;
        }
};

class WebGPURequestAdapterOptions : public WebGPUObjectBase<WebGPURequestAdapterOptions, WGPURequestAdapterOptions> {
    public:

        static WebGPURequestAdapterOptions Obtain() {
            WebGPURequestAdapterOptions obj;
            return obj;
        }
};

class WebGPUBindGroup : public WebGPUObjectBase<WebGPUBindGroup, WGPUBindGroup> {
    private:

    public:
};

class WebGPURenderBundle : public WebGPUObjectBase<WebGPURenderBundle, WGPURenderBundle> {
    private:

    public:

};

class WebGPUAdapterInfo : public WebGPUObjectBase<WebGPUAdapterInfo, WGPUAdapterInfo> {
    public:

        static WebGPUAdapterInfo Obtain() {
            WebGPUAdapterInfo obj;
            return obj;
        }

        std::string GetVendor() {
            WebGPUStringView stringView(Get().vendor);
            return stringView.GetString();
        }

        std::string GetArchitecture() {
            WebGPUStringView stringView(Get().architecture);
            return stringView.GetString();
        }

        std::string GetDevice() {
            WebGPUStringView stringView(Get().device);
            return stringView.GetString();
        }

        std::string GetDescription() {
            WebGPUStringView stringView(Get().description);
            return stringView.GetString();
        }

        WGPUBackendType GetBackendType() {
            return Get().backendType;
        }

        WGPUAdapterType GetAdapterType() {
            return Get().adapterType;
        }
};

class WebGPUShaderModule : public WebGPUObjectBase<WebGPUShaderModule, WGPUShaderModule> {
    protected:

        void AddRefInternal() {
            wgpuShaderModuleAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuShaderModuleRelease(Get());
        }

    public:

        static WebGPUShaderModule Obtain() {
            WebGPUShaderModule obj;
            return obj;
        }
};

class WebGPUConstantEntry : public WebGPUObjectBase<WebGPUConstantEntry, WGPUConstantEntry> {
    public:
};

class WebGPUVertexBufferLayout : public WebGPUObjectBase<WebGPUVertexBufferLayout, WGPUVertexBufferLayout> {
    public:
};

class WebGPUVertexState : public WebGPUObjectBase<WebGPUVertexState, WGPUVertexState*> {
    public:

        void SetNextInChain(WebGPUChainedStruct* chainedStruct) {
            Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetModule(WebGPUShaderModule* shaderModule) {
            Get()->module = shaderModule != NULL ? shaderModule->Get() : NULL;
        }

        void SetEntryPoint(const char* value) {
            WebGPUStringView stringView(value);
            Get()->entryPoint = stringView.Get();
        }

        void SetConstants(WebGPUVectorConstantEntry* values) {
            if(values != NULL) {
                Get()->constantCount = values->size();
                Get()->constants = reinterpret_cast<const WGPUConstantEntry*>(values->data());
            }
            else {
                Get()->constantCount = 0;
                Get()->constants = NULL;
            }
        }

        void SetBuffers(WebGPUVectorVertexBufferLayout* values) {
            if(values != NULL) {
                Get()->bufferCount = values->size();
                Get()->buffers = reinterpret_cast<const WGPUVertexBufferLayout*>(values->data());
            }
            else {
                Get()->bufferCount = 0;
                Get()->buffers = NULL;
            }
        }
};

class WebGPUShaderSourceWGSL : public WebGPUObjectBase<WebGPUShaderSourceWGSL, WGPUShaderSourceWGSL> {
    public:

        static WebGPUShaderSourceWGSL Obtain() {
            WebGPUShaderSourceWGSL obj;
            return obj;
        }

        void SetCode(const char* value) {
            WebGPUStringView stringView(value);
            Get().code = stringView.Get();
        }

        void SetNext(WebGPUChainedStruct* value) {
            Get().chain.next = value != NULL ? value->Get() : NULL;
        }

        void SetSType(WGPUSType type) {
            Get().chain.sType = type;
        }

        WebGPUChainedStruct GetChain() {
            WGPUChainedStruct* wgpuChain = &Get().chain;
            WebGPUChainedStruct chain;
            chain.Set(wgpuChain);
            return chain;
        }
};

class WebGPUShaderModuleDescriptor : public WebGPUObjectBase<WebGPUShaderModuleDescriptor, WGPUShaderModuleDescriptor> {
    public:

        static WebGPUShaderModuleDescriptor Obtain() {
            WebGPUShaderModuleDescriptor obj;
            return obj;
        }

        void SetNextInChain(WebGPUChainedStruct* chainedStruct) {
            Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetLabel(const char* value) {
            WebGPUStringView stringView(value);
            Get().label = stringView.Get();
        }
};

class WebGPUBlendComponent : public WebGPUObjectBase<WebGPUBlendComponent, WGPUBlendComponent*> {
    public:

        void SetOperation(WGPUBlendOperation operation) {
            Get()->operation = operation;
        }

        WGPUBlendOperation GetOperation() {
            return Get()->operation;
        }

        void SetSrcFactor(WGPUBlendFactor factor) {
            Get()->srcFactor = factor;
        }

        WGPUBlendFactor GetSrcFactor() {
            return Get()->srcFactor;
        }

        void SetDstFactor(WGPUBlendFactor factor) {
            Get()->dstFactor = factor;
        }

        WGPUBlendFactor GetDstFactor() {
            return Get()->dstFactor;
        }
};

class WebGPUBlendState : public WebGPUObjectBase<WebGPUBlendState, WGPUBlendState> {
    public:

        static WebGPUBlendState Obtain() {
            WebGPUBlendState obj;
            return obj;
        }

        WebGPUBlendComponent GetColor() {
            WebGPUBlendComponent temp;
            temp.Set(&Get().color);
            return temp;
        }

        WebGPUBlendComponent GetAlpha() {
            WebGPUBlendComponent temp;
            temp.Set(&Get().alpha);
            return temp;
        }
};

class WebGPUColorTargetState : public WebGPUObjectBase<WebGPUColorTargetState, WGPUColorTargetState> {
    public:

        static WebGPUColorTargetState Obtain() {
            WebGPUColorTargetState obj;
            return obj;
        }

        void SetNextInChain(WebGPUChainedStruct* chainedStruct) {
            Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetFormat(WGPUTextureFormat format) {
            Get().format = format;
        }

        void SetBlend(WebGPUBlendState* blendState) {
            Get().blend = reinterpret_cast<const WGPUBlendState*>(blendState);
        }

        void SetWriteMask(WGPUColorWriteMask writeMask) {
            Get().writeMask = writeMask;
        }
};

class WebGPUFragmentState : public WebGPUObjectBase<WebGPUFragmentState, WGPUFragmentState> {
    public:

        static WebGPUFragmentState Obtain() {
            WebGPUFragmentState obj;
            return obj;
        }

        void SetNextInChain(WebGPUChainedStruct* chainedStruct) {
            Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetEntryPoint(const char* value) {
            WebGPUStringView stringView(value);
            Get().entryPoint = stringView.Get();
        }

        void SetTargets(WebGPUVectorColorTargetState* values) {
            if(values != NULL) {
                Get().targetCount = values->size();
                Get().targets = reinterpret_cast<const WGPUColorTargetState*>(values->data());
            }
            else {
                Get().targetCount = 0;
                Get().targets = NULL;
            }
        }

        void SetModule(WebGPUShaderModule* shaderModule) {
            Get().module = shaderModule != NULL ? shaderModule->Get() : NULL;
        }

        void SetConstants(WebGPUVectorConstantEntry* values) {
            if(values != NULL) {
                Get().constantCount = values->size();
                Get().constants = reinterpret_cast<const WGPUConstantEntry*>(values->data());
            }
            else {
                Get().constantCount = 0;
                Get().constants = NULL;
            }
        }
};

class WebGPUPrimitiveState : public WebGPUObjectBase<WebGPUPrimitiveState, WGPUPrimitiveState*> {
    public:

        void SetNextInChain(WebGPUChainedStruct* chainedStruct) {
            Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetTopology(WGPUPrimitiveTopology value) {
            Get()->topology = value;
        }

        void SetStripIndexFormat(WGPUIndexFormat value) {
            Get()->stripIndexFormat = value;
        }

        void SetFrontFace(WGPUFrontFace value) {
            Get()->frontFace = value;
        }

        void SetCullMode(WGPUCullMode value) {
            Get()->cullMode = value;
        }
};

class WebGPUDepthStencilState : public WebGPUObjectBase<WebGPUDepthStencilState, WGPUDepthStencilState> {
    public:

        static WebGPUDepthStencilState Obtain() {
            WebGPUDepthStencilState obj;
            return obj;
        }

        void SetNextInChain(WebGPUChainedStruct* chainedStruct) {
            Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetFormat(WGPUTextureFormat format) {
            Get().format = format;
        }

        void SetDepthWriteEnabled(WGPUOptionalBool depthWriteEnabled) {
            Get().depthWriteEnabled = depthWriteEnabled;
        }

        void SetDepthCompare(WGPUCompareFunction depthCompare) {
            Get().depthCompare = depthCompare;
        }

        void SetDepthBiasSlopeScale(float depthBiasSlopeScale) {
            Get().depthBiasSlopeScale = depthBiasSlopeScale;
        }

        void SetDepthBiasClamp(float depthBiasClamp) {
            Get().depthBiasClamp = depthBiasClamp;
        }
};

class WebGPUMultisampleState : public WebGPUObjectBase<WebGPUMultisampleState, WGPUMultisampleState*> {
    public:
        void SetNextInChain(WebGPUChainedStruct* chainedStruct) {
            Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetCount(int count) {
            Get()->count = count;
        }

        void SetMask(int mask) {
            Get()->mask = mask;
        }

        void SetAlphaToCoverageEnabled(int alphaToCoverageEnabled) {
            Get()->alphaToCoverageEnabled = alphaToCoverageEnabled;
        }
};

class WebGPUPipelineLayout : public WebGPUObjectBase<WebGPUPipelineLayout, WGPUPipelineLayout> {
    public:
};

class WebGPURenderPipelineDescriptor : public WebGPUObjectBase<WebGPURenderPipelineDescriptor, WGPURenderPipelineDescriptor> {
    public:

        static WebGPURenderPipelineDescriptor Obtain() {
            WebGPURenderPipelineDescriptor obj;
            return obj;
        }

        void SetNextInChain(WebGPUChainedStruct* chainedStruct) {
            Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetLabel(const char* value) {
            WebGPUStringView stringView(value);
            Get().label = stringView.Get();
        }

        WebGPUVertexState GetVertex() {
            WebGPUVertexState temp;
            temp.Set(&Get().vertex);
            return temp;
        }

        WebGPUPrimitiveState GetPrimitive() {
            WebGPUPrimitiveState temp;
            temp.Set(&Get().primitive);
            return temp;
        }

        void SetFragment(WebGPUFragmentState* fragment) {
            Get().fragment = &fragment->Get();
        }

        void SetDepthStencil(WebGPUDepthStencilState* depthStencilState) {
            Get().depthStencil = depthStencilState != NULL ? &depthStencilState->Get() : NULL;
        }

        WebGPUMultisampleState GetMultisample() {
            WebGPUMultisampleState temp;
            temp.Set(&Get().multisample);
            return temp;
        }

        void SetLayout(WebGPUPipelineLayout* pipelineLayout) {
            Get().layout = pipelineLayout != NULL ? pipelineLayout->Get() : NULL;
        }
};

class WebGPURenderPipeline : public WebGPUObjectBase<WebGPURenderPipeline, WGPURenderPipeline> {

    protected:
        void AddRefInternal() {
            wgpuRenderPipelineAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuRenderPipelineRelease(Get());
        }

    public:
        static WebGPURenderPipeline Obtain() {
            WebGPURenderPipeline obj;
            return obj;
        }
};

class WebGPUColor : public WebGPUObjectBase<WebGPUColor, WGPUColor*> {
    private:
        bool deleteObject;

    public:
        WebGPUColor() {
            deleteObject = true;
            Set(new WGPUColor());
        }

        WebGPUColor(WGPUColor* color) {
            deleteObject = false;
            Set(color);
        }

        ~WebGPUColor() {
            if(deleteObject) {
                delete Get();
            }
        }

        static WebGPUColor Obtain() {
            WebGPUColor obj;
            return obj;
        }

        void SetColor(float r, float g, float b, float a) {
            Get()->r = r;
            Get()->g = g;
            Get()->b = b;
            Get()->a = a;
        }

        float GetR() {
            return Get()->r;
        }
        float GetG() {
            return Get()->g;
        }
        float GetB() {
            return Get()->b;
        }
        float GetA() {
            return Get()->a;
        }
};

class WebGPUSupportedFeatures : public WebGPUObjectBase<WebGPUSupportedFeatures, WGPUSupportedFeatures> {
    public:

        static WebGPUSupportedFeatures Obtain() {
            WebGPUSupportedFeatures obj;
            return obj;
        }

        int GetFeatureCount() {
            return Get().featureCount;
        }

        WGPUFeatureName GetFeatureAt(long index) {
            return Get().features[index];
        }
};

class WebGPUBuffer : public WebGPUObjectBase<WebGPUBuffer, WGPUBuffer> {
    public:
};

class WebGPURenderPassEncoder : public WebGPUObjectBase<WebGPURenderPassEncoder, WGPURenderPassEncoder> {
    protected:

        void AddRefInternal() {
            wgpuRenderPassEncoderAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuRenderPassEncoderRelease(Get());
        }

    public:

        static WebGPURenderPassEncoder Obtain() {
            WebGPURenderPassEncoder obj;
            return obj;
        }

        void End() {
            wgpuRenderPassEncoderEnd(Get());
        }

        void SetPipeline(WebGPURenderPipeline* renderPipeline) {
            wgpuRenderPassEncoderSetPipeline(Get(), renderPipeline->Get());
        }

        void BeginOcclusionQuery(int queryIndex) {
            wgpuRenderPassEncoderBeginOcclusionQuery(Get(), queryIndex);
        }

        void Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance) {
            wgpuRenderPassEncoderDraw(Get(), vertexCount, instanceCount, firstVertex, firstInstance);
        }

        void DrawIndexed(int indexCount, int instanceCount, int firstIndex, int baseVertex, int firstInstance) {
            wgpuRenderPassEncoderDrawIndexed(Get(), indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
        }

        void DrawIndexedIndirect(WebGPUBuffer* indirectBuffer, int indirectOffset) {
            wgpuRenderPassEncoderDrawIndexedIndirect(Get(), indirectBuffer->Get(), indirectOffset);
        }

        void DrawIndirect(WebGPUBuffer* indirectBuffer, int indirectOffset) {
            wgpuRenderPassEncoderDrawIndirect(Get(), indirectBuffer->Get(), indirectOffset);
        }

        void EndOcclusionQuery() {
            wgpuRenderPassEncoderEndOcclusionQuery(Get());
        }

        void ExecuteBundles(WebGPUVectorRenderBundle* values) {
            if(values != NULL) {
                int size = values->size();
                wgpuRenderPassEncoderExecuteBundles(Get(), size,  reinterpret_cast<WGPURenderBundle const * >(values->data()));
            }
            else {
                wgpuRenderPassEncoderExecuteBundles(Get(), 0,  NULL);
            }
        }

        void InsertDebugMarker(const char* label) {
            WebGPUStringView stringView(label);
            wgpuRenderPassEncoderInsertDebugMarker(Get(), stringView.Get());
        }

        void PopDebugGroup() {
            wgpuRenderPassEncoderPopDebugGroup(Get());
        }

        void PushDebugGroup(const char* label) {
            WebGPUStringView stringView(label);
            wgpuRenderPassEncoderPushDebugGroup(Get(), stringView.Get());
        }

        void SetBindGroup(int groupIndex, WebGPUBindGroup* group, WebGPUVectorInt* dynamicOffsets) {
            // TODO test. May not work if Int to uint32_t fails
            int dynamicOffsetCount = dynamicOffsets->size();
            wgpuRenderPassEncoderSetBindGroup(Get(), groupIndex, group->Get(), dynamicOffsetCount, reinterpret_cast<uint32_t const *>(dynamicOffsets->data()));
        }

        void SetBlendConstant(WebGPUColor* color) {
            // TODO need to test if this is working
            wgpuRenderPassEncoderSetBlendConstant(Get(), reinterpret_cast<WGPUColor *>(color));
        }

        void SetIndexBuffer(WebGPUBuffer* buffer, WGPUIndexFormat format, int offset, int size) {
            wgpuRenderPassEncoderSetIndexBuffer(Get(), buffer->Get(), format, offset, size);
        }

        void SetLabel(const char* label) {
            WebGPUStringView stringView(label);
            wgpuRenderPassEncoderSetLabel(Get(), stringView.Get());
        }

        void SetScissorRect(int x, int y, int width, int height) {
            wgpuRenderPassEncoderSetScissorRect(Get(), x, y, width, height);
        }

        void SetStencilReference(int reference) {
            wgpuRenderPassEncoderSetStencilReference(Get(), reference);
        }

        void SetVertexBuffer(int slot, WebGPUBuffer* buffer, int offset, int size) {
            wgpuRenderPassEncoderSetVertexBuffer(Get(), slot, buffer->Get(), offset, size);
        }

        void SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth) {
            wgpuRenderPassEncoderSetViewport(Get(), x, y, width, height, minDepth, maxDepth);
        }
};

class WebGPUComputePassEncoder : public WebGPUObjectBase<WebGPUComputePassEncoder, WGPUComputePassEncoder> {
    protected:

        void AddRefInternal() {
            wgpuComputePassEncoderAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuComputePassEncoderRelease(Get());
        }

    public:

        static WebGPUComputePassEncoder Obtain() {
            WebGPUComputePassEncoder obj;
            return obj;
        }
};

class WebGPURenderPassDepthStencilAttachment : public WebGPUObjectBase<WebGPURenderPassDepthStencilAttachment, WGPURenderPassDepthStencilAttachment> {
    public:
        static WebGPURenderPassDepthStencilAttachment Obtain() {
            WebGPURenderPassDepthStencilAttachment obj;
            return obj;
        }

};

// Opaque pointer
class WebGPUQuerySet : public WebGPUObjectBase<WebGPUQuerySet, WGPUQuerySet> {
    protected:
        void AddRefInternal() {
            wgpuQuerySetAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuQuerySetRelease(Get());
        }

    public:

        void SetDestroy() {
            wgpuQuerySetDestroy(Get());
        }

        int GetCount() {
            return wgpuQuerySetGetCount(Get());
        }

        WGPUQueryType GetType() {
            return wgpuQuerySetGetType(Get());
        }

        void SetLabel(const char* label) {
            WebGPUStringView stringView(label);
            wgpuQuerySetSetLabel(Get(), stringView.Get());
        }
};

// TODO The class name differs from those in Dawn and wgpu-native.
class WebGPURenderPassTimestampWrites : public WebGPUObjectBase<WebGPURenderPassTimestampWrites, WGPURenderPassTimestampWrites> {
    public:

        static WebGPURenderPassTimestampWrites Obtain() {
            WebGPURenderPassTimestampWrites obj;
            return obj;
        }

        void SetQuerySet(WebGPUQuerySet* value) {
            Get().querySet = value->Get();
        }

        void SetBeginningOfPassWriteIndex(int value) {
            Get().beginningOfPassWriteIndex = value;
        }

        void SetEndOfPassWriteIndex(int value) {
            Get().endOfPassWriteIndex = value;
        }
};

class WebGPUCommandEncoderDescriptor : public WebGPUObjectBase<WebGPUCommandEncoderDescriptor, WGPUCommandEncoderDescriptor> {
    public:

        static WebGPUCommandEncoderDescriptor Obtain() {
            WebGPUCommandEncoderDescriptor obj;
            return obj;
        }

        void SetLabel(const char* value) {
            WebGPUStringView stringView(value);
            Get().label = stringView.Get();
        }
};

class WebGPUCommandBufferDescriptor : public WebGPUObjectBase<WebGPUCommandBufferDescriptor, WGPUCommandBufferDescriptor> {
    public:

        static WebGPUCommandBufferDescriptor Obtain() {
            WebGPUCommandBufferDescriptor obj;
            return obj;
        }

        void SetNextInChain(WebGPUChainedStruct* chainedStruct) {
            Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetLabel(const char* value) {
            WebGPUStringView stringView(value);
            Get().label = stringView.Get();
        }
};

class WebGPURenderPassDescriptor : public WebGPUObjectBase<WebGPURenderPassDescriptor, WGPURenderPassDescriptor> {
    public:

        static WebGPURenderPassDescriptor Obtain() {
            WebGPURenderPassDescriptor obj;
            return obj;
        }

        void SetLabel(const char* value) {
            WebGPUStringView stringView(value);
            Get().label = stringView.Get();
        }

        void SetColorAttachments(WebGPUVectorRenderPassColorAttachment* values) {
            if(values != NULL) {
                Get().colorAttachmentCount = values->size();
                Get().colorAttachments = reinterpret_cast<const WGPURenderPassColorAttachment*>(values->data());
            }
            else {
                Get().colorAttachmentCount = 0;
                Get().colorAttachments = NULL;
            }
        }

        void SetDepthStencilAttachment(WebGPURenderPassDepthStencilAttachment* attachment) {
            Get().depthStencilAttachment = attachment == NULL ? NULL : &(attachment->Get());
        }

        void SetTimestampWrites(WebGPURenderPassTimestampWrites* timestampWrites) {
            Get().timestampWrites = timestampWrites == NULL ? NULL : &(timestampWrites->Get());
        }
};

class WebGPUComputePassDescriptor : public WebGPUObjectBase<WebGPUComputePassDescriptor, WGPUComputePassDescriptor> {
    public:

        static WebGPUComputePassDescriptor Obtain() {
            WebGPUComputePassDescriptor obj;
            return obj;
        }

        void SetLabel(const char* value) {
            WebGPUStringView stringView(value);
            Get().label = stringView.Get();
        }
};

class WebGPUCommandEncoder : public WebGPUObjectBase<WebGPUCommandEncoder, WGPUCommandEncoder> {
    protected:

        void AddRefInternal() {
            wgpuCommandEncoderAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuCommandEncoderRelease(Get());
        }

    public:

        static WebGPUCommandEncoder Obtain() {
            WebGPUCommandEncoder obj;
            return obj;
        }

        void BeginRenderPass(WebGPURenderPassDescriptor* renderPassDescriptor, WebGPURenderPassEncoder* encoder) {
            encoder->Set(wgpuCommandEncoderBeginRenderPass(Get(), &(renderPassDescriptor->Get())));
        }

        void BeginComputePass(WebGPUComputePassEncoder* encoder) {
            WGPUComputePassDescriptor computePassDescriptor;
            encoder->Set(wgpuCommandEncoderBeginComputePass(Get(), &computePassDescriptor));
        }

        void ClearBuffer(WebGPUBuffer* buffer, int offset, int size) {
            wgpuCommandEncoderClearBuffer(Get(), buffer->Get(), offset, size);
        }

        void Finish(WebGPUCommandBufferDescriptor* commandBufferDescriptor, WebGPUCommandBuffer* commandBuffer) {
            commandBuffer->Set(wgpuCommandEncoderFinish(Get(), &(commandBufferDescriptor->Get())));
        }
};

class WebGPUDevice : public WebGPUObjectBase<WebGPUDevice, WGPUDevice> {
    protected:

        void AddRefInternal() {
            wgpuDeviceAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuDeviceRelease(Get());
        }

    public:

//        bool GetAdapterInfo(WebGPUAdapterInfo* adapterInfo) {
//            #ifdef __EMSCRIPTEN__
//                WGPUStatus status = wgpuDeviceGetAdapterInfo(device, &(adapterInfo->adapterInfo));
//            #else
//                wgpuDeviceGetAdapterInfo(device); // Not implemented
//                WGPUStatus status = WGPUStatus_Error;
//            #endif
//            return status == WGPUStatus_Success;
//        }

        WebGPUQueue GetQueue() {
            WebGPUQueue temp;
            temp.Set(wgpuDeviceGetQueue(Get()));
            return temp;
        }

        void CreateRenderPipeline(WebGPURenderPipelineDescriptor* pipelineDescriptor, WebGPURenderPipeline* renderPipeline) {
            renderPipeline->Set(wgpuDeviceCreateRenderPipeline(Get(), reinterpret_cast<WGPURenderPipelineDescriptor const * >(pipelineDescriptor)));
        }

        void CreateShaderModule(WebGPUShaderModuleDescriptor* shaderModuleDescriptor, WebGPUShaderModule* shaderModule) {
            shaderModule->Set(wgpuDeviceCreateShaderModule(Get(), &shaderModuleDescriptor->Get()));
        }

        void GetFeatures(WebGPUSupportedFeatures* features) {
            wgpuDeviceGetFeatures(Get(), reinterpret_cast<WGPUSupportedFeatures * >(features));
        }

        void GetLimits(WebGPULimits* limits) {
            wgpuDeviceGetLimits(Get(), reinterpret_cast<WGPULimits * >(&(limits->Get())));
        }

        void CreateCommandEncoder(WebGPUCommandEncoderDescriptor* encoderDescriptor, WebGPUCommandEncoder* encoder) {
            encoder->Set(wgpuDeviceCreateCommandEncoder(Get(), &(encoderDescriptor->Get())));
        }
};

class WebGPUSurfaceConfiguration : public WebGPUObjectBase<WebGPUSurfaceConfiguration, WGPUSurfaceConfiguration> {
    public:

        static WebGPUSurfaceConfiguration Obtain() {
            WebGPUSurfaceConfiguration temp;
            return temp;
        }

        void SetNextInChain(WebGPUChainedStruct* chainedStruct) {
            Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetWidth(long width) {
            Get().width = width;
        }

        void SetHeight(long height) {
            Get().height = height;
        }

        void SetFormat(WGPUTextureFormat format) {
            Get().format = format;
        }

        void SetViewFormatCount(long value) {
            Get().viewFormatCount = value;
        }

        void SetViewFormats(WebGPUVectorTextureFormat* values) {
            if(values != NULL) {
                Get().viewFormatCount = values->size();
                Get().viewFormats = reinterpret_cast<const WGPUTextureFormat*>(values->data());
            }
            else {
                Get().viewFormatCount = 0;
                Get().viewFormats = NULL;
            }
        }

        void SetUsage(WGPUTextureUsage usage) {
            Get().usage = usage;
        }

        void SetDevice(WebGPUDevice* device) {
            Get().device = device != NULL ? device->Get() : NULL;
        }

        void SetPresentMode(WGPUPresentMode presentMode) {
            Get().presentMode = presentMode;
        }

        void SetAlphaMode(WGPUCompositeAlphaMode alphaMode) {
            Get().alphaMode = alphaMode;
        }
};

class WebGPUSurfaceCapabilities : public WebGPUObjectBase<WebGPUSurfaceCapabilities, WGPUSurfaceCapabilities> {
    public:

        static WebGPUSurfaceCapabilities Obtain() {
            WebGPUSurfaceCapabilities obj;
            return obj;
        }

        WGPUTextureFormat GetFormats(int index) {
            return Get().formats[index];
        }
};

class WebGPUAdapter : public WebGPUObjectBase<WebGPUAdapter, WGPUAdapter> {
    protected:

        void AddRefInternal() {
            wgpuAdapterAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuAdapterRelease(Get());
        }

    public:

        void RequestDevice(WebGPUDeviceDescriptor* descriptor, WGPUCallbackMode mode, RequestDeviceCallback* callback, UncapturedErrorCallback* errorCallback) {
            descriptor->Get().uncapturedErrorCallbackInfo.callback = [](const WGPUDevice* device, WGPUErrorType type, WGPUStringView message, void* callback_param, void* userdata_param) {
                UncapturedErrorCallback* cback = reinterpret_cast<UncapturedErrorCallback*>(callback_param);
                cback->OnCallback(type, message.data);
           };
           descriptor->Get().uncapturedErrorCallbackInfo.userdata1 = reinterpret_cast<void*>(errorCallback);
           descriptor->Get().uncapturedErrorCallbackInfo.userdata2 = NULL;

            WGPURequestDeviceCallbackInfo callbackInfo = {};
            callbackInfo.mode = mode;
            callbackInfo.callback = [](WGPURequestDeviceStatus status, WGPUDevice dev, WGPUStringView message, void* callback_param, void* userdata_param) {
                RequestDeviceCallback* cback = reinterpret_cast<RequestDeviceCallback*>(callback_param);
                WebGPUDevice* device = new WebGPUDevice();
                device->Set(dev);
                cback->OnCallback(status, device);
            };
            callbackInfo.userdata1 = reinterpret_cast<void*>(callback);
            callbackInfo.userdata2 = NULL;
            const WGPUDeviceDescriptor* desc = reinterpret_cast<WGPUDeviceDescriptor const*>(descriptor);
            auto result = wgpuAdapterRequestDevice(Get(), desc, callbackInfo);
        }

        bool GetInfo(WebGPUAdapterInfo* adapterInfo) {
            WGPUStatus status = wgpuAdapterGetInfo(Get(), &(adapterInfo->Get()));
            return status == WGPUStatus_Success;
        }

        bool HasFeature(WGPUFeatureName featureName) {
            return wgpuAdapterHasFeature(Get(), featureName);
        }
};

//struct WebGPUTextureViewDescriptor {
//    WebGPUChainedStruct const * nextInChain = NULL;
//    WGPUStringView label;
//    WGPUTextureFormat format;
//    WGPUTextureViewDimension dimension;
//    uint32_t baseMipLevel;
//    uint32_t mipLevelCount;
//    uint32_t baseArrayLayer;
//    uint32_t arrayLayerCount;
//    WGPUTextureAspect aspect;
//    WGPUTextureUsage usage;

//    inline operator const WGPUTextureViewDescriptor&() const noexcept {
//        return *reinterpret_cast<const WGPUTextureViewDescriptor*>(this);
//    }
//}

class WebGPUTextureViewDescriptor : public WebGPUObjectBase<WebGPUTextureViewDescriptor, WGPUTextureViewDescriptor> {
    public:

    static WebGPUTextureViewDescriptor Obtain() {
        WebGPUTextureViewDescriptor obj;
        return obj;
    }

    void SetLabel(const char* value) {
        WebGPUStringView stringView(value);
        Get().label = stringView.Get();
    }

    void SetFormat(WGPUTextureFormat format) {
        Get().format = format;
    }

    void SetDimension(WGPUTextureViewDimension dimension) {
        Get().dimension = dimension;
    }

    void SetBaseMipLevel(int baseMipLevel) {
        Get().baseMipLevel = baseMipLevel;
    }

    void SetMipLevelCount(int mipLevelCount) {
        Get().mipLevelCount = mipLevelCount;
    }

    void SetBaseArrayLayer(int baseArrayLayer) {
        Get().baseArrayLayer = baseArrayLayer;
    }

    void SetArrayLayerCount(int arrayLayerCount) {
        Get().arrayLayerCount = arrayLayerCount;
    }

    void SetAspect(WGPUTextureAspect aspect) {
        Get().aspect = aspect;
    }

    void SetUsage(WGPUTextureUsage usage) {
        Get().usage = usage;
    }
};

class WebGPUTextureView : public WebGPUObjectBase<WebGPUTextureView, WGPUTextureView> {
    protected:

        void AddRefInternal() {
            wgpuTextureViewAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuTextureViewRelease(Get());
        }

    public:

        static WebGPUTextureView Obtain() {
            WebGPUTextureView obj;
            return obj;
        }
};

class WebGPUTexture : public WebGPUObjectBase<WebGPUTexture, WGPUTexture> {
    protected:

        void AddRefInternal() {
            wgpuTextureAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuTextureRelease(Get());
        }

    public:

        static WebGPUTexture Obtain() {
            WebGPUTexture obj;
            return obj;
        }

        void CreateView(WebGPUTextureViewDescriptor* textureViewDescriptor, WebGPUTextureView* textureView) {
            textureView->Set(wgpuTextureCreateView(Get(), &(textureViewDescriptor->Get())));
        }

        WGPUTextureFormat GetFormat() {
            return wgpuTextureGetFormat(Get());
        }
};

class WebGPURenderPassColorAttachment : public WebGPUObjectBase<WebGPURenderPassColorAttachment, WGPURenderPassColorAttachment> {
    public:
        WebGPURenderPassColorAttachment() {
            SetDepthSlice(WGPU_DEPTH_SLICE_UNDEFINED); // Required for emscripten
        }

        static WebGPURenderPassColorAttachment Obtain() {
            WebGPURenderPassColorAttachment renderPassColorAttachment;
            return renderPassColorAttachment;
        }

        void SetResolveTarget(WebGPUTextureView* textureView) {
            Get().resolveTarget = textureView == NULL ? NULL : textureView->Get();
        }

        void SetView(WebGPUTextureView* textureView) {
            Get().view = textureView == NULL ? NULL : textureView->Get();
        }

        void SetLoadOp(WGPULoadOp loadOp) {
            Get().loadOp = loadOp;
        }

        void SetStoreOp(WGPUStoreOp storeOp) {
            Get().storeOp = storeOp;
        }

        void SetDepthSlice(int depthSlice) {
            Get().depthSlice = depthSlice;
        }

        WebGPUColor GetClearValue() {
            WebGPUColor temp(&Get().clearValue);
            return temp;
        }
};

class WebGPUSurfaceTexture : public WebGPUObjectBase<WebGPUSurfaceTexture, WGPUSurfaceTexture> {
    public:
        static WebGPUSurfaceTexture Obtain() {
            WebGPUSurfaceTexture obj;
            return obj;
        }

        void GetTexture(WebGPUTexture* tex) {
            tex->Set(Get().texture);
        }

        WGPUSurfaceGetCurrentTextureStatus GetStatus() {
            return Get().status;
        }
};

class WebGPUSurface : public WebGPUObjectBase<WebGPUSurface, WGPUSurface> {
    protected:

        void AddRefInternal() {
            wgpuSurfaceAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuSurfaceRelease(Get());
        }

    public:

        void Unconfigure() {
            wgpuSurfaceUnconfigure(Get());
        }

        void Configure(WebGPUSurfaceConfiguration* config) {
            wgpuSurfaceConfigure(Get(), reinterpret_cast<WGPUSurfaceConfiguration const * >(config));
        }

        void GetCapabilities(WebGPUAdapter* adapter, WebGPUSurfaceCapabilities* surfaceCapabilities) {
            wgpuSurfaceGetCapabilities(Get(), adapter->Get(), &(surfaceCapabilities->Get()));
        }

        void GetCurrentTexture(WebGPUSurfaceTexture* tex) {
            wgpuSurfaceGetCurrentTexture(Get(), &tex->Get());
        }

        void Present() {
            wgpuSurfacePresent(Get());
        }
};

class WebGPUInstance : public WebGPUObjectBase<WebGPUInstance, WGPUInstance> {
    private:
        void AddRefInternal() {
            wgpuInstanceAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuInstanceRelease(Get());
        }

    public:

        bool IsValid() {
            return Get() ? true : false;
        }

        void RequestAdapter(WebGPURequestAdapterOptions* options, WGPUCallbackMode mode, RequestAdapterCallback* callback) {
            WGPURequestAdapterCallbackInfo callbackInfo = {};
            callbackInfo.mode = mode;
            callbackInfo.callback = [](WGPURequestAdapterStatus status, WGPUAdapter ad, WGPUStringView message, void* callback_param, void*) {
                RequestAdapterCallback* cback = reinterpret_cast<RequestAdapterCallback*>(callback_param);
                WebGPUAdapter* adapter = new WebGPUAdapter();
                adapter->Set(ad);
                cback->OnCallback(status, adapter);
            };
            callbackInfo.userdata1 = reinterpret_cast<void*>(callback);
            callbackInfo.userdata2 = NULL;
            auto result = wgpuInstanceRequestAdapter(Get(), &(options->Get()), callbackInfo);
        }

        WebGPUSurface* CreateWebSurface(const char* canvas) {
            #ifdef __EMSCRIPTEN__
                WGPUEmscriptenSurfaceSourceCanvasHTMLSelector canvasDesc {};
                WebGPUStringView stringView(canvas);
                canvasDesc.chain.sType = WGPUSType_EmscriptenSurfaceSourceCanvasHTMLSelector;
                canvasDesc.selector = stringView.Get();
                WGPUSurfaceDescriptor surfDesc{};
                surfDesc.nextInChain = (WGPUChainedStruct*)&canvasDesc;
                WebGPUSurface* surface = new WebGPUSurface();
                surface->Set(wgpuInstanceCreateSurface(Get(), &surfDesc));
            #else
                WebGPUSurface* surface = NULL;
            #endif
            return surface;
        }

        WebGPUSurface* CreateWindowsSurface(void * hwnd) {
            WebGPUSurface* surface = NULL;
            #if _WIN32
                surface = new WebGPUSurface();
                HINSTANCE hinstance = GetModuleHandle(NULL);
                WGPUSurfaceSourceWindowsHWND fromWindowsHWND;
                fromWindowsHWND.chain.next = NULL;
                fromWindowsHWND.chain.sType = WGPUSType_SurfaceSourceWindowsHWND;
                fromWindowsHWND.hinstance = hinstance;
                fromWindowsHWND.hwnd = hwnd;
                WGPUSurfaceDescriptor surfaceDescriptor{};
                surfaceDescriptor.nextInChain = &fromWindowsHWND.chain;
                surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
            #endif
            return surface;
        }

        WebGPUSurface* CreateLinuxSurface(bool isWayland, void * windowOrSurface, void* display) {
            WebGPUSurface* surface = NULL;
            #if __linux__
                surface = new WebGPUSurface();
                if(isWayland) {
                    WGPUSurfaceSourceWaylandSurface fromLinux;
                    fromLinux.chain.next = NULL;
                    fromLinux.chain.sType = WGPUSType_SurfaceSourceWaylandSurface;
                    fromLinux.display = display;
                    fromLinux.surface = windowOrSurface;
                    WGPUSurfaceDescriptor surfaceDescriptor{};
                    surfaceDescriptor.nextInChain = &fromLinux.chain;
                    surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
                }
                else {
                    WGPUSurfaceSourceXlibWindow fromLinux;
                    fromLinux.chain.next = NULL;
                    fromLinux.chain.sType = WGPUSType_SurfaceSourceXlibWindow;
                    fromLinux.display = display;
                    fromLinux.window = (uint64_t)windowOrSurface;
                    WGPUSurfaceDescriptor surfaceDescriptor{};
                    surfaceDescriptor.nextInChain = &fromLinux.chain;
                    surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
                }
            #endif
            return surface;
        }

        WebGPUSurface* CreateMacSurface(void * metalLayer) {
            WebGPUSurface* surface = NULL;
            #if TARGET_OS_MAC
                surface = new WebGPUSurface();
                WGPUSurfaceSourceMetalLayer fromMac;
                fromMac.chain.next = NULL;
                fromMac.chain.sType = WGPUSType_SurfaceSourceWaylandSurface;
                fromMac.layer = metalLayer;
                WGPUSurfaceDescriptor surfaceDescriptor{};
                surfaceDescriptor.nextInChain = &fromMac.chain;
                surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
            #endif
            return surface;
        }

        WebGPUSurface* CreateAndroidSurface(WGPUAndroidWindow* window) {
            WebGPUSurface* surface = NULL;
            #if __ANDROID__
                void* androidWindow = window->GetWindow();
                surface = new WebGPUSurface();
                WGPUSurfaceSourceAndroidNativeWindow androidSurfaceWindow;
                androidSurfaceWindow.chain.next = NULL;
                androidSurfaceWindow.chain.sType = WGPUSType_SurfaceSourceAndroidNativeWindow;
                androidSurfaceWindow.window = androidWindow;
                WGPUSurfaceDescriptor surfaceDescriptor{};
                surfaceDescriptor.nextInChain = &androidSurfaceWindow.chain;
                surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
            #endif
            return surface;
        }

        void ProcessEvents() {
            wgpuInstanceProcessEvents(Get());
        }
};

class WGPU {
    private:

    public:
        static WGPUPlatformType GetPlatformType() {
            #if __EMSCRIPTEN__
                return WGPUPlatformType::WGPU_Web;
            #elif __ANDROID__
                 return WGPUPlatformType::WGPU_Android;
            #elif TARGET_OS_IPHONE
                return WGPUPlatformType::WGPU_iOS;
            #elif _WIN32
                return WGPUPlatformType::WGPU_Windows;
            #elif __linux__
                return WGPUPlatformType::WGPU_Linux;
            #elif TARGET_OS_MAC
                return WGPUPlatformType::WGPU_Mac;
            #else
                return WGPUPlatformType::WGPU_Unknown;
            #endif
        }

        static WebGPUInstance CreateInstance() {
            WebGPUInstance instance;
            instance.Set(wgpuCreateInstance(NULL));
            return instance;
        }
};