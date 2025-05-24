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


class JStringView : public JChainedStruct {
    private:

    public:

};

struct JLimits {
    WGPUChainedStruct * nextInChain;
    uint32_t maxTextureDimension1D;
    uint32_t maxTextureDimension2D;
    uint32_t maxTextureDimension3D;
    uint32_t maxTextureArrayLayers;
    uint32_t maxBindGroups;
    uint32_t maxBindGroupsPlusVertexBuffers;
    uint32_t maxBindingsPerBindGroup;
    uint32_t maxDynamicUniformBuffersPerPipelineLayout;
    uint32_t maxDynamicStorageBuffersPerPipelineLayout;
    uint32_t maxSampledTexturesPerShaderStage;
    uint32_t maxSamplersPerShaderStage;
    uint32_t maxStorageBuffersPerShaderStage;
    uint32_t maxStorageTexturesPerShaderStage;
    uint32_t maxUniformBuffersPerShaderStage;
    uint64_t maxUniformBufferBindingSize;
    uint64_t maxStorageBufferBindingSize;
    uint32_t minUniformBufferOffsetAlignment;
    uint32_t minStorageBufferOffsetAlignment;
    uint32_t maxVertexBuffers;
    uint64_t maxBufferSize;
    uint32_t maxVertexAttributes;
    uint32_t maxVertexBufferArrayStride;
    uint32_t maxInterStageShaderVariables;
    uint32_t maxColorAttachments;
    uint32_t maxColorAttachmentBytesPerSample;
    uint32_t maxComputeWorkgroupStorageSize;
    uint32_t maxComputeInvocationsPerWorkgroup;
    uint32_t maxComputeWorkgroupSizeX;
    uint32_t maxComputeWorkgroupSizeY;
    uint32_t maxComputeWorkgroupSizeZ;
    uint32_t maxComputeWorkgroupsPerDimension;

    inline operator const WGPULimits&() const noexcept {
        return *reinterpret_cast<const WGPULimits*>(this);
    }

    void SetMaxTextureDimension1D(int value) {
        maxTextureDimension1D = value;
    }
    int GetMaxTextureDimension1D() const {
        return static_cast<int>(maxTextureDimension1D);
    }

    void SetMaxTextureDimension2D(int value) {
        maxTextureDimension2D = value;
    }
    int GetMaxTextureDimension2D() const {
        return static_cast<int>(maxTextureDimension2D);
    }

    void SetMaxTextureDimension3D(int value) {
        maxTextureDimension3D = value;
    }
    int GetMaxTextureDimension3D() const {
        return static_cast<int>(maxTextureDimension3D);
    }

    void SetMaxTextureArrayLayers(int value) {
        maxTextureArrayLayers = value;
    }
    int GetMaxTextureArrayLayers() const {
        return static_cast<int>(maxTextureArrayLayers);
    }

    void SetMaxBindGroups(int value) {
        maxBindGroups = value;
    }
    int GetMaxBindGroups() const {
        return static_cast<int>(maxBindGroups);
    }

    void SetMaxBindGroupsPlusVertexBuffers(int value) {
        maxBindGroupsPlusVertexBuffers = value;
    }
    int GetMaxBindGroupsPlusVertexBuffers() const {
        return static_cast<int>(maxBindGroupsPlusVertexBuffers);
    }

    void SetMaxBindingsPerBindGroup(int value) {
        maxBindingsPerBindGroup = value;
    }
    int GetMaxBindingsPerBindGroup() const {
        return static_cast<int>(maxBindingsPerBindGroup);
    }

    void SetMaxDynamicUniformBuffersPerPipelineLayout(int value) {
        maxDynamicUniformBuffersPerPipelineLayout = value;
    }
    int GetMaxDynamicUniformBuffersPerPipelineLayout() const {
        return static_cast<int>(maxDynamicUniformBuffersPerPipelineLayout);
    }

    void SetMaxDynamicStorageBuffersPerPipelineLayout(int value) {
        maxDynamicStorageBuffersPerPipelineLayout = value;
    }
    int GetMaxDynamicStorageBuffersPerPipelineLayout() const {
        return static_cast<int>(maxDynamicStorageBuffersPerPipelineLayout);
    }

    void SetMaxSampledTexturesPerShaderStage(int value) {
        maxSampledTexturesPerShaderStage = value;
    }
    int GetMaxSampledTexturesPerShaderStage() const {
        return static_cast<int>(maxSampledTexturesPerShaderStage);
    }

