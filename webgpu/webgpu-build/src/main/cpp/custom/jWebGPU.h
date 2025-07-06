#pragma once

#include "IDLHelper.h"

// WGPUBufferUsage macros
#define WGPUBufferUsage JWGPUBufferUsage
#define WGPUBufferUsage_None JWGPUBufferUsage_None
#define WGPUBufferUsage_MapRead JWGPUBufferUsage_MapRead
#define WGPUBufferUsage_MapWrite JWGPUBufferUsage_MapWrite
#define WGPUBufferUsage_CopySrc JWGPUBufferUsage_CopySrc
#define WGPUBufferUsage_CopyDst JWGPUBufferUsage_CopyDst
#define WGPUBufferUsage_Index JWGPUBufferUsage_Index
#define WGPUBufferUsage_Vertex JWGPUBufferUsage_Vertex
#define WGPUBufferUsage_Uniform JWGPUBufferUsage_Uniform
#define WGPUBufferUsage_Storage JWGPUBufferUsage_Storage
#define WGPUBufferUsage_Indirect JWGPUBufferUsage_Indirect
#define WGPUBufferUsage_QueryResolve JWGPUBufferUsage_QueryResolve

// WGPUColorWriteMask macros
#define WGPUColorWriteMask JWGPUColorWriteMask
#define WGPUColorWriteMask_None JWGPUColorWriteMask_None
#define WGPUColorWriteMask_Red JWGPUColorWriteMask_Red
#define WGPUColorWriteMask_Green JWGPUColorWriteMask_Green
#define WGPUColorWriteMask_Blue JWGPUColorWriteMask_Blue
#define WGPUColorWriteMask_Alpha JWGPUColorWriteMask_Alpha
#define WGPUColorWriteMask_All JWGPUColorWriteMask_All

// WGPUMapMode macros (already present, included for completeness)
#define WGPUMapMode JWGPUMapMode
#define WGPUMapMode_None JWGPUMapMode_None
#define WGPUMapMode_Read JWGPUMapMode_Read
#define WGPUMapMode_Write JWGPUMapMode_Write

// WGPUShaderStage macros
#define WGPUShaderStage JWGPUShaderStage
#define WGPUShaderStage_None JWGPUShaderStage_None
#define WGPUShaderStage_Vertex JWGPUShaderStage_Vertex
#define WGPUShaderStage_Fragment JWGPUShaderStage_Fragment
#define WGPUShaderStage_Compute JWGPUShaderStage_Compute

// WGPUTextureUsage macros
#define WGPUTextureUsage JWGPUTextureUsage
#define WGPUTextureUsage_None JWGPUTextureUsage_None
#define WGPUTextureUsage_CopySrc JWGPUTextureUsage_CopySrc
#define WGPUTextureUsage_CopyDst JWGPUTextureUsage_CopyDst
#define WGPUTextureUsage_TextureBinding JWGPUTextureUsage_TextureBinding
#define WGPUTextureUsage_StorageBinding JWGPUTextureUsage_StorageBinding
#define WGPUTextureUsage_RenderAttachment JWGPUTextureUsage_RenderAttachment

#include "webgpu/webgpu.h"

// Undefine WGPUBufferUsage macros
#undef WGPUBufferUsage
#undef WGPUBufferUsage_None
#undef WGPUBufferUsage_MapRead
#undef WGPUBufferUsage_MapWrite
#undef WGPUBufferUsage_CopySrc
#undef WGPUBufferUsage_CopyDst
#undef WGPUBufferUsage_Index
#undef WGPUBufferUsage_Vertex
#undef WGPUBufferUsage_Uniform
#undef WGPUBufferUsage_Storage
#undef WGPUBufferUsage_Indirect
#undef WGPUBufferUsage_QueryResolve

// Undefine WGPUColorWriteMask macros
#undef WGPUColorWriteMask
#undef WGPUColorWriteMask_None
#undef WGPUColorWriteMask_Red
#undef WGPUColorWriteMask_Green
#undef WGPUColorWriteMask_Blue
#undef WGPUColorWriteMask_Alpha
#undef WGPUColorWriteMask_All

// Undefine WGPUMapMode macros (already present, included for completeness)
#undef WGPUMapMode
#undef WGPUMapMode_None
#undef WGPUMapMode_Read
#undef WGPUMapMode_Write

// Undefine WGPUShaderStage macros
#undef WGPUShaderStage
#undef WGPUShaderStage_None
#undef WGPUShaderStage_Vertex
#undef WGPUShaderStage_Fragment
#undef WGPUShaderStage_Compute

// Undefine WGPUTextureUsage macros
#undef WGPUTextureUsage
#undef WGPUTextureUsage_None
#undef WGPUTextureUsage_CopySrc
#undef WGPUTextureUsage_CopyDst
#undef WGPUTextureUsage_TextureBinding
#undef WGPUTextureUsage_StorageBinding
#undef WGPUTextureUsage_RenderAttachment

