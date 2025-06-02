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
class JRenderPassColorAttachment;

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

class JCommandBuffer {
    private:

    public:
        WGPUCommandBuffer commandBuffer;

        static JCommandBuffer Obtain() {
            JCommandBuffer obj;
            return obj;
        }

        void Release() {
            wgpuCommandBufferRelease(commandBuffer);
        }
};

class JQueue {
    private:

    public:
        WGPUQueue queue;

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            wgpuQueueSetLabel(queue, stringView);
        }

        void Submit(int commandCount, JCommandBuffer* commandBuffer) {
            wgpuQueueSubmit(queue, commandCount, &(commandBuffer->commandBuffer));
        }

        void Release() {
            wgpuQueueRelease(queue);
        }
};

struct JChainedStruct {
    JChainedStruct const * nextInChain = nullptr;
    WGPUSType sType = (WGPUSType)0;

    void SetNext(JChainedStruct* value) {
        nextInChain = value;
    }

    void SetSType(WGPUSType type) {
        sType = type;
    }
};

// ChainedStruct
static_assert(sizeof(JChainedStruct) == sizeof(WGPUChainedStruct),
    "sizeof mismatch for JChainedStruct");
static_assert(alignof(JChainedStruct) == alignof(WGPUChainedStruct),
    "alignof mismatch for JChainedStruct");
static_assert(offsetof(JChainedStruct, nextInChain) == offsetof(WGPUChainedStruct, next),
    "offsetof mismatch for JChainedStruct::nextInChain");


class JStringView {
    private:

    public:
        WGPUStringView stringView;

        JStringView() {}
        JStringView(WGPUStringView stringView) {
            this->stringView = stringView;
        }

        const std::string GetString() {
            return std::string(stringView.data, stringView.length);
        }
};

class JLimits {
// TODO wgpu-native limits is different from emscripten dawn limits so it cannot align with C header
    private:

    public:
        WGPULimits limits;

        void SetMaxTextureDimension1D(int value) {
            limits.maxTextureDimension1D = value;
        }
        int GetMaxTextureDimension1D() const {
            return static_cast<int>(limits.maxTextureDimension1D);
        }

        void SetMaxTextureDimension2D(int value) {
            limits.maxTextureDimension2D = value;
        }
        int GetMaxTextureDimension2D() const {
            return static_cast<int>(limits.maxTextureDimension2D);
        }

        void SetMaxTextureDimension3D(int value) {
            limits.maxTextureDimension3D = value;
        }
        int GetMaxTextureDimension3D() const {
            return static_cast<int>(limits.maxTextureDimension3D);
        }

        void SetMaxTextureArrayLayers(int value) {
            limits.maxTextureArrayLayers = value;
        }
        int GetMaxTextureArrayLayers() const {
            return static_cast<int>(limits.maxTextureArrayLayers);
        }

        void SetMaxBindGroups(int value) {
            limits.maxBindGroups = value;
        }
        int GetMaxBindGroups() const {
            return static_cast<int>(limits.maxBindGroups);
        }

        void SetMaxBindGroupsPlusVertexBuffers(int value) {
            limits.maxBindGroupsPlusVertexBuffers = value;
        }
        int GetMaxBindGroupsPlusVertexBuffers() const {
            return static_cast<int>(limits.maxBindGroupsPlusVertexBuffers);
        }

        void SetMaxBindingsPerBindGroup(int value) {
            limits.maxBindingsPerBindGroup = value;
        }
        int GetMaxBindingsPerBindGroup() const {
            return static_cast<int>(limits.maxBindingsPerBindGroup);
        }

        void SetMaxDynamicUniformBuffersPerPipelineLayout(int value) {
            limits.maxDynamicUniformBuffersPerPipelineLayout = value;
        }
        int GetMaxDynamicUniformBuffersPerPipelineLayout() const {
            return static_cast<int>(limits.maxDynamicUniformBuffersPerPipelineLayout);
        }

        void SetMaxDynamicStorageBuffersPerPipelineLayout(int value) {
            limits.maxDynamicStorageBuffersPerPipelineLayout = value;
        }
        int GetMaxDynamicStorageBuffersPerPipelineLayout() const {
            return static_cast<int>(limits.maxDynamicStorageBuffersPerPipelineLayout);
        }