    void SetMaxSamplersPerShaderStage(int value) {
        maxSamplersPerShaderStage = value;
    }
    int GetMaxSamplersPerShaderStage() const {
        return static_cast<int>(maxSamplersPerShaderStage);
    }

    void SetMaxStorageBuffersPerShaderStage(int value) {
        maxStorageBuffersPerShaderStage = value;
    }
    int GetMaxStorageBuffersPerShaderStage() const {
        return static_cast<int>(maxStorageBuffersPerShaderStage);
    }

    void SetMaxStorageTexturesPerShaderStage(int value) {
        maxStorageTexturesPerShaderStage = value;
    }
    int GetMaxStorageTexturesPerShaderStage() const {
        return static_cast<int>(maxStorageTexturesPerShaderStage);
    }

    void SetMaxUniformBuffersPerShaderStage(int value) {
        maxUniformBuffersPerShaderStage = value;
    }
    int GetMaxUniformBuffersPerShaderStage() const {
        return static_cast<int>(maxUniformBuffersPerShaderStage);
    }

    void SetMaxUniformBufferBindingSize(int value) {
        maxUniformBufferBindingSize = value;
    }
    int GetMaxUniformBufferBindingSize() const {
        return static_cast<int>(maxUniformBufferBindingSize);
    }

    void SetMaxStorageBufferBindingSize(int value) {
        maxStorageBufferBindingSize = value;
    }
    int GetMaxStorageBufferBindingSize() const {
        return static_cast<int>(maxStorageBufferBindingSize);
    }

    void SetMinUniformBufferOffsetAlignment(int value) {
        minUniformBufferOffsetAlignment = value;
    }
    int GetMinUniformBufferOffsetAlignment() const {
        return static_cast<int>(minUniformBufferOffsetAlignment);
    }

    void SetMinStorageBufferOffsetAlignment(int value) {
        minStorageBufferOffsetAlignment = value;
    }
    int GetMinStorageBufferOffsetAlignment() const {
        return static_cast<int>(minStorageBufferOffsetAlignment);
    }

    void SetMaxVertexBuffers(int value) {
        maxVertexBuffers = value;
    }
    int GetMaxVertexBuffers() const {
        return static_cast<int>(maxVertexBuffers);
    }

    void SetMaxBufferSize(int value) {
        maxBufferSize = value;
    }
    int GetMaxBufferSize() const {
        return static_cast<int>(maxBufferSize);
    }

    void SetMaxVertexAttributes(int value) {
        maxVertexAttributes = value;
    }
    int GetMaxVertexAttributes() const {
        return static_cast<int>(maxVertexAttributes);
    }

    void SetMaxVertexBufferArrayStride(int value) {
        maxVertexBufferArrayStride = value;
    }
    int GetMaxVertexBufferArrayStride() const {
        return static_cast<int>(maxVertexBufferArrayStride);
    }

    void SetMaxInterStageShaderVariables(int value) {
        maxInterStageShaderVariables = value;
    }
    int GetMaxInterStageShaderVariables() const {
        return static_cast<int>(maxInterStageShaderVariables);
    }

    void SetMaxColorAttachments(int value) {
        maxColorAttachments = value;
    }
    int GetMaxColorAttachments() const {
        return static_cast<int>(maxColorAttachments);
    }

    void SetMaxColorAttachmentBytesPerSample(int value) {
        maxColorAttachmentBytesPerSample = value;
    }
    int GetMaxColorAttachmentBytesPerSample() const {
        return static_cast<int>(maxColorAttachmentBytesPerSample);
    }

    void SetMaxComputeWorkgroupStorageSize(int value) {
        maxComputeWorkgroupStorageSize = value;
    }
    int GetMaxComputeWorkgroupStorageSize() const {
        return static_cast<int>(maxComputeWorkgroupStorageSize);
    }

    void SetMaxComputeInvocationsPerWorkgroup(int value) {
        maxComputeInvocationsPerWorkgroup = value;
    }
    int GetMaxComputeInvocationsPerWorkgroup() const {
        return static_cast<int>(maxComputeInvocationsPerWorkgroup);
    }

    void SetMaxComputeWorkgroupSizeX(int value) {
        maxComputeWorkgroupSizeX = value;
    }
    int GetMaxComputeWorkgroupSizeX() const {
        return static_cast<int>(maxComputeWorkgroupSizeX);
    }

