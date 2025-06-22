#pragma once

#include "IDLHelper.h"
#include "webgpu/webgpu.h"
#include <iostream>
#include <cstring>
#include <memory>

#if _WIN32
    #include <windows.h>
    #include <glfw3.h>
    #include <glfw3native.h>
#endif

#ifdef __APPLE__
    #include <TargetConditionals.h>
    #ifdef TARGET_OS_MAC
        #include <glfw3.h>
        #include <glfw3native.h>
        #include <QuartzCore/CAMetalLayer.h>
        #include <Cocoa/Cocoa.h>
    #endif
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

    int overflow(int c) override;
    int sync() override;
};

// Singleton to manage Logcat redirection
class LogcatRedirector {
public:
    static void initialize();

private:
    LogcatRedirector();
    ~LogcatRedirector();

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
class WebGPUVertexAttribute;
class WebGPUBindGroup;
class WebGPUBindGroupLayout;
class WebGPUBindGroupLayoutEntry;
class WebGPUInstance;
class WebGPUShaderModule;
class WebGPUQuerySet;
class WebGPUTexture;
class WebGPUBuffer;
class WebGPUTextureView;
class WebGPUPipelineLayout;
class WebGPUCommandEncoder;
class WebGPUComputePipeline;

class WGPUFloatBuffer;
class WGPUShortBuffer;

#ifdef __EMSCRIPTEN__
using WGPURenderPassTimestampWrites = WGPUPassTimestampWrites; // dawn version TODO remove when both are the same
using WGPUComputePassTimestampWrites = WGPUPassTimestampWrites; // dawn version TODO remove when both are the same
using WGPUProgrammableStageDescriptor = WGPUComputeState; // dawn version TODO remove when both are the same
#else
using WGPURenderPassTimestampWrites = WGPURenderPassTimestampWrites;  // wgpu-native version TODO remove when both are the same
using WGPUComputePassTimestampWrites = WGPUComputePassTimestampWrites;  // wgpu-native version TODO remove when both are the same
using WGPUProgrammableStageDescriptor = WGPUProgrammableStageDescriptor;  // wgpu-native version TODO remove when both are the same
#endif //__EMSCRIPTEN__

// ################################### CUSTOM CODE ###################################

enum WGPUPlatformType : int {
    WGPU_Unknown = 0,
    WGPU_Windows,
    WGPU_Mac,
    WGPU_Linux,
    WGPU_iOS,
    WGPU_Android,
    WGPU_Web
};

class WGPU {
    public:
        static WGPUPlatformType GetPlatformType();
        static WebGPUInstance CreateInstance();
};


class WGPUByteBuffer {
    private:
        // Detect host endianness at compile time
        static bool isLittleEndianHost();
        // Portable byte-swapping functions
        static uint16_t swapBytes(uint16_t value);
        static uint32_t swapBytes(uint32_t value);
        static uint64_t swapBytes(uint64_t value);

    public:
        enum WGPUByteOrder : int { BigEndian = 0, LittleEndian };

    private:
        std::vector<uint8_t> buffer;
        size_t _position = 0;
        size_t _limit;
        WGPUByteOrder byteOrder = WGPUByteOrder::BigEndian;
        std::unique_ptr<WGPUFloatBuffer> floatBuffer;
        std::unique_ptr<WGPUShortBuffer> shortBuffer;
        bool isClearing = false;

    public:
        static WGPUByteBuffer Obtain();
        explicit WGPUByteBuffer();
        explicit WGPUByteBuffer(int capacity);
        int size();
        void push_back(char value);
        const uint8_t* data();
        void order(WGPUByteOrder order);
        void put(char value);
        char get(int index);
        int remaining() const;
        void position(int newPosition);
        int getPosition();
        void limit(int newLimit);
        size_t getLimit() const;
        void clear();
        template<typename T>
        void putNumeric(int index, T value);
        template<typename T>
        T getNumeric(int index);
        WGPUFloatBuffer& asFloatBuffer();
        WGPUShortBuffer& asShortBuffer();