        void SetMaxSampledTexturesPerShaderStage(int value) {
            limits.maxSampledTexturesPerShaderStage = value;
        }
        int GetMaxSampledTexturesPerShaderStage() const {
            return static_cast<int>(limits.maxSampledTexturesPerShaderStage);
        }

        void SetMaxSamplersPerShaderStage(int value) {
            limits.maxSamplersPerShaderStage = value;
        }
        int GetMaxSamplersPerShaderStage() const {
            return static_cast<int>(limits.maxSamplersPerShaderStage);
        }

        void SetMaxStorageBuffersPerShaderStage(int value) {
            limits.maxStorageBuffersPerShaderStage = value;
        }
        int GetMaxStorageBuffersPerShaderStage() const {
            return static_cast<int>(limits.maxStorageBuffersPerShaderStage);
        }

        void SetMaxStorageTexturesPerShaderStage(int value) {
            limits.maxStorageTexturesPerShaderStage = value;
        }
        int GetMaxStorageTexturesPerShaderStage() const {
            return static_cast<int>(limits.maxStorageTexturesPerShaderStage);
        }

        void SetMaxUniformBuffersPerShaderStage(int value) {
            limits.maxUniformBuffersPerShaderStage = value;
        }
        int GetMaxUniformBuffersPerShaderStage() const {
            return static_cast<int>(limits.maxUniformBuffersPerShaderStage);
        }

        void SetMaxUniformBufferBindingSize(int value) {
            limits.maxUniformBufferBindingSize = value;
        }
        int GetMaxUniformBufferBindingSize() const {
            return static_cast<int>(limits.maxUniformBufferBindingSize);
        }

        void SetMaxStorageBufferBindingSize(int value) {
            limits.maxStorageBufferBindingSize = value;
        }
        int GetMaxStorageBufferBindingSize() const {
            return static_cast<int>(limits.maxStorageBufferBindingSize);
        }

        void SetMinUniformBufferOffsetAlignment(int value) {
            limits.minUniformBufferOffsetAlignment = value;
        }
        int GetMinUniformBufferOffsetAlignment() const {
            return static_cast<int>(limits.minUniformBufferOffsetAlignment);
        }

        void SetMinStorageBufferOffsetAlignment(int value) {
            limits.minStorageBufferOffsetAlignment = value;
        }
        int GetMinStorageBufferOffsetAlignment() const {
            return static_cast<int>(limits.minStorageBufferOffsetAlignment);
        }

        void SetMaxVertexBuffers(int value) {
            limits.maxVertexBuffers = value;
        }
        int GetMaxVertexBuffers() const {
            return static_cast<int>(limits.maxVertexBuffers);
        }

        void SetMaxBufferSize(int value) {
            limits.maxBufferSize = value;
        }
        int GetMaxBufferSize() const {
            return static_cast<int>(limits.maxBufferSize);
        }

        void SetMaxVertexAttributes(int value) {
            limits.maxVertexAttributes = value;
        }
        int GetMaxVertexAttributes() const {
            return static_cast<int>(limits.maxVertexAttributes);
        }

        void SetMaxVertexBufferArrayStride(int value) {
            limits.maxVertexBufferArrayStride = value;
        }
        int GetMaxVertexBufferArrayStride() const {
            return static_cast<int>(limits.maxVertexBufferArrayStride);
        }

        void SetMaxInterStageShaderVariables(int value) {
            limits.maxInterStageShaderVariables = value;
        }
        int GetMaxInterStageShaderVariables() const {
            return static_cast<int>(limits.maxInterStageShaderVariables);
        }

        void SetMaxColorAttachments(int value) {
            limits.maxColorAttachments = value;
        }
        int GetMaxColorAttachments() const {
            return static_cast<int>(limits.maxColorAttachments);
        }

        void SetMaxColorAttachmentBytesPerSample(int value) {
            limits.maxColorAttachmentBytesPerSample = value;
        }
        int GetMaxColorAttachmentBytesPerSample() const {
            return static_cast<int>(limits.maxColorAttachmentBytesPerSample);
        }

        void SetMaxComputeWorkgroupStorageSize(int value) {
            limits.maxComputeWorkgroupStorageSize = value;
        }
        int GetMaxComputeWorkgroupStorageSize() const {
            return static_cast<int>(limits.maxComputeWorkgroupStorageSize);
        }

        void SetMaxComputeInvocationsPerWorkgroup(int value) {
            limits.maxComputeInvocationsPerWorkgroup = value;
        }
        int GetMaxComputeInvocationsPerWorkgroup() const {
            return static_cast<int>(limits.maxComputeInvocationsPerWorkgroup);
        }