    void SetMaxComputeWorkgroupSizeY(int value) {
        maxComputeWorkgroupSizeY = value;
    }
    int GetMaxComputeWorkgroupSizeY() const {
        return static_cast<int>(maxComputeWorkgroupSizeY);
    }

    void SetMaxComputeWorkgroupSizeZ(int value) {
        maxComputeWorkgroupSizeZ = value;
    }
    int GetMaxComputeWorkgroupSizeZ() const {
        return static_cast<int>(maxComputeWorkgroupSizeZ);
    }

    void SetMaxComputeWorkgroupsPerDimension(int value) {
        maxComputeWorkgroupsPerDimension = value;
    }
    int GetMaxComputeWorkgroupsPerDimension() const {
        return static_cast<int>(maxComputeWorkgroupsPerDimension);
    }
};

// TODO wgpu-native limits is different from emscripten dawn limits. Need to change back to reference

//static_assert(sizeof(JLimits) == sizeof(WGPULimits), "sizeof mismatch for JLimits");
//static_assert(alignof(JLimits) == alignof(WGPULimits), "alignof mismatch for JLimits");
//static_assert(offsetof(JLimits, nextInChain) == offsetof(WGPULimits, nextInChain), "offsetof mismatch for JLimits::nextInChain");

struct JQueueDescriptor {
    JChainedStruct const * nextInChain;
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
    JLimits const * requiredLimits = NULL;
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
        requiredLimits = limits;
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