    friend class WGPUFloatBuffer;
    friend class WGPUShortBuffer;
};

using WGPUByteOrder = WGPUByteBuffer::WGPUByteOrder;

class WGPUFloatBuffer {
    private:
        WGPUByteBuffer& parent;
    public:
        size_t startPosition;
        size_t floatLimit;
        WGPUFloatBuffer(WGPUByteBuffer& bb);
        void put(float value);
        float get();
        long remaining() const;
        void position(int newPosition);
        int getPosition() const;
        void clear();
        void limit(int newLimit);
        int getLimit() const;
};

class WGPUShortBuffer {
    private:
        WGPUByteBuffer& parent;
    public:
        size_t startPosition;
        size_t shortLimit;
        WGPUShortBuffer(WGPUByteBuffer& bb);
        WGPUShortBuffer& put(int16_t value);
        int16_t get();
        void clear();
        void limit(int newLimit);
        int getLimit() const;
        void position(size_t newPosition);
        int getPosition() const;
        int remaining() const;
};

class WGPUAndroidWindow {
    public:
        #ifdef __ANDROID__
            ANativeWindow* g_window = nullptr;
        #endif
        ~WGPUAndroidWindow();
        void SetWindow(void * window);
        void* GetWindow();
        void InitLogcat();
};

// ################################### VECTORS ###################################

class WebGPUVectorColorTargetState {
    private:
        std::vector<WebGPUColorTargetState> vector;
    public:
        static WebGPUVectorColorTargetState Obtain();
        int size();
        void push_back(const WebGPUColorTargetState& attachment);
        const WebGPUColorTargetState* data();
};

class WebGPUVectorFeatureName {
    private:
        std::vector<WGPUFeatureName> vector;
    public:
        static WebGPUVectorFeatureName Obtain();
        int size();
        void push_back(const WGPUFeatureName& attachment);
        const WGPUFeatureName* data();
};

class WebGPUVectorConstantEntry {
    private:
        std::vector<WebGPUConstantEntry> vector;
    public:
        static WebGPUVectorConstantEntry Obtain();
        int size();
        void push_back(const WebGPUConstantEntry& attachment);
        const WebGPUConstantEntry* data();
};

class WebGPUVectorVertexBufferLayout {
    private:
        std::vector<WebGPUVertexBufferLayout> vector;
    public:
        static WebGPUVectorVertexBufferLayout Obtain();
        int size();
        void push_back(const WebGPUVertexBufferLayout& attachment);
        const WebGPUVertexBufferLayout* data();
};

class WebGPUVectorTextureFormat {
    private:
        std::vector<WGPUTextureFormat> vector;
    public:
        static WebGPUVectorTextureFormat Obtain();
        int size();
        void push_back(const WGPUTextureFormat& attachment);
        const WGPUTextureFormat* data();
};

class WebGPUVectorRenderBundle {
    private:
        std::vector<WebGPURenderBundle> vector;
    public:
        static WebGPUVectorRenderBundle Obtain();
        int size();
        void push_back(const WebGPURenderBundle& attachment);
        const WebGPURenderBundle* data();
};

class WebGPUVectorRenderPassColorAttachment {
    private:
        std::vector<WebGPURenderPassColorAttachment> vector;
    public:
        static WebGPUVectorRenderPassColorAttachment Obtain();
        int size();
        void push_back(const WebGPURenderPassColorAttachment& attachment);
        const WebGPURenderPassColorAttachment* data();
};

class WebGPUVectorVertexAttribute {
    private:
        std::vector<WebGPUVertexAttribute> vector;
    public:
        static WebGPUVectorVertexAttribute Obtain();
        int size();
        void push_back(const WebGPUVertexAttribute& attribute);
        const WebGPUVertexAttribute* data();
};

class WebGPUVectorBindGroupLayout {
    private:
        std::vector<WebGPUBindGroupLayout> vector;
    public:
        static WebGPUVectorBindGroupLayout Obtain();
        int size();
        void push_back(const WebGPUBindGroupLayout& groupLayout);
        const WebGPUBindGroupLayout* data();
};

class WebGPUVectorBindGroupLayoutEntry {
    private:
        std::vector<WebGPUBindGroupLayoutEntry> vector;
    public:
        static WebGPUVectorBindGroupLayoutEntry Obtain();
        int size();
        void push_back(const WebGPUBindGroupLayoutEntry& entry);
        const WebGPUBindGroupLayoutEntry* data();
};

class WebGPUVectorInt {
    private:
        std::vector<int> vector;
    public:
        static WebGPUVectorInt Obtain();
        int size();
        void push_back(int attachment);
        int get(int index);
        const int* data();
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
        WebGPUStringView();
        WebGPUStringView(WGPUStringView stringView);
        WebGPUStringView(const char* value);
        const std::string GetString();
};

class WebGPUChainedStruct : public WebGPUObjectBase<WebGPUChainedStruct, WGPUChainedStruct*> {
    public:
        void SetNext(WebGPUChainedStruct* value);
        void SetSType(WGPUSType type);
};

class WebGPULimits : public WebGPUObjectBase<WebGPULimits, WGPULimits> {
    public:
        static WebGPULimits Obtain();
        void SetMaxTextureDimension1D(int value);
        int GetMaxTextureDimension1D();
        void SetMaxTextureDimension2D(int value);
        int GetMaxTextureDimension2D();
        void SetMaxTextureDimension3D(int value);
        int GetMaxTextureDimension3D();
        void SetMaxTextureArrayLayers(int value);
        int GetMaxTextureArrayLayers();
        void SetMaxBindGroups(int value);
        int GetMaxBindGroups();
        void SetMaxBindGroupsPlusVertexBuffers(int value);
        int GetMaxBindGroupsPlusVertexBuffers();
        void SetMaxBindingsPerBindGroup(int value);
        int GetMaxBindingsPerBindGroup();
        void SetMaxDynamicUniformBuffersPerPipelineLayout(int value);
        int GetMaxDynamicUniformBuffersPerPipelineLayout();
        void SetMaxDynamicStorageBuffersPerPipelineLayout(int value);
        int GetMaxDynamicStorageBuffersPerPipelineLayout();
        void SetMaxSampledTexturesPerShaderStage(int value);
        int GetMaxSampledTexturesPerShaderStage();
        void SetMaxSamplersPerShaderStage(int value);
        int GetMaxSamplersPerShaderStage();
        void SetMaxStorageBuffersPerShaderStage(int value);
        int GetMaxStorageBuffersPerShaderStage();
        void SetMaxStorageTexturesPerShaderStage(int value);
        int GetMaxStorageTexturesPerShaderStage();
        void SetMaxUniformBuffersPerShaderStage(int value);
        int GetMaxUniformBuffersPerShaderStage();
        void SetMaxUniformBufferBindingSize(int value);
        int GetMaxUniformBufferBindingSize();
        void SetMaxStorageBufferBindingSize(int value);
        int GetMaxStorageBufferBindingSize();
        void SetMinUniformBufferOffsetAlignment(int value);
        int GetMinUniformBufferOffsetAlignment();
        void SetMinStorageBufferOffsetAlignment(int value);
        int GetMinStorageBufferOffsetAlignment();
        void SetMaxVertexBuffers(int value);
        int GetMaxVertexBuffers();
        void SetMaxBufferSize(int value);
        int GetMaxBufferSize();
        void SetMaxVertexAttributes(int value);
        int GetMaxVertexAttributes();
        void SetMaxVertexBufferArrayStride(int value);
        int GetMaxVertexBufferArrayStride();
        void SetMaxInterStageShaderVariables(int value);
        int GetMaxInterStageShaderVariables();
        void SetMaxColorAttachments(int value);
        int GetMaxColorAttachments();
        void SetMaxColorAttachmentBytesPerSample(int value);
        int GetMaxColorAttachmentBytesPerSample();
        void SetMaxComputeWorkgroupStorageSize(int value);
        int GetMaxComputeWorkgroupStorageSize();
        void SetMaxComputeInvocationsPerWorkgroup(int value);
        int GetMaxComputeInvocationsPerWorkgroup();
        void SetMaxComputeWorkgroupSizeX(int value);
        int GetMaxComputeWorkgroupSizeX();
        void SetMaxComputeWorkgroupSizeY(int value);
        int GetMaxComputeWorkgroupSizeY();
        void SetMaxComputeWorkgroupSizeZ(int value);
        int GetMaxComputeWorkgroupSizeZ();
        void SetMaxComputeWorkgroupsPerDimension(int value);
        int GetMaxComputeWorkgroupsPerDimension();
};

class WebGPUBufferBindingLayout : public WebGPUObjectBase<WebGPUBufferBindingLayout, WGPUBufferBindingLayout> {
    public:
        static WebGPUBufferBindingLayout Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetType(WGPUBufferBindingType type);
        void SetHasDynamicOffset(int hasDynamicOffset);
        void SetMinBindingSize(int minBindingSize);
};

class WebGPUSamplerBindingLayout : public WebGPUObjectBase<WebGPUSamplerBindingLayout, WGPUSamplerBindingLayout> {
    public:
        static WebGPUSamplerBindingLayout Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetType(WGPUSamplerBindingType type);
};

class WebGPUTextureBindingLayout : public WebGPUObjectBase<WebGPUTextureBindingLayout, WGPUTextureBindingLayout> {
    public:
        static WebGPUTextureBindingLayout Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetSampleType(WGPUTextureSampleType sampleType);
        void SetViewDimension(WGPUTextureViewDimension viewDimension);
        void SetMultisampled(int multisampled);
};

class WebGPUStorageTextureBindingLayout : public WebGPUObjectBase<WebGPUStorageTextureBindingLayout, WGPUStorageTextureBindingLayout> {
    public:
        static WebGPUStorageTextureBindingLayout Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetAccess(WGPUStorageTextureAccess access);
        void SetFormat(WGPUTextureFormat format);
        void SetViewDimension(WGPUTextureViewDimension viewDimension);
};

class WebGPUBindGroupLayoutEntry : public WebGPUObjectBase<WebGPUBindGroupLayoutEntry, WGPUBindGroupLayoutEntry> {
    public:
        static WebGPUBindGroupLayoutEntry Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetBinding(int binding);
        void SetVisibility(WGPUShaderStage visibility);
        void SetBuffer(WebGPUBufferBindingLayout* buffer);
        void SetSampler(WebGPUSamplerBindingLayout* sampler);
        void SetTexture(WebGPUTextureBindingLayout* texture);
        void SetStorageTexture(WebGPUStorageTextureBindingLayout* storageTexture);
};

class WebGPURequestAdapterOptions : public WebGPUObjectBase<WebGPURequestAdapterOptions, WGPURequestAdapterOptions> {
    public:
        static WebGPURequestAdapterOptions Obtain();
};

class WebGPUAdapterInfo : public WebGPUObjectBase<WebGPUAdapterInfo, WGPUAdapterInfo> {
    public:
        static WebGPUAdapterInfo Obtain();
        std::string GetVendor();
        std::string GetArchitecture();
        std::string GetDevice();
        std::string GetDescription();
        WGPUBackendType GetBackendType();
        WGPUAdapterType GetAdapterType();
};

class WebGPUConstantEntry : public WebGPUObjectBase<WebGPUConstantEntry, WGPUConstantEntry> {
    public:
};

class WebGPUVertexAttribute : public WebGPUObjectBase<WebGPUVertexAttribute, WGPUVertexAttribute> {
    public:
        static WebGPUVertexAttribute Obtain();
        void SetFormat(WGPUVertexFormat format);
        void SetOffset(int offset);
        void SetShaderLocation(int shaderLocation);
};

class WebGPUVertexBufferLayout : public WebGPUObjectBase<WebGPUVertexBufferLayout, WGPUVertexBufferLayout> {
    public:
        static WebGPUVertexBufferLayout Obtain();
        void SetAttributes(WebGPUVectorVertexAttribute* values);
        void SetArrayStride(int offset);
        void SetStepMode(WGPUVertexStepMode stepMode);
};

class WebGPUVertexState : public WebGPUObjectBase<WebGPUVertexState, WGPUVertexState*> {
    public:
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetModule(WebGPUShaderModule* shaderModule);
        void SetEntryPoint(const char* value);
        void SetConstants(WebGPUVectorConstantEntry* values);
        void SetBuffers(WebGPUVectorVertexBufferLayout* values);
};

class WebGPUShaderSourceWGSL : public WebGPUObjectBase<WebGPUShaderSourceWGSL, WGPUShaderSourceWGSL> {
    public:
        static WebGPUShaderSourceWGSL Obtain();
        void SetCode(const char* value);
        void SetNext(WebGPUChainedStruct* value);
        void SetSType(WGPUSType type);
        WebGPUChainedStruct GetChain();
};

class WebGPUBlendComponent : public WebGPUObjectBase<WebGPUBlendComponent, WGPUBlendComponent*> {
    public:
        void SetOperation(WGPUBlendOperation operation);
        WGPUBlendOperation GetOperation();
        void SetSrcFactor(WGPUBlendFactor factor);
        WGPUBlendFactor GetSrcFactor();
        void SetDstFactor(WGPUBlendFactor factor);
        WGPUBlendFactor GetDstFactor();
};

class WebGPUBlendState : public WebGPUObjectBase<WebGPUBlendState, WGPUBlendState> {
    public:
        static WebGPUBlendState Obtain();
        WebGPUBlendComponent GetColor();
        WebGPUBlendComponent GetAlpha();
};

class WebGPUColorTargetState : public WebGPUObjectBase<WebGPUColorTargetState, WGPUColorTargetState> {
    public:
        static WebGPUColorTargetState Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetFormat(WGPUTextureFormat format);
        void SetBlend(WebGPUBlendState* blendState);
        void SetWriteMask(WGPUColorWriteMask writeMask);
};

class WebGPUFragmentState : public WebGPUObjectBase<WebGPUFragmentState, WGPUFragmentState> {
    public:
        static WebGPUFragmentState Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetEntryPoint(const char* value);
        void SetTargets(WebGPUVectorColorTargetState* values);
        void SetModule(WebGPUShaderModule* shaderModule);
        void SetConstants(WebGPUVectorConstantEntry* values);
};

class WebGPUPrimitiveState : public WebGPUObjectBase<WebGPUPrimitiveState, WGPUPrimitiveState*> {
    public:
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetTopology(WGPUPrimitiveTopology value);
        void SetStripIndexFormat(WGPUIndexFormat value);
        void SetFrontFace(WGPUFrontFace value);
        void SetCullMode(WGPUCullMode value);
};

class WebGPUStencilFaceState : public WebGPUObjectBase<WebGPUStencilFaceState, WGPUStencilFaceState*> {
    public:
        void SetCompare(WGPUCompareFunction compare);
        void SetFailOp(WGPUStencilOperation failOp);
        void SetDepthFailOp(WGPUStencilOperation depthFailOp);
        void SetPassOp(WGPUStencilOperation passOp);
};

class WebGPUDepthStencilState : public WebGPUObjectBase<WebGPUDepthStencilState, WGPUDepthStencilState> {
    public:
        static WebGPUDepthStencilState Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetFormat(WGPUTextureFormat format);
        void SetDepthWriteEnabled(WGPUOptionalBool depthWriteEnabled);
        void SetDepthCompare(WGPUCompareFunction depthCompare);
        void SetDepthBiasSlopeScale(float depthBiasSlopeScale);
        void SetDepthBiasClamp(float depthBiasClamp);
        void SetStencilReadMask(int stencilReadMask);
        void SetStencilWriteMask(int stencilWriteMask);
        void SetDepthBias(int depthBias);
        WebGPUStencilFaceState GetStencilFront();
        WebGPUStencilFaceState GetStencilBack();
};

class WebGPUMultisampleState : public WebGPUObjectBase<WebGPUMultisampleState, WGPUMultisampleState*> {
    public:
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetCount(int count);
        void SetMask(int mask);
        void SetAlphaToCoverageEnabled(int alphaToCoverageEnabled);
};

class WebGPUColor : public WebGPUObjectBase<WebGPUColor, WGPUColor*> {
    private:
        bool deleteObject;
    public:
        WebGPUColor();
        WebGPUColor(WGPUColor* color);
        ~WebGPUColor();
        static WebGPUColor Obtain();
        void SetColor(float r, float g, float b, float a);
        float GetR();
        float GetG();
        float GetB();
        float GetA();
};

class WebGPUSupportedFeatures : public WebGPUObjectBase<WebGPUSupportedFeatures, WGPUSupportedFeatures> {
    public:
        static WebGPUSupportedFeatures Obtain();
        int GetFeatureCount();
        WGPUFeatureName GetFeatureAt(long index);
};

class WebGPURenderPassDepthStencilAttachment : public WebGPUObjectBase<WebGPURenderPassDepthStencilAttachment, WGPURenderPassDepthStencilAttachment> {
    public:
        static WebGPURenderPassDepthStencilAttachment Obtain();
};

class WebGPURenderPassTimestampWrites : public WebGPUObjectBase<WebGPURenderPassTimestampWrites, WGPURenderPassTimestampWrites> {
    public:
        static WebGPURenderPassTimestampWrites Obtain();
        void SetQuerySet(WebGPUQuerySet* value);
        void SetBeginningOfPassWriteIndex(int value);
        void SetEndOfPassWriteIndex(int value);
};

class WebGPUComputePassTimestampWrites : public WebGPUObjectBase<WebGPUComputePassTimestampWrites, WGPUComputePassTimestampWrites> {
    public:
        static WebGPUComputePassTimestampWrites Obtain();
        void SetQuerySet(WebGPUQuerySet* value);
        void SetBeginningOfPassWriteIndex(int value);
        void SetEndOfPassWriteIndex(int value);
};

class WebGPUOrigin3D : public WebGPUObjectBase<WebGPUOrigin3D, WGPUOrigin3D> {
    public:
        static WebGPUOrigin3D Obtain();
        void Set(int x, int y, int z);
        void SetX(int value);
        void SetY(int value);
        void SetZ(int value);
};

class WebGPUTexelCopyTextureInfo : public WebGPUObjectBase<WebGPUTexelCopyTextureInfo, WGPUTexelCopyTextureInfo> {
    public:
        static WebGPUTexelCopyTextureInfo Obtain();
        void SetTexture(WebGPUTexture* texture);
        void SetMipLevel(int mipLevel);
        void SetOrigin(WebGPUOrigin3D origin);
        void SetAspect(WGPUTextureAspect aspect);
};

class WebGPUExtent3D : public WebGPUObjectBase<WebGPUExtent3D, WGPUExtent3D> {
    public:
        static WebGPUExtent3D Obtain();
        void SetWidth(int width);
        void SetHeight(int height);
        void SetDepthOrArrayLayers(int depthOrArrayLayers);
};

class WebGPUTexelCopyBufferLayout : public WebGPUObjectBase<WebGPUTexelCopyBufferLayout, WGPUTexelCopyBufferLayout*> {
    public:
        static WebGPUTexelCopyBufferLayout Obtain();
        void SetOffset(int offset);
        void SetBytesPerRow(int bytesPerRow);
        void SetRowsPerImage(int rowsPerImage);
};

class WebGPUTexelCopyBufferInfo : public WebGPUObjectBase<WebGPUTexelCopyBufferInfo, WGPUTexelCopyBufferInfo> {
    public:
        static WebGPUTexelCopyBufferInfo Obtain();
        WebGPUTexelCopyBufferLayout GetLayout();
        WebGPUBuffer GetBuffer();
};

class WebGPUSurfaceConfiguration : public WebGPUObjectBase<WebGPUSurfaceConfiguration, WGPUSurfaceConfiguration> {
    public:
        static WebGPUSurfaceConfiguration Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetWidth(long width);
        void SetHeight(long height);
        void SetFormat(WGPUTextureFormat format);
        void SetViewFormatCount(long value);
        void SetViewFormats(WebGPUVectorTextureFormat* values);
        void SetUsage(WGPUTextureUsage usage);
        void SetDevice(WebGPUDevice* device);
        void SetPresentMode(WGPUPresentMode presentMode);
        void SetAlphaMode(WGPUCompositeAlphaMode alphaMode);
};

class WebGPUSurfaceCapabilities : public WebGPUObjectBase<WebGPUSurfaceCapabilities, WGPUSurfaceCapabilities> {
    public:
        static WebGPUSurfaceCapabilities Obtain();
        WGPUTextureFormat GetFormats(int index);
};

class WebGPURenderPassColorAttachment : public WebGPUObjectBase<WebGPURenderPassColorAttachment, WGPURenderPassColorAttachment> {
    public:
        WebGPURenderPassColorAttachment();
        static WebGPURenderPassColorAttachment Obtain();
        void SetResolveTarget(WebGPUTextureView* textureView);
        void SetView(WebGPUTextureView* textureView);
        void SetLoadOp(WGPULoadOp loadOp);
        void SetStoreOp(WGPUStoreOp storeOp);
        void SetDepthSlice(int depthSlice);
        WebGPUColor GetClearValue();
};

class WebGPUSurfaceTexture : public WebGPUObjectBase<WebGPUSurfaceTexture, WGPUSurfaceTexture> {
    public:
        static WebGPUSurfaceTexture Obtain();
        void GetTexture(WebGPUTexture* tex);
        WGPUSurfaceGetCurrentTextureStatus GetStatus();
};

// ################################### DESCRIPTOR STRUCTS ###################################

class WebGPUBindGroupLayoutDescriptor : public WebGPUObjectBase<WebGPUBindGroupLayoutDescriptor, WGPUBindGroupLayoutDescriptor> {
    public:
        static WebGPUBindGroupLayoutDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetEntries(WebGPUVectorBindGroupLayoutEntry* entries);
};

class WebGPUProgrammableStageDescriptor : public WebGPUObjectBase<WebGPUProgrammableStageDescriptor, WGPUProgrammableStageDescriptor*> {
    public:
        static WebGPUProgrammableStageDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
};

class WebGPUComputePipelineDescriptor : public WebGPUObjectBase<WebGPUComputePipelineDescriptor, WGPUComputePipelineDescriptor> {
    public:
        static WebGPUComputePipelineDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        WebGPUProgrammableStageDescriptor GetCompute();
};

class WebGPUQueueDescriptor : public WebGPUObjectBase<WebGPUQueueDescriptor, WGPUQueueDescriptor*> {
    public:
        static WebGPUQueueDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
};

class WebGPUBufferDescriptor : public WebGPUObjectBase<WebGPUBufferDescriptor, WGPUBufferDescriptor> {
    public:
        static WebGPUBufferDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetUsage(WGPUBufferUsage usage);
        void SetSize(int size);
        void SetMappedAtCreation(int mappedAtCreation);
};

class WebGPUBindGroupDescriptor : public WebGPUObjectBase<WebGPUBindGroupDescriptor, WGPUBindGroupDescriptor> {
    public:
        static WebGPUBindGroupDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetLayout(WebGPUBindGroupLayout* layout);
};

class WebGPUPipelineLayoutDescriptor : public WebGPUObjectBase<WebGPUPipelineLayoutDescriptor, WGPUPipelineLayoutDescriptor> {
    public:
        static WebGPUPipelineLayoutDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetBindGroupLayouts(WebGPUVectorBindGroupLayout* bindGroupLayouts);
};

class WebGPUDeviceDescriptor : public WebGPUObjectBase<WebGPUDeviceDescriptor, WGPUDeviceDescriptor> {
    public:
        static WebGPUDeviceDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetRequiredLimits(WebGPULimits* limits);
        void SetRequiredFeatures(WebGPUVectorFeatureName* features);
        WebGPUQueueDescriptor GetDefaultQueue();
};

class WebGPUShaderModuleDescriptor : public WebGPUObjectBase<WebGPUShaderModuleDescriptor, WGPUShaderModuleDescriptor> {
    public:
        static WebGPUShaderModuleDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
};

class WebGPURenderPipelineDescriptor : public WebGPUObjectBase<WebGPURenderPipelineDescriptor, WGPURenderPipelineDescriptor> {
    public:
        static WebGPURenderPipelineDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        WebGPUVertexState GetVertex();
        WebGPUPrimitiveState GetPrimitive();
        void SetFragment(WebGPUFragmentState* fragment);
        void SetDepthStencil(WebGPUDepthStencilState* depthStencilState);
        WebGPUMultisampleState GetMultisample();
        void SetLayout(WebGPUPipelineLayout* pipelineLayout);
};

class WebGPUCommandEncoderDescriptor : public WebGPUObjectBase<WebGPUCommandEncoderDescriptor, WGPUCommandEncoderDescriptor> {
    public:
        static WebGPUCommandEncoderDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
};

class WebGPUCommandBufferDescriptor : public WebGPUObjectBase<WebGPUCommandBufferDescriptor, WGPUCommandBufferDescriptor> {
    public:
        static WebGPUCommandBufferDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
};

class WebGPURenderPassDescriptor : public WebGPUObjectBase<WebGPURenderPassDescriptor, WGPURenderPassDescriptor> {
    public:
        static WebGPURenderPassDescriptor Obtain();
        void SetLabel(const char* value);
        void SetColorAttachments(WebGPUVectorRenderPassColorAttachment* values);
        void SetDepthStencilAttachment(WebGPURenderPassDepthStencilAttachment* attachment);
        void SetTimestampWrites(WebGPURenderPassTimestampWrites* timestampWrites);
};

class WebGPUComputePassDescriptor : public WebGPUObjectBase<WebGPUComputePassDescriptor, WGPUComputePassDescriptor> {
    public:
        static WebGPUComputePassDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetTimestampWrites(WebGPUComputePassTimestampWrites* timestampWrites);
};

class WebGPUTextureViewDescriptor : public WebGPUObjectBase<WebGPUTextureViewDescriptor, WGPUTextureViewDescriptor> {
    public:
        static WebGPUTextureViewDescriptor Obtain();
        void SetLabel(const char* value);
        void SetFormat(WGPUTextureFormat format);
        void SetDimension(WGPUTextureViewDimension dimension);
        void SetBaseMipLevel(int baseMipLevel);
        void SetMipLevelCount(int mipLevelCount);
        void SetBaseArrayLayer(int baseArrayLayer);
        void SetArrayLayerCount(int arrayLayerCount);
        void SetAspect(WGPUTextureAspect aspect);
        void SetUsage(WGPUTextureUsage usage);
};

// ################################### OPAQUE POINTER ###################################

class WebGPUTextureView : public WebGPUObjectBase<WebGPUTextureView, WGPUTextureView> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WebGPUTextureView Obtain();
};