#include <iostream>
#include <cstring>
#include <memory>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif // __EMSCRIPTEN__

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
class WebGPURenderPipeline;
class WebGPUSampler;
class WebGPUCompilationMessage;
class WebGPUBindGroupEntry;
class WebGPUFuture;
class WebGPUFutureWaitInfo;
class WebGPUInstanceDescriptor;

class STBImage;

class WGPUByteBuffer;

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

// Define 32-bit enums
enum WGPUBufferUsage : uint32_t {
    WGPUBufferUsage_None = static_cast<uint32_t>(JWGPUBufferUsage_None),
    WGPUBufferUsage_MapRead = static_cast<uint32_t>(JWGPUBufferUsage_MapRead),
    WGPUBufferUsage_MapWrite = static_cast<uint32_t>(JWGPUBufferUsage_MapWrite),
    WGPUBufferUsage_CopySrc = static_cast<uint32_t>(JWGPUBufferUsage_CopySrc),
    WGPUBufferUsage_CopyDst = static_cast<uint32_t>(JWGPUBufferUsage_CopyDst),
    WGPUBufferUsage_Index = static_cast<uint32_t>(JWGPUBufferUsage_Index),
    WGPUBufferUsage_Vertex = static_cast<uint32_t>(JWGPUBufferUsage_Vertex),
    WGPUBufferUsage_Uniform = static_cast<uint32_t>(JWGPUBufferUsage_Uniform),
    WGPUBufferUsage_Storage = static_cast<uint32_t>(JWGPUBufferUsage_Storage),
    WGPUBufferUsage_Indirect = static_cast<uint32_t>(JWGPUBufferUsage_Indirect),
    WGPUBufferUsage_QueryResolve = static_cast<uint32_t>(JWGPUBufferUsage_QueryResolve)
};

enum WGPUColorWriteMask : uint32_t {
    WGPUColorWriteMask_None = static_cast<uint32_t>(JWGPUColorWriteMask_None),
    WGPUColorWriteMask_Red = static_cast<uint32_t>(JWGPUColorWriteMask_Red),
    WGPUColorWriteMask_Green = static_cast<uint32_t>(JWGPUColorWriteMask_Green),
    WGPUColorWriteMask_Blue = static_cast<uint32_t>(JWGPUColorWriteMask_Blue),
    WGPUColorWriteMask_Alpha = static_cast<uint32_t>(JWGPUColorWriteMask_Alpha),
    WGPUColorWriteMask_All = static_cast<uint32_t>(JWGPUColorWriteMask_All)
};

enum WGPUMapMode : uint32_t {
    WGPUMapMode_None = static_cast<uint32_t>(JWGPUMapMode_None),
    WGPUMapMode_Read = static_cast<uint32_t>(JWGPUMapMode_Read),
    WGPUMapMode_Write = static_cast<uint32_t>(JWGPUMapMode_Write)
};

enum WGPUShaderStage : uint32_t {
    WGPUShaderStage_None = static_cast<uint32_t>(JWGPUShaderStage_None),
    WGPUShaderStage_Vertex = static_cast<uint32_t>(JWGPUShaderStage_Vertex),
    WGPUShaderStage_Fragment = static_cast<uint32_t>(JWGPUShaderStage_Fragment),
    WGPUShaderStage_Compute = static_cast<uint32_t>(JWGPUShaderStage_Compute)
};

enum WGPUTextureUsage : uint32_t {
    WGPUTextureUsage_None = static_cast<uint32_t>(JWGPUTextureUsage_None),
    WGPUTextureUsage_CopySrc = static_cast<uint32_t>(JWGPUTextureUsage_CopySrc),
    WGPUTextureUsage_CopyDst = static_cast<uint32_t>(JWGPUTextureUsage_CopyDst),
    WGPUTextureUsage_TextureBinding = static_cast<uint32_t>(JWGPUTextureUsage_TextureBinding),
    WGPUTextureUsage_StorageBinding = static_cast<uint32_t>(JWGPUTextureUsage_StorageBinding),
    WGPUTextureUsage_RenderAttachment = static_cast<uint32_t>(JWGPUTextureUsage_RenderAttachment)
};

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
        static WebGPUInstance CreateInstance(WebGPUInstanceDescriptor* descriptor = NULL);
        static STBImage* loadImage(WGPUByteBuffer* buffer, int desiredChannels = 0);
};