        ~JRenderPipeline() {
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

struct JRenderPassColorAttachment {
    JChainedStruct const * nextInChain;
    WGPUTextureView view;
    uint32_t depthSlice;
    WGPUTextureView resolveTarget;
    WGPULoadOp loadOp;
    WGPUStoreOp storeOp;
    JColor clearValue;

    inline operator const WGPURenderPassColorAttachment&() const noexcept {
        return *reinterpret_cast<const WGPURenderPassColorAttachment*>(this);
    }

    JColor& GetClearValue() {
        return clearValue;
    }

    WGPUStoreOp GetStoreOp() {
        return storeOp;
    }
};

class JRenderPassEncoder {
    private:

    public:
        WGPURenderPassEncoder renderPassEncoder;

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
};

class JCommandBuffer {
    private:

    public:
        WGPUCommandBuffer commandBuffer;

        void Release() {
            wgpuCommandBufferRelease(commandBuffer);
        }
};

class JCommandEncoder {
    private:

    public:
        WGPUCommandEncoder commandEncoder;

        void Release() {
            wgpuCommandEncoderRelease(commandEncoder);
        }

        JRenderPassEncoder BeginRenderPass() {
            WGPURenderPassDescriptor renderPassDescriptor;
            JRenderPassEncoder encoder;
            encoder.renderPassEncoder = wgpuCommandEncoderBeginRenderPass(commandEncoder, &renderPassDescriptor);
            return encoder;
        }

        JComputePassEncoder BeginComputePass() {
            WGPUComputePassDescriptor computePassDescriptor;
            JComputePassEncoder encoder;
            encoder.computePassEncoder = wgpuCommandEncoderBeginComputePass(commandEncoder, &computePassDescriptor);
            return encoder;
        }

        void ClearBuffer(JBuffer* buffer, int offset, int size) {
            wgpuCommandEncoderClearBuffer(commandEncoder, buffer->buffer, offset, size);
        }

        JCommandBuffer Finish(const char* label) {
            WGPUStringView stringView = {};
            stringView.data = strdup(label);
            stringView.length = strlen(label);
            WGPUCommandBufferDescriptor commandBufferDescriptor;
            commandBufferDescriptor.label = stringView;
            JCommandBuffer commandBuffer;
            commandBuffer.commandBuffer = wgpuCommandEncoderFinish(commandEncoder, &commandBufferDescriptor);
            return commandBuffer;
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
            shaderModule->shaderModule = wgpuDeviceCreateShaderModule(device, reinterpret_cast<WGPUShaderModuleDescriptor const * >(shaderModuleDescriptor));
            return shaderModule;
        }

        void GetFeatures(JSupportedFeatures* features) {
            wgpuDeviceGetFeatures(device, reinterpret_cast<WGPUSupportedFeatures * >(features));
        }

        void GetLimits(JLimits* limits) {
            wgpuDeviceGetLimits(device, reinterpret_cast<WGPULimits * >(limits));
        }

        JCommandEncoder CreateCommandEncoder(const char* label) {
            WGPUStringView stringView = {};
            stringView.data = strdup(label);
            stringView.length = strlen(label);

            WGPUCommandEncoderDescriptor encoderDescriptor;
            encoderDescriptor.label = stringView;
            JCommandEncoder encoder;
            encoder.commandEncoder = wgpuDeviceCreateCommandEncoder(device, &encoderDescriptor);
            return encoder;
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

        JAdapter() {
        }

        ~JAdapter() {
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

struct JTextureViewDescriptor {
    WGPUChainedStruct const * nextInChain;
    WGPUStringView label;
    WGPUTextureFormat format;
    WGPUTextureViewDimension dimension;
    uint32_t baseMipLevel;
    uint32_t mipLevelCount;
    uint32_t baseArrayLayer;
    uint32_t arrayLayerCount;
    WGPUTextureAspect aspect;
    WGPUTextureUsage usage;

    inline operator const WGPUTextureViewDescriptor&() const noexcept {
        return *reinterpret_cast<const WGPUTextureViewDescriptor*>(this);
    }

    void SetLabel(const char* value) {
        WGPUStringView stringView = {};
        stringView.data = strdup(value);
        stringView.length = strlen(value);
        label = stringView;
    }

    void SetFormat(WGPUTextureFormat format) {
        this->format = format;
    }

    void SetDimension(WGPUTextureViewDimension dimension) {
        this->dimension = dimension;
    }

    void SetBaseMipLevel(int baseMipLevel) {
        this->baseMipLevel = baseMipLevel;
    }

    void SetMipLevelCount(int mipLevelCount) {
        this->mipLevelCount = mipLevelCount;
    }

    void SetBaseArrayLayer(int baseArrayLayer) {
        this->baseArrayLayer = baseArrayLayer;
    }

    void SetArrayLayerCount(int arrayLayerCount) {
        this->arrayLayerCount = arrayLayerCount;
    }

    void SetAspect(WGPUTextureAspect aspect) {
        this->aspect = aspect;
    }

    void SetUsage(WGPUTextureUsage usage) {
        this->usage = usage;
    }
};

class JTextureView {
    private:

    public:
        WGPUTextureView textureView;

};

class JTexture {
    private:

    public:
        WGPUTexture texture;

        JTextureView CreateView(JTextureViewDescriptor* textureViewDescriptor) {
            JTextureView textureView;
            textureView.textureView = wgpuTextureCreateView(texture, reinterpret_cast<WGPUTextureViewDescriptor const * >(textureViewDescriptor));
            return textureView;
        }
};

struct JSurfaceTexture {

    WGPUChainedStruct * nextInChain;
    WGPUTexture texture;
    WGPUSurfaceGetCurrentTextureStatus status;

    inline operator const WGPUSurfaceTexture&() const noexcept {
        return *reinterpret_cast<const WGPUSurfaceTexture*>(this);
    }

    JTexture GetTexture() {
        JTexture tex;
        tex.texture = texture;
        return tex;
    }

    WGPUSurfaceGetCurrentTextureStatus GetStatus() {
        return status;
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
            wgpuSurfaceConfigure(surface, reinterpret_cast<WGPUSurfaceConfiguration const * >(config));
        }

        void GetCapabilities(JAdapter* adapter, JSurfaceCapabilities* surfaceCapabilities) {
            wgpuSurfaceGetCapabilities(surface, adapter->adapter, &(surfaceCapabilities->surfaceCapabilities));
        }

        void GetCurrentTexture(JSurfaceTexture* surfaceTexture) {
            wgpuSurfaceGetCurrentTexture(surface, reinterpret_cast<WGPUSurfaceTexture * >(surfaceTexture));
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

        static JRenderPipeline* CreateRenderPipeline(JDevice* device, const char* shaderSource, WGPUTextureFormat surfaceFormat) {

            JShaderModuleDescriptor shaderDesc;
            shaderDesc.SetLabel("triangle shader");

            JShaderSourceWGSL shaderCodeDesc;
            shaderCodeDesc.SetNext(NULL);
            shaderCodeDesc.SetSType(WGPUSType_ShaderSourceWGSL);
            shaderCodeDesc.SetCode(shaderSource);

            shaderDesc.SetNextInChain(&shaderCodeDesc);
            JShaderModule* shaderModule = device->CreateShaderModule(&shaderDesc);

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