class WebGPUTexture : public WebGPUObjectBase<WebGPUTexture, WGPUTexture> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WebGPUTexture Obtain();
        void CreateView(WebGPUTextureViewDescriptor* textureViewDescriptor, WebGPUTextureView* textureView);
        WGPUTextureFormat GetFormat();
};

class WebGPUShaderModule : public WebGPUObjectBase<WebGPUShaderModule, WGPUShaderModule> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WebGPUShaderModule Obtain();
};

class WebGPURenderPipeline : public WebGPUObjectBase<WebGPURenderPipeline, WGPURenderPipeline> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WebGPURenderPipeline Obtain();
};

class WebGPURenderPassEncoder : public WebGPUObjectBase<WebGPURenderPassEncoder, WGPURenderPassEncoder> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WebGPURenderPassEncoder Obtain();
        void End();
        void SetPipeline(WebGPURenderPipeline* renderPipeline);
        void BeginOcclusionQuery(int queryIndex);
        void Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance);
        void DrawIndexed(int indexCount, int instanceCount, int firstIndex, int baseVertex, int firstInstance);
        void DrawIndexedIndirect(WebGPUBuffer* indirectBuffer, int indirectOffset);
        void DrawIndirect(WebGPUBuffer* indirectBuffer, int indirectOffset);
        void EndOcclusionQuery();
        void ExecuteBundles(WebGPUVectorRenderBundle* values);
        void InsertDebugMarker(const char* label);
        void PopDebugGroup();
        void PushDebugGroup(const char* label);
        void SetBindGroup(int groupIndex, WebGPUBindGroup* group, WebGPUVectorInt* dynamicOffsets);
        void SetBlendConstant(WebGPUColor* color);
        void SetIndexBuffer(WebGPUBuffer* buffer, WGPUIndexFormat format, int offset, int size);
        void SetLabel(const char* label);
        void SetScissorRect(int x, int y, int width, int height);
        void SetStencilReference(int reference);
        void SetVertexBuffer(int slot, WebGPUBuffer* buffer, int offset, int size);
        void SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth);
};