        void SetMaxComputeWorkgroupSizeX(int value) {
            limits.maxComputeWorkgroupSizeX = value;
        }
        int GetMaxComputeWorkgroupSizeX() const {
            return static_cast<int>(limits.maxComputeWorkgroupSizeX);
        }

        void SetMaxComputeWorkgroupSizeY(int value) {
            limits.maxComputeWorkgroupSizeY = value;
        }
        int GetMaxComputeWorkgroupSizeY() const {
            return static_cast<int>(limits.maxComputeWorkgroupSizeY);
        }

        void SetMaxComputeWorkgroupSizeZ(int value) {
            limits.maxComputeWorkgroupSizeZ = value;
        }
        int GetMaxComputeWorkgroupSizeZ() const {
            return static_cast<int>(limits.maxComputeWorkgroupSizeZ);
        }

        void SetMaxComputeWorkgroupsPerDimension(int value) {
            limits.maxComputeWorkgroupsPerDimension = value;
        }
        int GetMaxComputeWorkgroupsPerDimension() const {
            return static_cast<int>(limits.maxComputeWorkgroupsPerDimension);
        }
};

struct JQueueDescriptor {
    JChainedStruct const * nextInChain = NULL;
    WGPUStringView label;

    inline operator const WGPUQueueDescriptor&() const noexcept {
        return *reinterpret_cast<const WGPUQueueDescriptor*>(this);
    }

    void SetLabel(const char* value) {
        WGPUStringView stringView = {};
        stringView.data = strdup(value);
        stringView.length = strlen(value);
        label = stringView;
    }
};

struct JDeviceDescriptor {

    JChainedStruct const * nextInChain = NULL;
    WGPUStringView label;
    size_t requiredFeatureCount = 0;
    WGPUFeatureName const * requiredFeatures = NULL;
    WGPULimits const * requiredLimits = NULL;
    JQueueDescriptor defaultQueue;
    WGPUDeviceLostCallbackInfo deviceLostCallbackInfo;
    WGPUUncapturedErrorCallbackInfo uncapturedErrorCallbackInfo;

    inline operator const WGPUDeviceDescriptor&() const noexcept {
        return *reinterpret_cast<const WGPUDeviceDescriptor*>(this);
    }

    void SetLabel(const char* value) {
        WGPUStringView stringView = {};
        stringView.data = strdup(value);
        stringView.length = strlen(value);
        label = stringView;
    }

    void SetRequiredLimits(JLimits* limits) {
        requiredLimits = &(limits->limits);
    }

    void SetRequiredFeatureCount(int count) {
        requiredFeatureCount = count;
    }

    JQueueDescriptor& GetDefaultQueue() {
        return defaultQueue;
    }
};

class JRequestAdapterOptions { // TODO change to struct
    private:

    public:
        WGPURequestAdapterOptions options;
};

class JAdapterInfo { // TODO change to struct
    private:

    public:
        WGPUAdapterInfo adapterInfo;

        std::string GetVendor() {
            JStringView stringView(adapterInfo.vendor);
            return stringView.GetString();
        }

        std::string GetArchitecture() {
            JStringView stringView(adapterInfo.architecture);
            return stringView.GetString();
        }

        std::string GetDevice() {
            JStringView stringView(adapterInfo.device);
            return stringView.GetString();
        }

        std::string GetDescription() {
            JStringView stringView(adapterInfo.description);
            return stringView.GetString();
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

        static JShaderModule Obtain() {
            JShaderModule obj;
            return obj;
        }

        void Release() {
            wgpuShaderModuleRelease(shaderModule);
        }
};

struct JConstantEntry {
    WGPUChainedStruct const * nextInChain = NULL;
    WGPUStringView key;
    double value;

    inline operator const WGPUConstantEntry&() const noexcept {
        return *reinterpret_cast<const WGPUConstantEntry*>(this);
    }
};

struct JVertexBufferLayout {
    WGPUVertexStepMode stepMode;
    uint64_t arrayStride;
    size_t attributeCount;
    WGPUVertexAttribute const * attributes;