class WGPUFloatBuffer {
    public:
        WGPUByteBuffer* parent;
        WGPUFloatBuffer() : parent(NULL) {}
        WGPUByteBuffer& getByteBuffer();
        void put(int index, float value);
        void put(const float* values, int offset, int size);
        float get(int index);
        void put(float value);
        float get();
        int remaining() const;
        int getCapacity();
        void position(int newPosition);
        int getPosition() const;
        void clear();
        void limit(int newLimit);
        int getLimit() const;
        void flip();
};

class WGPUIntBuffer {
    public:
        WGPUByteBuffer* parent;
        WGPUIntBuffer() : parent(NULL) {}
        WGPUByteBuffer& getByteBuffer();
        void put(int index, int value);
        void put(const int* values, int offset, int size);
        int get(int index);
        void put(int value);
        int get();
        int remaining() const;
        int getCapacity();
        void position(int newPosition);
        int getPosition() const;
        void clear();
        void limit(int newLimit);
        int getLimit() const;
        void flip();
};

class WGPULongBuffer {
    public:
        WGPUByteBuffer* parent;
        WGPULongBuffer() : parent(NULL) {}
        WGPUByteBuffer& getByteBuffer();
        void put(int index, long long value);
        void put(const long long* values, int offset, int size);
        long long get(int index);
        void put(long long value);
        long long get();
        int remaining() const;
        int getCapacity();
        void position(int newPosition);
        int getPosition() const;
        void clear();
        void limit(int newLimit);
        int getLimit() const;
        void flip();
};

class WGPUShortBuffer {
    public:
        WGPUByteBuffer* parent;
        WGPUShortBuffer() : parent(NULL) {}
        WGPUByteBuffer& getByteBuffer();
        void put(int index, int16_t value);
        int16_t get(int index);
        void put(int16_t value);
        void put(const int16_t* values, int offset, int size);
        int16_t get();
        int remaining() const;
        int getCapacity();
        void position(int newPosition);
        int getPosition() const;
        void clear();
        void limit(int newLimit);
        int getLimit() const;
        void flip();
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
        WGPUFloatBuffer floatBuffer;
        WGPUIntBuffer intBuffer;
        WGPULongBuffer longBuffer;
        WGPUShortBuffer shortBuffer;

    public:
        void push_back(char value);
        const uint8_t* data();
        template<typename T>
        void putNumeric(int index, T value);
        template<typename T>
        T getNumeric(int index);

    public:
        static WGPUByteBuffer Obtain(int capacity);
        WGPUByteBuffer() : buffer(0), _limit(0) {}
        WGPUByteBuffer(int capacity) : buffer(capacity), _limit(capacity) {}
        WGPUByteBuffer(uint8_t* bufferData, int dataSize) : _limit(dataSize) {
            buffer.insert(buffer.end(), bufferData, bufferData + dataSize);
        }
        void order(WGPUByteOrder order);
        void put(int index, char value);
        char get(int index);
        void put(char value);
        char get();
        void put(const uint8_t* values, int index, int size);
        int remaining() const;
        void position(int newPosition);
        int getPosition();
        void limit(int newLimit);
        size_t getLimit() const;
        int getCapacity();
        void clear();
        void flip();
        WGPUFloatBuffer& asFloatBuffer();
        WGPUIntBuffer& asIntBuffer();
        WGPULongBuffer& asLongBuffer();
        WGPUShortBuffer& asShortBuffer();

        friend class WGPUFloatBuffer;
        friend class WGPUIntBuffer;
        friend class WGPULongBuffer;
        friend class WGPUShortBuffer;
};

using WGPUByteOrder = WGPUByteBuffer::WGPUByteOrder;

class STBImage {
    public:
        uint32_t width;
        uint32_t height;
        uint32_t format;
        WGPUByteBuffer pixel;