class WebGPUQuerySet : public WebGPUObjectBase<WebGPUQuerySet, WGPUQuerySet> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetDestroy();
        int GetCount();
        WGPUQueryType GetType();
        void SetLabel(const char* label);
};

class WebGPUPipelineLayout : public WebGPUObjectBase<WebGPUPipelineLayout, WGPUPipelineLayout> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
};

class WebGPUInstance : public WebGPUObjectBase<WebGPUInstance, WGPUInstance> {
    private:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        bool IsValid();
        void RequestAdapter(WebGPURequestAdapterOptions* options, WGPUCallbackMode mode, RequestAdapterCallback* callback);
        WebGPUSurface* CreateWebSurface(const char* canvas);
        WebGPUSurface* CreateWindowsSurface(void * hwnd);
        WebGPUSurface* CreateLinuxSurface(bool isWayland, void * windowOrSurface, void* display);
        WebGPUSurface* CreateMacSurface(void * windowHandle);
        WebGPUSurface* CreateAndroidSurface(WGPUAndroidWindow* window);
        void ProcessEvents();
};

class WebGPUDevice : public WebGPUObjectBase<WebGPUDevice, WGPUDevice> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void CreateBindGroup(WebGPUBindGroupDescriptor* descriptor, WebGPUBindGroup* valueOut);
        void CreateBindGroupLayout(WebGPUBindGroupLayoutDescriptor* descriptor, WebGPUBindGroupLayout* valueOut);
        void CreateBuffer(WebGPUBufferDescriptor* descriptor, WebGPUBuffer* valueOut);
        void CreateCommandEncoder(WebGPUCommandEncoderDescriptor* descriptor, WebGPUCommandEncoder* valueOut);
        void CreateComputePipeline(WebGPUComputePipelineDescriptor* descriptor, WebGPUComputePipeline* valueOut);
        WebGPUQueue GetQueue();
        void CreateRenderPipeline(WebGPURenderPipelineDescriptor* pipelineDescriptor, WebGPURenderPipeline* valueOut);
        void CreateShaderModule(WebGPUShaderModuleDescriptor* shaderModuleDescriptor, WebGPUShaderModule* valueOut);
        void GetFeatures(WebGPUSupportedFeatures* features);
        void GetLimits(WebGPULimits* limits);
};

