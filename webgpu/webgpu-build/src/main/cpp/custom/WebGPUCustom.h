#pragma once

#include "IDLHelper.h"
#include "webgpu/webgpu.h"
#include <iostream>

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
class JRenderPassColorAttachment;
class JConstantEntry;
class JVertexBufferLayout;

using JVectorColorTargetState = std::vector<JColorTargetState>;
using JVectorRequiredFeatures = std::vector<WGPUFeatureName>;
using JVectorConstantEntry = std::vector<JConstantEntry>;
using JVectorVertexBufferLayout = std::vector<JVertexBufferLayout>;
using JVectorTextureFormat = std::vector<WGPUTextureFormat>;

#ifdef __EMSCRIPTEN__

using WGPURenderPassTimestampWrites = WGPUPassTimestampWrites; // dawn version TODO remove when both are the same

#else

using WGPURenderPassTimestampWrites = WGPURenderPassTimestampWrites;  // wgpu-native version TODO remove when both are the same

#endif //__EMSCRIPTEN__

enum JPlatformType : int {
    WGPU_Unknown = 0,
    WGPU_Windows,
    WGPU_Mac,
    WGPU_Linux,
    WGPU_iOS,
    WGPU_Android,
    WGPU_Web
};

class JAndroidWindow {
    public:
        #ifdef __ANDROID__
            ANativeWindow* g_window = nullptr;
        #endif

        ~JAndroidWindow() {
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

template<typename Derived, typename CType>
class JObjectBase {
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

class JCommandBuffer : public JObjectBase<JCommandBuffer, WGPUCommandBuffer> {
    protected:

        void AddRefInternal() {
            wgpuCommandBufferAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuCommandBufferRelease(Get());
        }

    public:

        static JCommandBuffer Obtain() {
            JCommandBuffer obj;
            return obj;
        }

};

class JQueue : public JObjectBase<JQueue, WGPUQueue> {
    protected:

        void AddRefInternal() {
            wgpuQueueAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuQueueRelease(Get());
        }

    public:

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            wgpuQueueSetLabel(Get(), stringView);
        }

        void Submit(int commandCount, JCommandBuffer* commandBuffer) {
            wgpuQueueSubmit(Get(), commandCount, &(commandBuffer->Get()));
        }
};

class JChainedStruct : public JObjectBase<JChainedStruct, WGPUChainedStruct*> {
    private:

    public:
        void SetNext(JChainedStruct* value) {
            Get()->next = value->Get();
        }

        void SetSType(WGPUSType type) {
            Get()->sType = type;
        }
};

class JStringView : public JObjectBase<JStringView, WGPUStringView> {
    public:

        JStringView() {
        }

        JStringView(WGPUStringView stringView) {
            Set(stringView);
        }
        JStringView(const char* value) {
            Get().data = strdup(value);
            Get().length = strlen(value);
        }

        const std::string GetString() {
            return std::string(Get().data, Get().length);
        }
};

class JLimits : public JObjectBase<JLimits, WGPULimits> {
    public: // TODO wgpu-native limits is different from emscripten dawn limits

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

class JQueueDescriptor : public JObjectBase<JQueueDescriptor, WGPUQueueDescriptor*> {
        public:

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            Get()->label = stringView;
        }
};

class JDeviceDescriptor : public JObjectBase<JDeviceDescriptor, WGPUDeviceDescriptor> {
    public:

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            Get().label = stringView;
        }

        void SetRequiredLimits(JLimits* limits) {
            Get().requiredLimits = &(limits->Get());
        }

        void SetRequiredFeatures(JVectorRequiredFeatures* features) {
            if(features != NULL) {
                Get().requiredFeatureCount = features->size();
                Get().requiredFeatures = features->data();
            }
            else {
                Get().requiredFeatureCount = 0;
                Get().requiredFeatures = NULL;
            }
        }

        JQueueDescriptor GetDefaultQueue() {
            JQueueDescriptor temp;
            temp.Set(&Get().defaultQueue);
            return temp;
        }
};

class JRequestAdapterOptions : public JObjectBase<JRequestAdapterOptions, WGPURequestAdapterOptions> {
    public:
};

class JBindGroup : public JObjectBase<JBindGroup, WGPUBindGroup> {
    private:

    public:
};

class JRenderBundle : public JObjectBase<JRenderBundle, WGPURenderBundle> {
    private:

    public:

};

class JAdapterInfo : public JObjectBase<JAdapterInfo, WGPUAdapterInfo> {
    public:

        std::string GetVendor() {
            JStringView stringView(Get().vendor);
            return stringView.GetString();
        }

        std::string GetArchitecture() {
            JStringView stringView(Get().architecture);
            return stringView.GetString();
        }

        std::string GetDevice() {
            JStringView stringView(Get().device);
            return stringView.GetString();
        }

        std::string GetDescription() {
            JStringView stringView(Get().description);
            return stringView.GetString();
        }

        WGPUBackendType GetBackendType() {
            return Get().backendType;
        }

        WGPUAdapterType GetAdapterType() {
            return Get().adapterType;
        }
};

class JShaderModule : public JObjectBase<JShaderModule, WGPUShaderModule> {
    protected:

        void AddRefInternal() {
            wgpuShaderModuleAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuShaderModuleRelease(Get());
        }

    public:

        static JShaderModule Obtain() {
            JShaderModule obj;
            return obj;
        }
};

class JConstantEntry : public JObjectBase<JConstantEntry, WGPUConstantEntry> {
    public:
};

class JVertexBufferLayout : public JObjectBase<JVertexBufferLayout, WGPUVertexBufferLayout> {
    public:
};


class JVertexState : public JObjectBase<JVertexState, WGPUVertexState*> {
    public:

        void SetNextInChain(JChainedStruct* chainedStruct) {
            Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetModule(JShaderModule* shaderModule) {
            Get()->module = shaderModule != NULL ? shaderModule->Get() : NULL;
        }

        void SetEntryPoint(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            Get()->entryPoint = stringView;
        }

        void SetConstants(JVectorConstantEntry* values) {
            if(values != NULL) {
                Get()->constantCount = values->size();
                Get()->constants = reinterpret_cast<const WGPUConstantEntry*>(values->data());
            }
            else {
                Get()->constantCount = 0;
                Get()->constants = NULL;
            }
        }

        void SetBuffers(JVectorVertexBufferLayout* values) {
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

class JShaderSourceWGSL : public JObjectBase<JShaderSourceWGSL, WGPUShaderSourceWGSL> {
    public:

        void SetCode(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            Get().code = stringView;
        }

        void SetNext(JChainedStruct* value) {
            Get().chain.next = value != NULL ? value->Get() : NULL;
        }

        void SetSType(WGPUSType type) {
            Get().chain.sType = type;
        }

        JChainedStruct GetChain() {
            WGPUChainedStruct* wgpuChain = &Get().chain;
            JChainedStruct chain;
            chain.Set(wgpuChain);
            return chain;
        }
};

class JShaderModuleDescriptor : public JObjectBase<JShaderModuleDescriptor, WGPUShaderModuleDescriptor> {
    public:

        void SetNextInChain(JChainedStruct* chainedStruct) {
            Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            Get().label = stringView;
        }
};

class JBlendComponent : public JObjectBase<JBlendComponent, WGPUBlendComponent*> {
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

class JBlendState : public JObjectBase<JBlendState, WGPUBlendState> {
    public:

        JBlendComponent GetColor() {
            JBlendComponent temp;
            temp.Set(&Get().color);
            return temp;
        }

        JBlendComponent GetAlpha() {
            JBlendComponent temp;
            temp.Set(&Get().alpha);
            return temp;
        }
};

class JColorTargetState : public JObjectBase<JColorTargetState, WGPUColorTargetState> {
    public:

        void SetNextInChain(JChainedStruct* chainedStruct) {
            Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetFormat(WGPUTextureFormat format) {
            Get().format = format;
        }

        void SetBlend(JBlendState* blendState) {
            Get().blend = reinterpret_cast<const WGPUBlendState*>(blendState);
        }

        void SetWriteMask(WGPUColorWriteMask writeMask) {
            Get().writeMask = writeMask;
        }
};

class JFragmentState : public JObjectBase<JFragmentState, WGPUFragmentState> {
    public:

        void SetNextInChain(JChainedStruct* chainedStruct) {
            Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetEntryPoint(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            Get().entryPoint = stringView;
        }

        void SetTargets(JVectorColorTargetState* values) {
            if(values != NULL) {
                Get().targetCount = values->size();
                Get().targets = reinterpret_cast<const WGPUColorTargetState*>(values->data());
            }
            else {
                Get().targetCount = 0;
                Get().targets = NULL;
            }
        }

        void SetModule(JShaderModule* shaderModule) {
            Get().module = shaderModule != NULL ? shaderModule->Get() : NULL;
        }

        void SetConstants(JVectorConstantEntry* values) {
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

class JPrimitiveState : public JObjectBase<JPrimitiveState, WGPUPrimitiveState*> {
    public:

        void SetNextInChain(JChainedStruct* chainedStruct) {
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

class JDepthStencilState : public JObjectBase<JDepthStencilState, WGPUDepthStencilState> {
    public:
        void SetNextInChain(JChainedStruct* chainedStruct) {
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

class JMultisampleState : public JObjectBase<JMultisampleState, WGPUMultisampleState*> {
    public:
        void SetNextInChain(JChainedStruct* chainedStruct) {
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

class JPipelineLayout : public JObjectBase<JPipelineLayout, WGPUPipelineLayout> {
    public:
};

class JRenderPipelineDescriptor : public JObjectBase<JRenderPipelineDescriptor, WGPURenderPipelineDescriptor> {
    public:
        void SetNextInChain(JChainedStruct* chainedStruct) {
            Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            Get().label = stringView;
        }

        JVertexState GetVertex() {
            JVertexState temp;
            temp.Set(&Get().vertex);
            return temp;
        }

        JPrimitiveState GetPrimitive() {
            JPrimitiveState temp;
            temp.Set(&Get().primitive);
            return temp;
        }

        void SetFragment(JFragmentState* fragment) {
            Get().fragment = &fragment->Get();
        }

        void SetDepthStencil(JDepthStencilState* depthStencilState) {
            Get().depthStencil = depthStencilState != NULL ? &depthStencilState->Get() : NULL;
        }

        JMultisampleState GetMultisample() {
            JMultisampleState temp;
            temp.Set(&Get().multisample);
            return temp;
        }

        void SetLayout(JPipelineLayout* pipelineLayout) {
            Get().layout = pipelineLayout != NULL ? pipelineLayout->Get() : NULL;
        }
};

class JRenderPipeline : public JObjectBase<JRenderPipeline, WGPURenderPipeline> {

    protected:
        void AddRefInternal() {
            wgpuRenderPipelineAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuRenderPipelineRelease(Get());
        }

    public:
        static JRenderPipeline Obtain() {
            JRenderPipeline obj;
            return obj;
        }
};

class JColor : public JObjectBase<JColor, WGPUColor*> {
    private:
        bool deleteObject;

    public:
        JColor() {
            deleteObject = true;
            Set(new WGPUColor());
        }

        JColor(WGPUColor* color) {
            deleteObject = false;
            Set(color);
        }

        ~JColor() {
            if(deleteObject) {
                delete Get();
            }
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

class JSupportedFeatures : public JObjectBase<JSupportedFeatures, WGPUSupportedFeatures> {
    public:

        int GetFeatureCount() {
            return Get().featureCount;
        }

        WGPUFeatureName GetFeatureAt(long index) {
            return Get().features[index];
        }
};

class JBuffer : public JObjectBase<JBuffer, WGPUBuffer> {
    public:
};

class JRenderPassEncoder : public JObjectBase<JRenderPassEncoder, WGPURenderPassEncoder> {
    protected:

        void AddRefInternal() {
            wgpuRenderPassEncoderAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuRenderPassEncoderRelease(Get());
        }

    public:

        static JRenderPassEncoder Obtain() {
            JRenderPassEncoder obj;
            return obj;
        }

        void End() {
            wgpuRenderPassEncoderEnd(Get());
        }

        void SetPipeline(JRenderPipeline* renderPipeline) {
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

        void DrawIndexedIndirect(JBuffer* indirectBuffer, int indirectOffset) {
            wgpuRenderPassEncoderDrawIndexedIndirect(Get(), indirectBuffer->Get(), indirectOffset);
        }

        void DrawIndirect(JBuffer* indirectBuffer, int indirectOffset) {
            wgpuRenderPassEncoderDrawIndirect(Get(), indirectBuffer->Get(), indirectOffset);
        }

        void EndOcclusionQuery() {
            wgpuRenderPassEncoderEndOcclusionQuery(Get());
        }

        void ExecuteBundles(int bundleCount, JRenderBundle* bundles) {
            // TODO change to array class
            wgpuRenderPassEncoderExecuteBundles(Get(), bundleCount,  reinterpret_cast<WGPURenderBundle const * >(bundles));
        }

        void ExecuteBundles(const char* label) {
            JStringView stringView(label);
            wgpuRenderPassEncoderInsertDebugMarker(Get(), stringView.Get());
        }

        void PopDebugGroup() {
            wgpuRenderPassEncoderPopDebugGroup(Get());
        }

        void PushDebugGroup(const char* label) {
            JStringView stringView(label);
            wgpuRenderPassEncoderPushDebugGroup(Get(), stringView.Get());
        }

        void SetBindGrou(int groupIndex, JBindGroup* group, int dynamicOffsetCount, const uint32_t* dynamicOffsets) {
            // TODO change to array class
            wgpuRenderPassEncoderSetBindGroup(Get(), groupIndex, *reinterpret_cast<WGPUBindGroup *>(group), dynamicOffsetCount, dynamicOffsets);
        }

        void SetBlendConstant(JColor* color) {
            // TODO need to test if this is working
            wgpuRenderPassEncoderSetBlendConstant(Get(), reinterpret_cast<WGPUColor *>(color));
        }

        void SetIndexBuffer(JBuffer* buffer, WGPUIndexFormat format, int offset, int size) {
            wgpuRenderPassEncoderSetIndexBuffer(Get(), buffer->Get(), format, offset, size);
        }

        void SetLabel(const char* label) {
            JStringView stringView(label);
            wgpuRenderPassEncoderSetLabel(Get(), stringView.Get());
        }

        void SetScissorRect(int x, int y, int width, int height) {
            wgpuRenderPassEncoderSetScissorRect(Get(), x, y, width, height);
        }

        void SetStencilReference(int reference) {
            wgpuRenderPassEncoderSetStencilReference(Get(), reference);
        }

        void SetVertexBuffer(int slot, JBuffer* buffer, int offset, int size) {
            wgpuRenderPassEncoderSetVertexBuffer(Get(), slot, buffer->Get(), offset, size);
        }

        void SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth) {
            wgpuRenderPassEncoderSetViewport(Get(), x, y, width, height, minDepth, maxDepth);
        }

        void AddRef() {
            wgpuRenderPassEncoderAddRef(Get());
        }
};

class JComputePassEncoder : public JObjectBase<JComputePassEncoder, WGPUComputePassEncoder> {
    protected:

        void AddRefInternal() {
            wgpuComputePassEncoderAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuComputePassEncoderRelease(Get());
        }

    public:

        static JComputePassEncoder Obtain() {
            JComputePassEncoder obj;
            return obj;
        }
};

class JRenderPassDepthStencilAttachment : public JObjectBase<JRenderPassDepthStencilAttachment, WGPURenderPassDepthStencilAttachment> {
    public:

};

// Opaque pointer
class JQuerySet : public JObjectBase<JQuerySet, WGPUQuerySet> {
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
            JStringView stringView(label);
            wgpuQuerySetSetLabel(Get(), stringView.Get());
        }
};

// TODO The class name differs from those in Dawn and wgpu-native.
class JRenderPassTimestampWrites : public JObjectBase<JRenderPassTimestampWrites, WGPURenderPassTimestampWrites> {
    public:

        void SetQuerySet(JQuerySet* value) {
            Get().querySet = value->Get();
        }

        void SetBeginningOfPassWriteIndex(int value) {
            Get().beginningOfPassWriteIndex = value;
        }

        void SetEndOfPassWriteIndex(int value) {
            Get().endOfPassWriteIndex = value;
        }
};

class JCommandEncoderDescriptor : public JObjectBase<JCommandEncoderDescriptor, WGPUCommandEncoderDescriptor> {
    public:

        static JCommandEncoderDescriptor Obtain() {
            JCommandEncoderDescriptor obj;
            return obj;
        }

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            Get().label = stringView;
        }
};

class JCommandBufferDescriptor : public JObjectBase<JCommandBufferDescriptor, WGPUCommandBufferDescriptor> {
    public:

        static JCommandBufferDescriptor Obtain() {
            JCommandBufferDescriptor obj;
            return obj;
        }

        void SetNextInChain(JChainedStruct* chainedStruct) {
            Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
        }

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            Get().label = stringView;
        }
};

class JRenderPassDescriptor : public JObjectBase<JRenderPassDescriptor, WGPURenderPassDescriptor> {
    public:

        static JRenderPassDescriptor Obtain() {
            JRenderPassDescriptor descriptor;
            return descriptor;
        }

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            Get().label = stringView;
        }

        void SetColorAttachmentCount(int size) {
            Get().colorAttachmentCount = size;
        }

        void SetColorAttachments(JRenderPassColorAttachment* colorAttachment) {
            // TODO Must be an array
            Get().colorAttachments = reinterpret_cast<WGPURenderPassColorAttachment const * >(colorAttachment);
        }

        void SetDepthStencilAttachment(JRenderPassDepthStencilAttachment* attachment) {
            Get().depthStencilAttachment = attachment == NULL ? NULL : &(attachment->Get());
        }

        void SetTimestampWrites(JRenderPassTimestampWrites* timestampWrites) {
            // TODO emscripten does not have this class
//            Get().timestampWrites = timestampWrites == NULL ? NULL : &(timestampWrites->timestampWrites);
        }
};

class JComputePassDescriptor : public JObjectBase<JComputePassDescriptor, WGPUComputePassDescriptor> {
    public:

        static JComputePassDescriptor Obtain() {
            JComputePassDescriptor descriptor;
            return descriptor;
        }

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            Get().label = stringView;
        }
};

class JCommandEncoder : public JObjectBase<JCommandEncoder, WGPUCommandEncoder> {
    protected:

        void AddRefInternal() {
            wgpuCommandEncoderAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuCommandEncoderRelease(Get());
        }

    public:

        static JCommandEncoder Obtain() {
            JCommandEncoder obj;
            return obj;
        }

        void BeginRenderPass(JRenderPassDescriptor* renderPassDescriptor, JRenderPassEncoder* encoder) {
            encoder->Set(wgpuCommandEncoderBeginRenderPass(Get(), &(renderPassDescriptor->Get())));
        }

        void BeginComputePass(JComputePassEncoder* encoder) {
            WGPUComputePassDescriptor computePassDescriptor;
            encoder->Set(wgpuCommandEncoderBeginComputePass(Get(), &computePassDescriptor));
        }

        void ClearBuffer(JBuffer* buffer, int offset, int size) {
            wgpuCommandEncoderClearBuffer(Get(), buffer->Get(), offset, size);
        }

        void Finish(JCommandBufferDescriptor* commandBufferDescriptor, JCommandBuffer* commandBuffer) {
            commandBuffer->Set(wgpuCommandEncoderFinish(Get(), &(commandBufferDescriptor->Get())));
        }
};

class JDevice : public JObjectBase<JDevice, WGPUDevice> {
    protected:

        void AddRefInternal() {
            wgpuDeviceAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuDeviceRelease(Get());
        }

    public:

//        bool GetAdapterInfo(JAdapterInfo* adapterInfo) {
//            #ifdef __EMSCRIPTEN__
//                WGPUStatus status = wgpuDeviceGetAdapterInfo(device, &(adapterInfo->adapterInfo));
//            #else
//                wgpuDeviceGetAdapterInfo(device); // Not implemented
//                WGPUStatus status = WGPUStatus_Error;
//            #endif
//            return status == WGPUStatus_Success;
//        }

        JQueue GetQueue() {
            JQueue temp;
            temp.Set(wgpuDeviceGetQueue(Get()));
            return temp;
        }

        void CreateRenderPipeline(JRenderPipelineDescriptor* pipelineDescriptor, JRenderPipeline* renderPipeline) {
            renderPipeline->Set(wgpuDeviceCreateRenderPipeline(Get(), reinterpret_cast<WGPURenderPipelineDescriptor const * >(pipelineDescriptor)));
        }

        void CreateShaderModule(JShaderModuleDescriptor* shaderModuleDescriptor, JShaderModule* shaderModule) {
            shaderModule->Set(wgpuDeviceCreateShaderModule(Get(), &shaderModuleDescriptor->Get()));
        }

        void GetFeatures(JSupportedFeatures* features) {
            wgpuDeviceGetFeatures(Get(), reinterpret_cast<WGPUSupportedFeatures * >(features));
        }

        void GetLimits(JLimits* limits) {
            wgpuDeviceGetLimits(Get(), reinterpret_cast<WGPULimits * >(&(limits->Get())));
        }

        void CreateCommandEncoder(JCommandEncoderDescriptor* encoderDescriptor, JCommandEncoder* encoder) {
            encoder->Set(wgpuDeviceCreateCommandEncoder(Get(), &(encoderDescriptor->Get())));
        }
};

class JSurfaceConfiguration : public JObjectBase<JSurfaceConfiguration, WGPUSurfaceConfiguration> {
    public:

        void SetNextInChain(JChainedStruct* chainedStruct) {
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

        void SetViewFormats(JVectorTextureFormat* values) {
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

        void SetDevice(JDevice* device) {
            Get().device = device != NULL ? device->Get() : NULL;
        }

        void SetPresentMode(WGPUPresentMode presentMode) {
            Get().presentMode = presentMode;
        }

        void SetAlphaMode(WGPUCompositeAlphaMode alphaMode) {
            Get().alphaMode = alphaMode;
        }
};

class JSurfaceCapabilities : public JObjectBase<JSurfaceCapabilities, WGPUSurfaceCapabilities> {
    public:

        WGPUTextureFormat GetFormats(int index) {
            return Get().formats[index];
        }
};

class JAdapter : public JObjectBase<JAdapter, WGPUAdapter> {
    protected:

        void AddRefInternal() {
            wgpuAdapterAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuAdapterRelease(Get());
        }

    public:

        void RequestDevice(JDeviceDescriptor* descriptor, WGPUCallbackMode mode, RequestDeviceCallback* callback, UncapturedErrorCallback* errorCallback) {
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
                JDevice* device = new JDevice();
                device->Set(dev);
                cback->OnCallback(status, device);
            };
            callbackInfo.userdata1 = reinterpret_cast<void*>(callback);
            callbackInfo.userdata2 = NULL;
            const WGPUDeviceDescriptor* desc = reinterpret_cast<WGPUDeviceDescriptor const*>(descriptor);
            auto result = wgpuAdapterRequestDevice(Get(), desc, callbackInfo);
        }

        bool GetInfo(JAdapterInfo* adapterInfo) {
            WGPUStatus status = wgpuAdapterGetInfo(Get(), &(adapterInfo->Get()));
            return status == WGPUStatus_Success;
        }

        bool HasFeature(WGPUFeatureName featureName) {
            return wgpuAdapterHasFeature(Get(), featureName);
        }
};

//struct JTextureViewDescriptor {
//    JChainedStruct const * nextInChain = NULL;
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

class JTextureViewDescriptor : public JObjectBase<JTextureViewDescriptor, WGPUTextureViewDescriptor> {
    public:

    static JTextureViewDescriptor Obtain() {
        JTextureViewDescriptor textureViewDescriptor;
        return textureViewDescriptor;
    }

    void SetLabel(const char* value) {
        WGPUStringView stringView = {};
        stringView.data = strdup(value);
        stringView.length = strlen(value);
        Get().label = stringView;
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

class JTextureView : public JObjectBase<JTextureView, WGPUTextureView> {
    protected:

        void AddRefInternal() {
            wgpuTextureViewAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuTextureViewRelease(Get());
        }

    public:

        static JTextureView Obtain() {
            JTextureView obj;
            return obj;
        }
};

class JTexture : public JObjectBase<JTexture, WGPUTexture> {
    protected:

        void AddRefInternal() {
            wgpuTextureAddRef(Get());
        }

        void ReleaseInternal() {
            wgpuTextureRelease(Get());
        }

    public:

        static JTexture Obtain() {
            JTexture obj;
            return obj;
        }

        void CreateView(JTextureViewDescriptor* textureViewDescriptor, JTextureView* textureView) {
            textureView->Set(wgpuTextureCreateView(Get(), &(textureViewDescriptor->Get())));
        }

        WGPUTextureFormat GetFormat() {
            return wgpuTextureGetFormat(Get());
        }
};

class JRenderPassColorAttachment : public JObjectBase<JRenderPassColorAttachment, WGPURenderPassColorAttachment> {
    public:
        JRenderPassColorAttachment() {
            SetDepthSlice(WGPU_DEPTH_SLICE_UNDEFINED); // Required for emscripten
        }

        static JRenderPassColorAttachment Obtain() {
            JRenderPassColorAttachment renderPassColorAttachment;
            return renderPassColorAttachment;
        }

        void SetResolveTarget(JTextureView* textureView) {
            Get().resolveTarget = textureView == NULL ? NULL : textureView->Get();
        }

        void SetView(JTextureView* textureView) {
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

        JColor GetClearValue() {
            JColor temp(&Get().clearValue);
            return temp;
        }
};

class JSurfaceTexture : public JObjectBase<JSurfaceTexture, WGPUSurfaceTexture> {
    public:
        static JSurfaceTexture Obtain() {
            JSurfaceTexture obj;
            return obj;
        }

        void GetTexture(JTexture* tex) {
            tex->Set(Get().texture);
        }

        WGPUSurfaceGetCurrentTextureStatus GetStatus() {
            return Get().status;
        }
};

class JSurface : public JObjectBase<JSurface, WGPUSurface> {
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

        void Configure(JSurfaceConfiguration* config) {
            wgpuSurfaceConfigure(Get(), reinterpret_cast<WGPUSurfaceConfiguration const * >(config));
        }

        void GetCapabilities(JAdapter* adapter, JSurfaceCapabilities* surfaceCapabilities) {
            wgpuSurfaceGetCapabilities(Get(), adapter->Get(), &(surfaceCapabilities->Get()));
        }

        void GetCurrentTexture(JSurfaceTexture* tex) {
            wgpuSurfaceGetCurrentTexture(Get(), &tex->Get());
        }

        void Present() {
            wgpuSurfacePresent(Get());
        }
};

class JInstance : public JObjectBase<JInstance, WGPUInstance> {
    private:

    public:
        bool isValid = false;

        JInstance() {
            WGPUInstance instance = wgpuCreateInstance(NULL);
            if (!instance) {
                isValid = false;
            }
            else {
                Set(instance);
                isValid = true;
            }
        }

        bool IsValid() {
            return isValid;
        }

        void Release() {
            if(isValid) {
                isValid = false;
                wgpuInstanceRelease(Get());
            }
        }

        void RequestAdapter(JRequestAdapterOptions* options, WGPUCallbackMode mode, RequestAdapterCallback* callback) {
            WGPURequestAdapterCallbackInfo callbackInfo = {};
            callbackInfo.mode = mode;
            callbackInfo.callback = [](WGPURequestAdapterStatus status, WGPUAdapter ad, WGPUStringView message, void* callback_param, void*) {
                RequestAdapterCallback* cback = reinterpret_cast<RequestAdapterCallback*>(callback_param);
                JAdapter* adapter = new JAdapter();
                adapter->Set(ad);
                cback->OnCallback(status, adapter);
            };
            callbackInfo.userdata1 = reinterpret_cast<void*>(callback);
            callbackInfo.userdata2 = NULL;
            auto result = wgpuInstanceRequestAdapter(Get(), &(options->Get()), callbackInfo);
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
                surface->Set(wgpuInstanceCreateSurface(Get(), &surfDesc));
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
                surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
            #endif
            return surface;
        }

        JSurface* CreateAndroidSurface(JAndroidWindow* window) {
            JSurface* surface = NULL;
            #if __ANDROID__
                void* androidWindow = window->GetWindow();
                surface = new JSurface();
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

class JWebGPU {
    private:

    public:
        static JPlatformType GetPlatformType() {
            #if __EMSCRIPTEN__
                return JPlatformType::WGPU_Web;
            #elif __ANDROID__
                 return JPlatformType::WGPU_Android;
            #elif TARGET_OS_IPHONE
                return JPlatformType::WGPU_iOS;
            #elif _WIN32
                return JPlatformType::WGPU_Windows;
            #elif __linux__
                return JPlatformType::WGPU_Linux;
            #elif TARGET_OS_MAC
                return JPlatformType::WGPU_Mac;
            #else
                return JPlatformType::WGPU_Unknown;
            #endif
        }

        static JRenderPipeline* CreateRenderPipeline(JDevice* device, const char* shaderSource, WGPUTextureFormat surfaceFormat) {

            JShaderModuleDescriptor shaderDesc;
            shaderDesc.SetLabel("triangle shader");

            JShaderSourceWGSL shaderCodeDesc;
            shaderCodeDesc.SetNext(NULL);
            shaderCodeDesc.SetSType(WGPUSType_ShaderSourceWGSL);
            shaderCodeDesc.SetCode(shaderSource);

            JChainedStruct chainedStruct = shaderCodeDesc.GetChain();
            shaderDesc.SetNextInChain(&chainedStruct);

            JShaderModule* shaderModule = new JShaderModule();
            device->CreateShaderModule(&shaderDesc, shaderModule);

            JRenderPipelineDescriptor pipelineDesc;
            pipelineDesc.SetLabel("my pipeline");

            pipelineDesc.GetVertex().SetBuffers(NULL);  // no vertex buffer, because we define it in the shader

            pipelineDesc.GetVertex().SetModule(shaderModule);
            pipelineDesc.GetVertex().SetEntryPoint("vs_main");
            pipelineDesc.GetVertex().SetConstants(NULL);

            pipelineDesc.GetPrimitive().SetTopology(WGPUPrimitiveTopology_TriangleList);
            pipelineDesc.GetPrimitive().SetStripIndexFormat(WGPUIndexFormat_Undefined);
            pipelineDesc.GetPrimitive().SetFrontFace(WGPUFrontFace_CCW);
            pipelineDesc.GetPrimitive().SetCullMode(WGPUCullMode_None);

            JFragmentState fragmentState;
            fragmentState.SetNextInChain(NULL);
            fragmentState.SetModule(shaderModule);
            fragmentState.SetEntryPoint("fs_main");
            fragmentState.SetConstants(NULL);

            // blending
            JBlendState blendState;
            blendState.GetColor().SetSrcFactor(WGPUBlendFactor_SrcAlpha);
            blendState.GetColor().SetDstFactor(WGPUBlendFactor_OneMinusSrcAlpha);
            blendState.GetColor().SetOperation(WGPUBlendOperation_Add);
            blendState.GetAlpha().SetSrcFactor(WGPUBlendFactor_One);
            blendState.GetAlpha().SetDstFactor(WGPUBlendFactor_Zero);
            blendState.GetAlpha().SetOperation(WGPUBlendOperation_Add);

            JVectorColorTargetState targetStateVector;
            JColorTargetState colorTarget;
            colorTarget.SetFormat(surfaceFormat); // match output surface
            colorTarget.SetBlend(&blendState);
            colorTarget.SetWriteMask(WGPUColorWriteMask_All);
            targetStateVector.push_back(colorTarget);

            fragmentState.SetTargets(&targetStateVector);

            pipelineDesc.SetFragment(&fragmentState);

            pipelineDesc.SetDepthStencil(NULL); // no depth or stencil buffer

            pipelineDesc.GetMultisample().SetCount(1);
            pipelineDesc.GetMultisample().SetMask(-1);
            pipelineDesc.GetMultisample().SetAlphaToCoverageEnabled(0);

            pipelineDesc.SetLayout(NULL);

            JRenderPipeline* renderPipeline = new JRenderPipeline();

            const WGPURenderPipelineDescriptor* desc = reinterpret_cast<WGPURenderPipelineDescriptor const * >(&pipelineDesc);
            renderPipeline->Set(wgpuDeviceCreateRenderPipeline(device->Get(), desc));
            return renderPipeline;
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