    inline operator const WGPUVertexBufferLayout&() const noexcept {
        return *reinterpret_cast<const WGPUVertexBufferLayout*>(this);
    }
};


struct JVertexState {
    JChainedStruct const * nextInChain = NULL;
    WGPUShaderModule module;
    WGPUStringView entryPoint;
    size_t constantCount;
    JConstantEntry const * constants = NULL;
    size_t bufferCount;
    JVertexBufferLayout const * buffers = NULL;

    inline operator const WGPUVertexState&() const noexcept {
        return *reinterpret_cast<const WGPUVertexState*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct;
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

    void SetBuffers(JVertexBufferLayout* value) {
        buffers = value;
    }
};

struct JShaderSourceWGSL : JChainedStruct {

    WGPUStringView code;

    inline operator const WGPUShaderSourceWGSL&() const noexcept {
        return *reinterpret_cast<const WGPUShaderSourceWGSL*>(this);
    }

    void SetCode(const char* value) {
        WGPUStringView stringView = {};
        stringView.data = strdup(value);
        stringView.length = strlen(value);
        code = stringView;
    }
};

struct JShaderModuleDescriptor {

    JChainedStruct const * nextInChain = NULL;
    WGPUStringView label;

    inline operator const WGPUShaderModuleDescriptor&() const noexcept {
        return *reinterpret_cast<const WGPUShaderModuleDescriptor*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct;
    }