        int GetWidth() { return width; }
        int GetHeight() { return height; }
        int GetFormat() { return format; }
        WGPUByteBuffer& GetPixels() { return pixel; }
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

class WGPUVectorFutureWaitInfo {
    private:
        std::vector<WebGPUFutureWaitInfo> vector;
    public:
        static WGPUVectorFutureWaitInfo Obtain();
        int size();
        void push_back(const WebGPUFutureWaitInfo& entry);
        const WebGPUFutureWaitInfo* data();
};

class WGPUVectorBindGroupEntry {
    private:
        std::vector<WebGPUBindGroupEntry> vector;
    public:
        static WGPUVectorBindGroupEntry Obtain();
        int size();
        void push_back(const WebGPUBindGroupEntry& entry);
        const WebGPUBindGroupEntry* data();
};

class WGPUVectorColorTargetState {
    private:
        std::vector<WebGPUColorTargetState> vector;
    public:
        static WGPUVectorColorTargetState Obtain();
        int size();
        void push_back(const WebGPUColorTargetState& attachment);
        const WebGPUColorTargetState* data();
};

class WGPUVectorFeatureName {
    private:
        std::vector<WGPUFeatureName> vector;
    public:
        static WGPUVectorFeatureName Obtain();
        int size();
        void push_back(const WGPUFeatureName& attachment);
        const WGPUFeatureName* data();
};

class WGPUVectorConstantEntry {
    private:
        std::vector<WebGPUConstantEntry> vector;
    public:
        static WGPUVectorConstantEntry Obtain();
        int size();
        void push_back(const WebGPUConstantEntry& attachment);
        const WebGPUConstantEntry* data();
};

class WGPUVectorVertexBufferLayout {
    private:
        std::vector<WebGPUVertexBufferLayout> vector;
    public:
        static WGPUVectorVertexBufferLayout Obtain();
        int size();
        void push_back(const WebGPUVertexBufferLayout& attachment);
        const WebGPUVertexBufferLayout* data();
};

class WGPUVectorTextureFormat {
    private:
        std::vector<WGPUTextureFormat> vector;
    public:
        static WGPUVectorTextureFormat Obtain();
        WGPUVectorTextureFormat() {}
        WGPUVectorTextureFormat(const WGPUTextureFormat* formats, size_t count) {
            vector.assign(formats, formats + count);
        }
        WGPUTextureFormat get(int index);
        int size();
        void push_back(const WGPUTextureFormat& attachment);
        const WGPUTextureFormat* data();
};

class WGPUVectorRenderBundle {
    private:
        std::vector<WebGPURenderBundle> vector;
    public:
        static WGPUVectorRenderBundle Obtain();
        int size();
        void push_back(const WebGPURenderBundle& attachment);
        const WebGPURenderBundle* data();
};

class WGPUVectorRenderPassColorAttachment {
    private:
        std::vector<WebGPURenderPassColorAttachment> vector;
    public:
        static WGPUVectorRenderPassColorAttachment Obtain();
        int size();
        void push_back(const WebGPURenderPassColorAttachment& attachment);
        const WebGPURenderPassColorAttachment* data();
};

class WGPUVectorVertexAttribute {
    private:
        std::vector<WebGPUVertexAttribute> vector;
    public:
        static WGPUVectorVertexAttribute Obtain();
        int size();
        void push_back(const WebGPUVertexAttribute& attribute);
        const WebGPUVertexAttribute* data();
};

class WGPUVectorBindGroupLayout {
    private:
        std::vector<WebGPUBindGroupLayout> vector;
    public:
        static WGPUVectorBindGroupLayout Obtain();
        int size();
        void push_back(const WebGPUBindGroupLayout& groupLayout);
        const WebGPUBindGroupLayout* data();
};

class WGPUVectorBindGroupLayoutEntry {
    private:
        std::vector<WebGPUBindGroupLayoutEntry> vector;
    public:
        static WGPUVectorBindGroupLayoutEntry Obtain();
        int size();
        void push_back(const WebGPUBindGroupLayoutEntry& entry);
        const WebGPUBindGroupLayoutEntry* data();
};

class WGPUVectorInt {
    private:
        std::vector<uint32_t> vector;
    public:
        static WGPUVectorInt Obtain();
        int size();
        void push_back(int attachment);
        int get(int index);
        const uint32_t* data();
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

class BufferMapCallback {
public:
    virtual void OnCallback(WGPUMapAsyncStatus status, const char* message) = 0;
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

class WebGPUBufferBindingLayout : public WebGPUObjectBase<WebGPUBufferBindingLayout, WGPUBufferBindingLayout*> {
    public:
        WebGPUBufferBindingLayout() {
            static WGPUBufferBindingLayout temp;
            temp = WGPUBufferBindingLayout{};
            Set(&temp);
        }
        WebGPUBufferBindingLayout(WGPUBufferBindingLayout* obj) {
            Set(obj);
        }
        static WebGPUBufferBindingLayout Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetType(WGPUBufferBindingType type);
        void SetHasDynamicOffset(int hasDynamicOffset);
        void SetMinBindingSize(int minBindingSize);
};

class WebGPUSamplerBindingLayout : public WebGPUObjectBase<WebGPUSamplerBindingLayout, WGPUSamplerBindingLayout*> {
    public:
        WebGPUSamplerBindingLayout() {
            static WGPUSamplerBindingLayout temp;
            temp = WGPUSamplerBindingLayout{};
            Set(&temp);
        }
        WebGPUSamplerBindingLayout(WGPUSamplerBindingLayout* obj) {
            Set(obj);
        }
        static WebGPUSamplerBindingLayout Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetType(WGPUSamplerBindingType type);
};

class WebGPUTextureBindingLayout : public WebGPUObjectBase<WebGPUTextureBindingLayout, WGPUTextureBindingLayout*> {
    public:
        WebGPUTextureBindingLayout() {
            static WGPUTextureBindingLayout temp;
            temp = WGPUTextureBindingLayout{};
            Set(&temp);
        }
        WebGPUTextureBindingLayout(WGPUTextureBindingLayout* obj) {
            Set(obj);
        }
        static WebGPUTextureBindingLayout Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetSampleType(WGPUTextureSampleType sampleType);
        void SetViewDimension(WGPUTextureViewDimension viewDimension);
        void SetMultisampled(int multisampled);
};

class WebGPUStorageTextureBindingLayout : public WebGPUObjectBase<WebGPUStorageTextureBindingLayout, WGPUStorageTextureBindingLayout*> {
    public:
        WebGPUStorageTextureBindingLayout() {
            static WGPUStorageTextureBindingLayout temp;
            temp = WGPUStorageTextureBindingLayout{};
            Set(&temp);
        }
        WebGPUStorageTextureBindingLayout(WGPUStorageTextureBindingLayout* obj) {
            Set(obj);
        }
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
        WebGPUBufferBindingLayout GetBuffer();
        WebGPUSamplerBindingLayout GetSampler();
        WebGPUStorageTextureBindingLayout GetStorageTexture();
        WebGPUTextureBindingLayout GetTexture();
};

class WebGPURequestAdapterOptions : public WebGPUObjectBase<WebGPURequestAdapterOptions, WGPURequestAdapterOptions> {
    public:
        static WebGPURequestAdapterOptions Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetFeatureLevel(WGPUFeatureLevel featureLevel);
        void SetPowerPreference(WGPUPowerPreference powerPreference);
        void SetBackendType(WGPUBackendType backendType);
        void SetCompatibleSurface(WebGPUSurface* compatibleSurface);
};

class WebGPUAdapterInfo : public WebGPUObjectBase<WebGPUAdapterInfo, WGPUAdapterInfo> {
    public:
        static WebGPUAdapterInfo Obtain();
        std::string GetVendor();
        int GetVendorID();
        std::string GetArchitecture();
        std::string GetDevice();
        int GetDeviceID();
        std::string GetDescription();
        WGPUBackendType GetBackendType();
        WGPUAdapterType GetAdapterType();
};

class WebGPUFuture : public WebGPUObjectBase<WebGPUFuture, WGPUFuture> {
    public:
};

class WebGPUFutureWaitInfo : public WebGPUObjectBase<WebGPUFutureWaitInfo, WGPUFutureWaitInfo> {
    public:
        static WebGPUFutureWaitInfo Obtain();
        void SetFuture(WebGPUFuture* future);
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
        void SetAttributes(WGPUVectorVertexAttribute* values);
        void SetArrayStride(int offset);
        void SetStepMode(WGPUVertexStepMode stepMode);
};

class WebGPUVertexState : public WebGPUObjectBase<WebGPUVertexState, WGPUVertexState*> {
    public:
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetModule(WebGPUShaderModule* shaderModule);
        void SetEntryPoint(const char* value);
        void SetConstants(WGPUVectorConstantEntry* values);
        void SetBuffers(WGPUVectorVertexBufferLayout* values);
};

class WebGPUShaderSourceWGSL : public WebGPUObjectBase<WebGPUShaderSourceWGSL, WGPUShaderSourceWGSL> {
    public:
        static WebGPUShaderSourceWGSL Obtain();
        void SetCode(const char* value);
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
        void SetTargets(WGPUVectorColorTargetState* values);
        void SetModule(WebGPUShaderModule* shaderModule);
        void SetConstants(WGPUVectorConstantEntry* values);
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
        void SetAlphaToCoverageEnabled(bool alphaToCoverageEnabled);
};

class WebGPUColor : public WebGPUObjectBase<WebGPUColor, WGPUColor*> {
    public:
        WebGPUColor() {
            static WGPUColor temp;
            temp = WGPUColor{};
            Set(&temp);
        }
        WebGPUColor(WGPUColor* color) {
            Set(color);
        }
        static WebGPUColor Obtain();
        void SetColor(float r, float g, float b, float a);
        float GetR();
        float GetG();
        float GetB();
        float GetA();
        void SetR(float value);
        void SetG(float value);
        void SetB(float value);
        void SetA(float value);
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
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetView(WebGPUTextureView* textureView);
        void SetDepthLoadOp(WGPULoadOp loadOp);
        void SetDepthStoreOp(WGPUStoreOp storeOp);
        void SetDepthClearValue(float depthClearValue);
        void SetDepthReadOnly(bool depthReadOnly);
        void SetStencilLoadOp(WGPULoadOp loadOp);
        void SetStencilStoreOp(WGPUStoreOp storeOp);
        void SetStencilClearValue(int stencilClearValue);
        void SetStencilReadOnly(bool stencilReadOnly);
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

class WebGPUOrigin3D : public WebGPUObjectBase<WebGPUOrigin3D, WGPUOrigin3D*> {
    public:
        WebGPUOrigin3D() {
            static WGPUOrigin3D temp;
            temp = WGPUOrigin3D{};
            WebGPUObjectBase::Set(&temp);
        }
        WebGPUOrigin3D(WGPUOrigin3D* ptr) {
            WebGPUObjectBase::Set(ptr);
        }

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
        WebGPUOrigin3D GetOrigin();
        void SetAspect(WGPUTextureAspect aspect);
};

class WebGPUExtent3D : public WebGPUObjectBase<WebGPUExtent3D, WGPUExtent3D*> {
    public:
        WebGPUExtent3D() {
            static WGPUExtent3D temp;
            temp = WGPUExtent3D{};
            Set(&temp);
        }
        WebGPUExtent3D(WGPUExtent3D* ptr) {
            Set(ptr);
        }