class WebGPUComputePassEncoder : public WebGPUObjectBase<WebGPUComputePassEncoder, WGPUComputePassEncoder> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WebGPUComputePassEncoder Obtain();
        void SetDispatchWorkgroups(int workgroupCountX, int workgroupCountY, int workgroupCountZ);
        void DispatchWorkgroupsIndirect(WebGPUBuffer* indirectBuffer, int indirectOffset);
        void End();
        void InsertDebugMarker(const char* markerLabel);
        void PopDebugGroup();
        void PushDebugGroup(const char* groupLabel);
        void SetBindGroup(int groupIndex, WebGPUBindGroup* group, WebGPUVectorInt* offsets);
        void SetLabel(const char* label);
        void SetPipeline(WebGPUComputePipeline* pipeline);
};

class WebGPUCommandBuffer : public WebGPUObjectBase<WebGPUCommandBuffer, WGPUCommandBuffer> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WebGPUCommandBuffer Obtain();
};

class WebGPUCommandEncoder : public WebGPUObjectBase<WebGPUCommandEncoder, WGPUCommandEncoder> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WebGPUCommandEncoder Obtain();
        void BeginComputePass(WebGPUComputePassDescriptor* descriptor, WebGPUComputePassEncoder* encoder);
        void BeginRenderPass(WebGPURenderPassDescriptor* renderPassDescriptor, WebGPURenderPassEncoder* encoder);
        void ClearBuffer(WebGPUBuffer* buffer, int offset, int size);
        void CopyBufferToBuffer(WebGPUBuffer* source, int sourceOffset, WebGPUBuffer* destination, int destinationOffset, int size);
        void CopyBufferToTexture(WebGPUTexelCopyBufferInfo* source, WebGPUTexelCopyTextureInfo* destination, WebGPUExtent3D* copySize);
        void TextureToBuffer(WebGPUTexelCopyTextureInfo* source, WebGPUTexelCopyBufferInfo* destination, WebGPUExtent3D* copySize);
        void CopyTextureToTexture(WebGPUTexelCopyTextureInfo* source, WebGPUTexelCopyTextureInfo* destination, WebGPUExtent3D* copySize);
        void Finish(WebGPUCommandBufferDescriptor* commandBufferDescriptor, WebGPUCommandBuffer* commandBuffer);
        void InsertDebugMarker(const char* markerLabel);
        void PopDebugGroup();
        void PushDebugGroup(const char* groupLabel);
        void ResolveQuerySet(WebGPUQuerySet* querySet, int firstQuery, int queryCount, WebGPUBuffer* destination, int destinationOffset);
        void SetLabel(const char* label);
        void WriteTimestamp(WebGPUQuerySet* querySet, int queryIndex);
};