    void SetLabel(const char* value) {
        WGPUStringView stringView = {};
        stringView.data = strdup(value);
        stringView.length = strlen(value);
        label = stringView;
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
    JChainedStruct const * nextInChain = nullptr;
    WGPUTextureFormat format = WGPUTextureFormat_Undefined;
    JBlendState const * blend = nullptr;
    WGPUColorWriteMask writeMask = WGPUColorWriteMask_All;

    inline operator const WGPUColorTargetState&() const noexcept {
        return *reinterpret_cast<const WGPUColorTargetState*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct;
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
    JChainedStruct const * nextInChain = NULL;
    WGPUShaderModule module;
    WGPUStringView entryPoint;
    size_t constantCount;
    JConstantEntry const * constants = NULL;
    size_t targetCount;
    JColorTargetState const * targets = nullptr;

    inline operator const WGPUFragmentState&() const noexcept {
        return *reinterpret_cast<const WGPUFragmentState*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct;
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

    JChainedStruct const * nextInChain = NULL;
    WGPUPrimitiveTopology topology;
    WGPUIndexFormat stripIndexFormat;
    WGPUFrontFace frontFace;
    WGPUCullMode cullMode;
    WGPUBool unclippedDepth;

    inline operator const WGPUPrimitiveState&() const noexcept {
        return *reinterpret_cast<const WGPUPrimitiveState*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct;
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

    JChainedStruct const * nextInChain = NULL;
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
        nextInChain = chainedStruct;
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

    JChainedStruct const * nextInChain = NULL;
    uint32_t count;
    uint32_t mask;
    WGPUBool alphaToCoverageEnabled;

    inline operator const WGPUMultisampleState&() const noexcept {
        return *reinterpret_cast<const WGPUMultisampleState*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct;
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

    JChainedStruct const * nextInChain = NULL;
    WGPUStringView label;
    WGPU_NULLABLE WGPUPipelineLayout layout;
    JVertexState vertex;
    JPrimitiveState primitive;
    JDepthStencilState const * depthStencil = NULL;
    JMultisampleState multisample;
    JFragmentState const * fragment = NULL;

    inline operator const WGPURenderPipelineDescriptor&() const noexcept {
        return *reinterpret_cast<const WGPURenderPipelineDescriptor*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct;
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

        static JRenderPipeline Obtain() {
            JRenderPipeline obj;
            return obj;
        }

        void Release() {
            wgpuRenderPipelineRelease(renderPipeline);
        }
};

struct JColor {
    double r;
    double g;
    double b;
    double a;

    inline operator const WGPUColor&() const noexcept {
        return *reinterpret_cast<const WGPUColor*>(this);
    }

    void SetColor(float r, float g, float b, float a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    float GetR() {
        return r;
    }
    float GetG() {
        return g;
    }
    float GetB() {
        return b;
    }
    float GetA() {
        return a;
    }
};

struct JSupportedFeatures {
    size_t featureCount;
    WGPUFeatureName const * features;

    inline operator const WGPUSupportedFeatures&() const noexcept {
        return *reinterpret_cast<const WGPUSupportedFeatures*>(this);
    }

    int GetFeatureCount() {
        return featureCount;
    }

    WGPUFeatureName GetFeatureAt(long index) {
        return features[index];
    }

};

class JBuffer {
    private:

    public:
        WGPUBuffer buffer;
};

class JRenderPassEncoder {
    private:

    public:
        WGPURenderPassEncoder renderPassEncoder;

        static JRenderPassEncoder Obtain() {
            JRenderPassEncoder obj;
            return obj;
        }

        void Release() {
            wgpuRenderPassEncoderRelease(renderPassEncoder);
        }

        void End() {
            wgpuRenderPassEncoderEnd(renderPassEncoder);
        }
};

class JComputePassEncoder {
    private:

    public:
        WGPUComputePassEncoder computePassEncoder;

        static JComputePassEncoder Obtain() {
            JComputePassEncoder obj;
            return obj;
        }

        void Release() {
            wgpuComputePassEncoderRelease(computePassEncoder);
        }
};

class JRenderPassDepthStencilAttachment {
    private:

    public:
        WGPURenderPassDepthStencilAttachment attachment;
};

class JRenderPassTimestampWrites {
    private:

    public:
//        WGPURenderPassTimestampWrites timestampWrites; // TODO not in emscripten
};

class JCommandEncoderDescriptor {
    private:

    public:
        WGPUCommandEncoderDescriptor descriptor;

        static JCommandEncoderDescriptor Obtain() {
            JCommandEncoderDescriptor obj;
            return obj;
        }

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            descriptor.label = stringView;
        }
};

class JCommandBufferDescriptor  {
    private:

    public:
        WGPUCommandBufferDescriptor descriptor;

        static JCommandBufferDescriptor Obtain() {
            JCommandBufferDescriptor obj;
            return obj;
        }

        void SetNextInChain(JChainedStruct* chainedStruct) {
            #ifdef __EMSCRIPTEN__
                descriptor.nextInChain = reinterpret_cast<WGPUChainedStruct * >(chainedStruct);
            #else
                descriptor.nextInChain = reinterpret_cast<WGPUChainedStruct const * >(chainedStruct);
            #endif
        }

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            descriptor.label = stringView;
        }
};

class JRenderPassDescriptor {
    private:

    public:
        WGPURenderPassDescriptor descriptor;

//            size_t colorAttachmentCount;
//            WGPURenderPassColorAttachment const * colorAttachments;
//            WGPU_NULLABLE WGPURenderPassDepthStencilAttachment const * depthStencilAttachment;
//            WGPU_NULLABLE WGPUQuerySet occlusionQuerySet;
//            WGPU_NULLABLE WGPURenderPassTimestampWrites const * timestampWrites;

        static JRenderPassDescriptor Obtain() {
            JRenderPassDescriptor descriptor;
            return descriptor;
        }

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            descriptor.label = stringView;
        }

        void SetColorAttachmentCount(int size) {
            descriptor.colorAttachmentCount = size;
        }

        void SetColorAttachments(JRenderPassColorAttachment* colorAttachment) {
            // TODO Must be an array
            descriptor.colorAttachments = reinterpret_cast<WGPURenderPassColorAttachment const * >(colorAttachment);
        }

        void SetDepthStencilAttachment(JRenderPassDepthStencilAttachment* attachment) {
            descriptor.depthStencilAttachment = attachment == NULL ? NULL : &(attachment->attachment);
        }

        void SetTimestampWrites(JRenderPassTimestampWrites* timestampWrites) {
            // TODO emscripten does not have this class
//            descriptor.timestampWrites = timestampWrites == NULL ? NULL : &(timestampWrites->timestampWrites);
        }
};

class JComputePassDescriptor {
    private:

    public:
        WGPUComputePassDescriptor descriptor;

        static JComputePassDescriptor Obtain() {
            JComputePassDescriptor descriptor;
            return descriptor;
        }

        void SetLabel(const char* value) {
            WGPUStringView stringView = {};
            stringView.data = strdup(value);
            stringView.length = strlen(value);
            descriptor.label = stringView;
        }
};

class JCommandEncoder {
    private:

    public:
        WGPUCommandEncoder commandEncoder;

        static JCommandEncoder Obtain() {
            JCommandEncoder obj;
            return obj;
        }

        void Release() {
            wgpuCommandEncoderRelease(commandEncoder);
        }

        void BeginRenderPass(JRenderPassDescriptor* renderPassDescriptor, JRenderPassEncoder* encoder) {
            encoder->renderPassEncoder = wgpuCommandEncoderBeginRenderPass(commandEncoder, &(renderPassDescriptor->descriptor));
        }

        void BeginComputePass(JComputePassEncoder* encoder) {
            WGPUComputePassDescriptor computePassDescriptor;
            encoder->computePassEncoder = wgpuCommandEncoderBeginComputePass(commandEncoder, &computePassDescriptor);
        }

        void ClearBuffer(JBuffer* buffer, int offset, int size) {
            wgpuCommandEncoderClearBuffer(commandEncoder, buffer->buffer, offset, size);
        }

        void Finish(JCommandBufferDescriptor* commandBufferDescriptor, JCommandBuffer* commandBuffer) {
            commandBuffer->commandBuffer = wgpuCommandEncoderFinish(commandEncoder, &(commandBufferDescriptor->descriptor));
        }
};

class JDevice {

    private:

    public:
        WGPUDevice device;

        void Release() {
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

        void CreateRenderPipeline(JRenderPipelineDescriptor* pipelineDescriptor, JRenderPipeline* renderPipeline) {
            renderPipeline->renderPipeline = wgpuDeviceCreateRenderPipeline(device, reinterpret_cast<WGPURenderPipelineDescriptor const * >(pipelineDescriptor));
        }

        void CreateShaderModule(JShaderModuleDescriptor* shaderModuleDescriptor, JShaderModule* shaderModule) {
            shaderModule->shaderModule = wgpuDeviceCreateShaderModule(device, reinterpret_cast<WGPUShaderModuleDescriptor const * >(shaderModuleDescriptor));
        }

        void GetFeatures(JSupportedFeatures* features) {
            wgpuDeviceGetFeatures(device, reinterpret_cast<WGPUSupportedFeatures * >(features));
        }

        void GetLimits(JLimits* limits) {
            wgpuDeviceGetLimits(device, reinterpret_cast<WGPULimits * >(&(limits->limits)));
        }

        void CreateCommandEncoder(JCommandEncoderDescriptor* encoderDescriptor, JCommandEncoder* encoder) {
            encoder->commandEncoder = wgpuDeviceCreateCommandEncoder(device, &(encoderDescriptor->descriptor));
        }
};

struct JSurfaceConfiguration {

    JChainedStruct const * nextInChain = NULL;
    WGPUDevice device;
    WGPUTextureFormat format;
    WGPUTextureUsage usage;
    uint32_t width;
    uint32_t height;
    size_t viewFormatCount;
    WGPUTextureFormat const * viewFormats = NULL;
    WGPUCompositeAlphaMode alphaMode;
    WGPUPresentMode presentMode;

    inline operator const WGPUSurfaceConfiguration&() const noexcept {
        return *reinterpret_cast<const WGPUSurfaceConfiguration*>(this);
    }

    void SetNextInChain(JChainedStruct* chainedStruct) {
        nextInChain = chainedStruct;
    }

    void SetWidth(long width) {
        this->width = width;
    }

    void SetHeight(long height) {
        this->height = height;
    }

    void SetFormat(WGPUTextureFormat format) {
        this->format = format;
    }

    void SetViewFormatCount(long value) {
        viewFormatCount = value;
    }

    void SetViewFormats(void* viewFormats) {
        this->viewFormats = (WGPUTextureFormat*)viewFormats;
    }

    void SetUsage(WGPUTextureUsage usage) {
        this->usage = usage;
    }

    void SetDevice(JDevice* device) {
        this->device = device != NULL ? device->device : NULL;
    }

    void SetPresentMode(WGPUPresentMode presentMode) {
        this->presentMode = presentMode;
    }

    void SetAlphaMode(WGPUCompositeAlphaMode alphaMode) {
        this->alphaMode = alphaMode;
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

        void Release() {
            wgpuAdapterRelease(adapter);
        }

        void RequestDevice(JDeviceDescriptor* descriptor, WGPUCallbackMode mode, RequestDeviceCallback* callback, UncapturedErrorCallback* errorCallback) {
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
            const WGPUDeviceDescriptor* desc = reinterpret_cast<WGPUDeviceDescriptor const*>(descriptor);
            auto result = wgpuAdapterRequestDevice(adapter, desc, callbackInfo);
        }

        bool GetInfo(JAdapterInfo* adapterInfo) {
            WGPUStatus status = wgpuAdapterGetInfo(adapter, &(adapterInfo->adapterInfo));
            return status == WGPUStatus_Success;
        }

        bool HasFeature(WGPUFeatureName featureName) {
            return wgpuAdapterHasFeature(adapter, featureName);
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

class JTextureViewDescriptor {
    private:

    public:
        WGPUTextureViewDescriptor descriptor;

    static JTextureViewDescriptor Obtain() {
        JTextureViewDescriptor textureViewDescriptor;
        return textureViewDescriptor;
    }

    void SetLabel(const char* value) {
        WGPUStringView stringView = {};
        stringView.data = strdup(value);
        stringView.length = strlen(value);
        descriptor.label = stringView;
    }

    void SetFormat(WGPUTextureFormat format) {
        descriptor.format = format;
    }

    void SetDimension(WGPUTextureViewDimension dimension) {
        descriptor.dimension = dimension;
    }

    void SetBaseMipLevel(int baseMipLevel) {
        descriptor.baseMipLevel = baseMipLevel;
    }

    void SetMipLevelCount(int mipLevelCount) {
        descriptor.mipLevelCount = mipLevelCount;
    }

    void SetBaseArrayLayer(int baseArrayLayer) {
        descriptor.baseArrayLayer = baseArrayLayer;
    }

    void SetArrayLayerCount(int arrayLayerCount) {
        descriptor.arrayLayerCount = arrayLayerCount;
    }

    void SetAspect(WGPUTextureAspect aspect) {
        descriptor.aspect = aspect;
    }

    void SetUsage(WGPUTextureUsage usage) {
        descriptor.usage = usage;
    }
};

class JTextureView {
    private:

    public:
        WGPUTextureView textureView;

    static JTextureView Obtain() {
        JTextureView obj;
        return obj;
    }

    void Release() {
        wgpuTextureViewRelease(textureView);
    }
};

class JTexture {
    private:

    public:
        WGPUTexture texture;

        static JTexture Obtain() {
            JTexture obj;
            return obj;
        }

        void CreateView(JTextureViewDescriptor* textureViewDescriptor, JTextureView* textureView) {
            textureView->textureView = wgpuTextureCreateView(texture, &(textureViewDescriptor->descriptor));
        }

        WGPUTextureFormat GetFormat() {
            return wgpuTextureGetFormat(texture);
        }

        void Release() {
            wgpuTextureRelease(texture);
        }
};

struct JRenderPassColorAttachment {
    JChainedStruct const * nextInChain = NULL;
    WGPU_NULLABLE WGPUTextureView view;
    uint32_t depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
    WGPU_NULLABLE WGPUTextureView resolveTarget;
    WGPULoadOp loadOp;
    WGPUStoreOp storeOp;
    JColor clearValue;

    inline operator const WGPURenderPassColorAttachment&() const noexcept {
        return *reinterpret_cast<const WGPURenderPassColorAttachment*>(this);
    }

    static JRenderPassColorAttachment Obtain() {
        JRenderPassColorAttachment renderPassColorAttachment;
        return renderPassColorAttachment;
    }

    void SetResolveTarget(JTextureView* textureView) {
        this->resolveTarget = textureView == NULL ? NULL : textureView->textureView;
    }

    void SetView(JTextureView* textureView) {
        this->view = textureView == NULL ? NULL : textureView->textureView;
    }

    void SetLoadOp(WGPULoadOp loadOp) {
        this->loadOp = loadOp;
    }

    void SetStoreOp(WGPUStoreOp storeOp) {
        this->storeOp = storeOp;
    }

    void SetDepthSlice(int depthSlice) {
        this->depthSlice = depthSlice;
    }

    JColor& GetClearValue() {
        return clearValue;
    }
};

struct JSurfaceTexture {

    JChainedStruct * nextInChain = NULL;
    WGPUTexture texture;
    WGPUSurfaceGetCurrentTextureStatus status;

    inline operator const WGPUSurfaceTexture&() const noexcept {
        return *reinterpret_cast<const WGPUSurfaceTexture*>(this);
    }

    static JSurfaceTexture Obtain() {
        JSurfaceTexture obj;
        return obj;
    }

    void GetTexture(JTexture* tex) {
        tex->texture = texture;
    }

    WGPUSurfaceGetCurrentTextureStatus GetStatus() {
        return status;
    }
};

class JSurface {
    private:

    public:
        WGPUSurface surface;

        void Unconfigure() {
            wgpuSurfaceUnconfigure(surface);
        }

        void Release() {
            wgpuSurfaceRelease(surface);
        }

        void Configure(JSurfaceConfiguration* config) {
            wgpuSurfaceConfigure(surface, reinterpret_cast<WGPUSurfaceConfiguration const * >(config));
        }

        void GetCapabilities(JAdapter* adapter, JSurfaceCapabilities* surfaceCapabilities) {
            wgpuSurfaceGetCapabilities(surface, adapter->adapter, &(surfaceCapabilities->surfaceCapabilities));
        }

        void GetCurrentTexture(JSurfaceTexture* tex) {
            wgpuSurfaceGetCurrentTexture(surface, reinterpret_cast<WGPUSurfaceTexture * >(tex));
        }

        void Present() {
            wgpuSurfacePresent(surface);
        }
};

class JInstance {
    private:

    public:
        WGPUInstance instance;
        bool isValid = false;

        JInstance() {
            instance = wgpuCreateInstance(NULL);
            if (!instance) {
                isValid = false;
            }
            else {
                isValid = true;
            }
        }

        bool IsValid() {
            return isValid;
        }

        void Release() {
            if(isValid) {
                isValid = false;
                wgpuInstanceRelease(instance);
            }
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

            shaderDesc.SetNextInChain(&shaderCodeDesc);
            JShaderModule* shaderModule = new JShaderModule();
            device->CreateShaderModule(&shaderDesc, shaderModule);

            JRenderPipelineDescriptor pipelineDesc;
            pipelineDesc.SetLabel("my pipeline");

            pipelineDesc.GetVertex().SetBufferCount(0); // no vertex buffer, because we define it in the shader
            pipelineDesc.GetVertex().SetBuffers(NULL);

            pipelineDesc.GetVertex().SetModule(shaderModule);
            pipelineDesc.GetVertex().SetEntryPoint("vs_main");
            pipelineDesc.GetVertex().SetConstantCount(0);
            pipelineDesc.GetVertex().SetConstants(NULL);

            pipelineDesc.GetPrimitive().SetTopology(WGPUPrimitiveTopology_TriangleList);
            pipelineDesc.GetPrimitive().SetStripIndexFormat(WGPUIndexFormat_Undefined);
            pipelineDesc.GetPrimitive().SetFrontFace(WGPUFrontFace_CCW);
            pipelineDesc.GetPrimitive().SetCullMode(WGPUCullMode_None);

            JFragmentState fragmentState;
            fragmentState.SetNextInChain(NULL);
            fragmentState.SetModule(shaderModule);
            fragmentState.SetEntryPoint("fs_main");
            fragmentState.SetConstantCount(0);
            fragmentState.SetConstants(NULL);

            // blending
            JBlendState blendState;
            blendState.GetColor().SetSrcFactor(WGPUBlendFactor_SrcAlpha);
            blendState.GetColor().SetDstFactor(WGPUBlendFactor_OneMinusSrcAlpha);
            blendState.GetColor().SetOperation(WGPUBlendOperation_Add);
            blendState.GetAlpha().SetSrcFactor(WGPUBlendFactor_One);
            blendState.GetAlpha().SetDstFactor(WGPUBlendFactor_Zero);
            blendState.GetAlpha().SetOperation(WGPUBlendOperation_Add);

            JColorTargetState colorTarget;
            colorTarget.SetFormat(surfaceFormat); // match output surface
            colorTarget.SetBlend(&blendState);
            colorTarget.SetWriteMask(WGPUColorWriteMask_All);

            fragmentState.SetTargetCount(1);

            fragmentState.SetTargets(&colorTarget);

            pipelineDesc.SetFragment(&fragmentState);

            pipelineDesc.SetDepthStencil(NULL); // no depth or stencil buffer

            pipelineDesc.GetMultisample().SetCount(1);
            pipelineDesc.GetMultisample().SetMask(-1);
            pipelineDesc.GetMultisample().SetAlphaToCoverageEnabled(0);

            pipelineDesc.SetLayout(NULL);

            JRenderPipeline* renderPipeline = new JRenderPipeline();

            const WGPURenderPipelineDescriptor* desc = reinterpret_cast<WGPURenderPipelineDescriptor const * >(&pipelineDesc);
            renderPipeline->renderPipeline = wgpuDeviceCreateRenderPipeline(device->device, desc);
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