        static WebGPUExtent3D Obtain();
        void SetWidth(int width);
        void SetHeight(int height);
        void SetDepthOrArrayLayers(int depthOrArrayLayers);
};

class WebGPUTexelCopyBufferLayout : public WebGPUObjectBase<WebGPUTexelCopyBufferLayout, WGPUTexelCopyBufferLayout*> {
    public:
        WebGPUTexelCopyBufferLayout() {
            static WGPUTexelCopyBufferLayout temp;
            temp = WGPUTexelCopyBufferLayout{};
            Set(&temp);
        }
        WebGPUTexelCopyBufferLayout(WGPUTexelCopyBufferLayout* ptr) {
            Set(ptr);
        }
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
        void SetBuffer(WebGPUBuffer* buffer);
};

class WebGPUSurfaceConfiguration : public WebGPUObjectBase<WebGPUSurfaceConfiguration, WGPUSurfaceConfiguration> {
    public:
        static WebGPUSurfaceConfiguration Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetWidth(long width);
        void SetHeight(long height);
        void SetFormat(WGPUTextureFormat format);
        void SetViewFormatCount(long value);
        void SetViewFormats(WGPUVectorTextureFormat* values);
        void SetUsage(WGPUTextureUsage usage);
        void SetDevice(WebGPUDevice* device);
        void SetPresentMode(WGPUPresentMode presentMode);
        void SetAlphaMode(WGPUCompositeAlphaMode alphaMode);
};

class WebGPUSurfaceCapabilities : public WebGPUObjectBase<WebGPUSurfaceCapabilities, WGPUSurfaceCapabilities> {
    public:
        static WebGPUSurfaceCapabilities Obtain();
        WGPUVectorTextureFormat GetFormats();
};

class WebGPURenderPassColorAttachment : public WebGPUObjectBase<WebGPURenderPassColorAttachment, WGPURenderPassColorAttachment> {
    public:
        WebGPURenderPassColorAttachment();
        static WebGPURenderPassColorAttachment Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
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

class WebGPUBindGroupEntry : public WebGPUObjectBase<WebGPUBindGroupEntry, WGPUBindGroupEntry> {
public:
    static WebGPUBindGroupEntry Obtain();
    void SetNextInChain(WebGPUChainedStruct* chainedStruct);
    void SetBinding(int binding);
    void SetBuffer(WebGPUBuffer* buffer);
    void SetOffset(int offset);
    void SetSize(int size);
    void SetSampler(WebGPUSampler* sampler);
    void SetTextureView(WebGPUTextureView* textureView);
};

//class WebGPUInstanceCapabilities : public WebGPUObjectBase<WebGPUInstanceCapabilities, WGPUInstanceCapabilities*> {
//    public:
//        void SetTimedWaitAnyEnable(bool enable);
//        void SetTimedWaitAnyMaxCount(int timedWaitAnyMaxCount);
//};

class WebGPURenderPassMaxDrawCount : public WebGPUObjectBase<WebGPURenderPassMaxDrawCount, WGPURenderPassMaxDrawCount> {
    public:
        static WebGPURenderPassMaxDrawCount Obtain();
        void SetMaxDrawCount(int maxDrawCount);
        int GetMaxDrawCount();
};

class WebGPUShaderSourceSPIRV : public WebGPUObjectBase<WebGPUShaderSourceSPIRV, WGPUShaderSourceSPIRV> {
    public:
    //TODO change to vector
        static WebGPUShaderSourceSPIRV Obtain();
//        void SetCode(const int* code, int codeSize);
//        WebGPUChainedStruct GetChain();
};

class WebGPUSupportedWGSLLanguageFeatures : public WebGPUObjectBase<WebGPUSupportedWGSLLanguageFeatures, WGPUSupportedWGSLLanguageFeatures> {
    public:
        static WebGPUSupportedWGSLLanguageFeatures Obtain();
        int GetFeatureCount();
        WGPUWGSLLanguageFeatureName GetFeatureAt(int index);
};

class WebGPUCompilationInfo : public WebGPUObjectBase<WebGPUCompilationInfo, WGPUCompilationInfo> {
    public:
        int GetMessageCount();
        WebGPUCompilationMessage GetMessage(int index);
};

class WebGPUCompilationMessage : public WebGPUObjectBase<WebGPUCompilationMessage, WGPUCompilationMessage> {
    public:
        static WebGPUCompilationMessage Obtain();
        std::string GetMessage();
        WGPUCompilationMessageType GetType();
        int GetLineNum();
        int GetLinePos();
        int GetOffset();
        int GetLength();
};

// ################################### DESCRIPTOR STRUCTS ###################################

class WebGPUInstanceDescriptor : public WebGPUObjectBase<WebGPUInstanceDescriptor, WGPUInstanceDescriptor> {
    public:
        static WebGPUInstanceDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
//        WebGPUInstanceCapabilities GetFeatures(); // TODO dawn have different code
};

class WebGPURenderBundleDescriptor : public WebGPUObjectBase<WebGPURenderBundleDescriptor, WGPURenderBundleDescriptor> {
    public:
        static WebGPURenderBundleDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
};

class WebGPURenderBundleEncoderDescriptor : public WebGPUObjectBase<WebGPURenderBundleEncoderDescriptor, WGPURenderBundleEncoderDescriptor> {
    public:
        static WebGPURenderBundleEncoderDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetColorFormats(WGPUVectorTextureFormat* colorFormats);
        void SetDepthStencilFormat(WGPUTextureFormat depthStencilFormat);
        void SetSampleCount(int sampleCount);
        void SetDepthReadOnly(int depthReadOnly);
        void SetStencilReadOnly(int stencilReadOnly);
};

class WebGPUQuerySetDescriptor : public WebGPUObjectBase<WebGPUQuerySetDescriptor, WGPUQuerySetDescriptor> {
    public:
        static WebGPUQuerySetDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetType(WGPUQueryType type);
        void SetCount(int count);
};

class WebGPUSamplerDescriptor : public WebGPUObjectBase<WebGPUSamplerDescriptor, WGPUSamplerDescriptor> {
    public:
        static WebGPUSamplerDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetAddressModeU(WGPUAddressMode addressModeU);
        void SetAddressModeV(WGPUAddressMode addressModeV);
        void SetAddressModeW(WGPUAddressMode addressModeW);
        void SetMagFilter(WGPUFilterMode magFilter);
        void SetMinFilter(WGPUFilterMode minFilter);
        void SetMipmapFilter(WGPUMipmapFilterMode mipmapFilter);
        void SetLodMinClamp(float lodMinClamp);
        void SetLodMaxClamp(float lodMaxClamp);
        void SetCompare(WGPUCompareFunction compare);
        void SetMaxAnisotropy(int maxAnisotropy);
};

class WebGPUBindGroupLayoutDescriptor : public WebGPUObjectBase<WebGPUBindGroupLayoutDescriptor, WGPUBindGroupLayoutDescriptor> {
    public:
        static WebGPUBindGroupLayoutDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetEntries(WGPUVectorBindGroupLayoutEntry* entries);
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
        void SetMappedAtCreation(bool mappedAtCreation);
};

class WebGPUBindGroupDescriptor : public WebGPUObjectBase<WebGPUBindGroupDescriptor, WGPUBindGroupDescriptor> {
    public:
        static WebGPUBindGroupDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetLayout(WebGPUBindGroupLayout* layout);
        void SetEntries(WGPUVectorBindGroupEntry* entries);
};

class WebGPUPipelineLayoutDescriptor : public WebGPUObjectBase<WebGPUPipelineLayoutDescriptor, WGPUPipelineLayoutDescriptor> {
    public:
        static WebGPUPipelineLayoutDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetBindGroupLayouts(WGPUVectorBindGroupLayout* bindGroupLayouts);
};

class WebGPUDeviceDescriptor : public WebGPUObjectBase<WebGPUDeviceDescriptor, WGPUDeviceDescriptor> {
    public:
        static WebGPUDeviceDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetRequiredLimits(WebGPULimits* limits);
        void SetRequiredFeatures(WGPUVectorFeatureName* features);
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
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetColorAttachments(WGPUVectorRenderPassColorAttachment* values);
        void SetDepthStencilAttachment(WebGPURenderPassDepthStencilAttachment* attachment);
        void SetOcclusionQuerySet(WebGPUQuerySet* occlusionQuerySet);
        void SetTimestampWrites(WebGPURenderPassTimestampWrites* timestampWrites);
};

class WebGPUComputePassDescriptor : public WebGPUObjectBase<WebGPUComputePassDescriptor, WGPUComputePassDescriptor> {
    public:
        static WebGPUComputePassDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetTimestampWrites(WebGPUComputePassTimestampWrites* timestampWrites);
};

class WebGPUTextureDescriptor : public WebGPUObjectBase<WebGPUTextureDescriptor, WGPUTextureDescriptor> {
    public:
        static WebGPUTextureDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetUsage(WGPUTextureUsage usage);
        void SetDimension(WGPUTextureDimension dimension);
        WebGPUExtent3D GetSize();
        void SetFormat(WGPUTextureFormat format);
        void SetMipLevelCount(int mipLevelCount);
        void SetSampleCount(int sampleCount);
        void SetViewFormats(WGPUVectorTextureFormat* viewFormats);
};

class WebGPUTextureViewDescriptor : public WebGPUObjectBase<WebGPUTextureViewDescriptor, WGPUTextureViewDescriptor> {
    public:
        static WebGPUTextureViewDescriptor Obtain();
        void SetNextInChain(WebGPUChainedStruct* chainedStruct);
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

class WebGPUSampler : public WebGPUObjectBase<WebGPUSampler, WGPUSampler> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
};

class WebGPURenderBundleEncoder : public WebGPUObjectBase<WebGPURenderBundleEncoder, WGPURenderBundleEncoder> {
// TODO implement methods
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WebGPURenderBundleEncoder Obtain();
        void SetPipeline(WebGPURenderPipeline* renderPipeline);
        void Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance);
        void DrawIndexed(int indexCount, int instanceCount, int firstIndex, int baseVertex, int firstInstance);
        void DrawIndirect(WebGPUBuffer* indirectBuffer, int indirectOffset);
        void DrawIndexedIndirect(WebGPUBuffer* indirectBuffer, int indirectOffset);
        void SetBindGroup(int groupIndex, WebGPUBindGroup* group, WGPUVectorInt* dynamicOffsets);
        void SetVertexBuffer(int slot, WebGPUBuffer* buffer, int offset, int size);
        void SetIndexBuffer(WebGPUBuffer* buffer, WGPUIndexFormat format, int offset, int size);
        void InsertDebugMarker(const char* label);
        void PopDebugGroup();
        void PushDebugGroup(const char* label);
        void SetLabel(const char* label);
        void Finish(WebGPURenderBundleDescriptor* descriptor, WebGPURenderBundle* bundle);
};

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
        void Destroy();
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
        void ExecuteBundles(WGPUVectorRenderBundle* values);
        void InsertDebugMarker(const char* label);
        void PopDebugGroup();
        void PushDebugGroup(const char* label);
        void SetBindGroup(int groupIndex, WebGPUBindGroup* group, WGPUVectorInt* dynamicOffsets);
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
        void Destroy();
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
        WGPUWaitStatus WaitAny(WGPUVectorFutureWaitInfo* futureVector, int timeoutNS);
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
        void CreatePipelineLayout(WebGPUPipelineLayoutDescriptor* descriptor, WebGPUPipelineLayout* valueOut);
        void CreateQuerySet(WebGPUQuerySetDescriptor* descriptor, WebGPUQuerySet* valueOut);
        void CreateRenderBundleEncoder(WebGPURenderBundleEncoderDescriptor* descriptor, WebGPURenderBundleEncoder* valueOut);
        void CreateRenderPipeline(WebGPURenderPipelineDescriptor* pipelineDescriptor, WebGPURenderPipeline* valueOut);
        void CreateSampler(WebGPUSamplerDescriptor* descriptor, WebGPUSampler* valueOut);
        void CreateShaderModule(WebGPUShaderModuleDescriptor* shaderModuleDescriptor, WebGPUShaderModule* valueOut);
        void CreateTexture(WebGPUTextureDescriptor* descriptor, WebGPUTexture* valueOut);
        void Destroy();

        void GetFeatures(WebGPUSupportedFeatures* features);
        bool HasFeature(WGPUFeatureName feature);
        void GetLimits(WebGPULimits* limits);
        WebGPUQueue GetQueue();
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
        void SetBindGroup(int groupIndex, WebGPUBindGroup* group, WGPUVectorInt* offsets);
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
        WebGPUFuture MapAsync(WGPUMapMode mode, int offset, int size, WGPUCallbackMode callbackMode, BufferMapCallback* callback);
        WGPUByteBuffer GetConstMappedRange(int offset, int size);
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
        WGPUStatus GetLimits(WebGPULimits* limits);
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
        void WriteTexture(WebGPUTexelCopyTextureInfo* destination, WGPUByteBuffer* bytes, WebGPUTexelCopyBufferLayout* dataLayout, WebGPUExtent3D* writeSize);
};