class WebGPUBuffer : public WebGPUObjectBase<WebGPUBuffer, WGPUBuffer> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void Unmap();
        int GetSize();
        WGPUBufferUsage GetUsage();
        void Destroy();
};

class WebGPUBindGroup : public WebGPUObjectBase<WebGPUBindGroup, WGPUBindGroup> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
};

class WebGPUBindGroupLayout : public WebGPUObjectBase<WebGPUBindGroupLayout, WGPUBindGroupLayout> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
};

class WebGPUComputePipeline : public WebGPUObjectBase<WebGPUComputePipeline, WGPUComputePipeline> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
        WebGPUBindGroupLayout GetBindGroupLayout(int groupIndex);
};

class WebGPURenderBundle : public WebGPUObjectBase<WebGPURenderBundle, WGPURenderBundle> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
};

class WebGPUAdapter : public WebGPUObjectBase<WebGPUAdapter, WGPUAdapter> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void RequestDevice(WebGPUDeviceDescriptor* descriptor, WGPUCallbackMode mode, RequestDeviceCallback* callback, UncapturedErrorCallback* errorCallback);
        bool GetInfo(WebGPUAdapterInfo* adapterInfo);
        bool HasFeature(WGPUFeatureName featureName);
};

class WebGPUSurface : public WebGPUObjectBase<WebGPUSurface, WGPUSurface> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void Unconfigure();
        void Configure(WebGPUSurfaceConfiguration* config);
        void GetCapabilities(WebGPUAdapter* adapter, WebGPUSurfaceCapabilities* surfaceCapabilities);
        void GetCurrentTexture(WebGPUSurfaceTexture* tex);
        void Present();
};

class WebGPUQueue : public WebGPUObjectBase<WebGPUQueue, WGPUQueue> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
        void Submit(int commandCount, WebGPUCommandBuffer* commandBuffer);
        void WriteBuffer(WebGPUBuffer* buffer, int bufferOffset, WGPUByteBuffer* bytes);
};