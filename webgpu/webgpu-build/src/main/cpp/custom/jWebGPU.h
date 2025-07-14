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

namespace JGPU {

class WGPUInstance;
class WGPUAdapter;
class WGPUDevice;
class WGPUDeviceDescriptor;
class WGPUSurface;
class WGPUQueue;
class WGPUStringView;
class WGPUSurfaceConfiguration;
class WGPUBlendState;
class WGPUColorTargetState;
class WGPUDepthStencilState;
class WGPURenderPassColorAttachment;
class WGPUConstantEntry;
class WGPUVertexBufferLayout;
class WGPURenderBundle;
class WGPUVertexAttribute;
class WGPUBindGroup;
class WGPUBindGroupLayout;
class WGPUBindGroupLayoutEntry;
class WGPUShaderModule;
class WGPUQuerySet;
class WGPUTexture;
class WGPUBuffer;
class WGPUTextureView;
class WGPUPipelineLayout;
class WGPUCommandEncoder;
class WGPUComputePipeline;
class WGPURenderPipeline;
class WGPUSampler;
class WGPUCompilationMessage;
class WGPUBindGroupEntry;
class WGPUFuture;
class WGPUFutureWaitInfo;
class WGPUInstanceDescriptor;

}

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

enum WGPUByteOrder : int {
    BigEndian = 1,
    LittleEndian = 2
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

    private:
        std::vector<uint8_t> buffer;
        size_t _position = 0;
        size_t _limit;
        WGPUByteOrder byteOrder;
        bool needsSwap;
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
        static WGPUByteBuffer* Obtain(int capacity);
        WGPUByteBuffer() : buffer(0), _limit(0) {
            order(LittleEndian);
        }
        WGPUByteBuffer(int capacity) : buffer(capacity), _limit(capacity) {
            order(LittleEndian);
        }
        WGPUByteBuffer(uint8_t* bufferData, int dataSize) : _limit(dataSize) {
            order(LittleEndian);
            buffer.insert(buffer.end(), bufferData, bufferData + dataSize);
        }
        void order(WGPUByteOrder order);
        void put(int index, char value);
        char get(int index);
        void put(char value);
        char get();
        void put(const uint8_t* values, int index, int size);
        void putInt(int byteIndex, int value);
        int getInt(int byteIndex);
        void putFloat(int byteIndex, float value);
        float getFloat(int byteIndex);
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
        std::vector<JGPU::WGPUFutureWaitInfo> vector;
    public:
        static WGPUVectorFutureWaitInfo* Obtain();
        int size();
        void push_back(const JGPU::WGPUFutureWaitInfo& entry);
        const JGPU::WGPUFutureWaitInfo* data();
};

class WGPUVectorBindGroupEntry {
    private:
        std::vector<JGPU::WGPUBindGroupEntry> vector;
    public:
        static WGPUVectorBindGroupEntry* Obtain();
        int size();
        void push_back(const JGPU::WGPUBindGroupEntry& entry);
        const JGPU::WGPUBindGroupEntry* data();
};

class WGPUVectorColorTargetState {
    private:
        std::vector<JGPU::WGPUColorTargetState> vector;
    public:
        static WGPUVectorColorTargetState* Obtain();
        int size();
        void push_back(const JGPU::WGPUColorTargetState& attachment);
        const JGPU::WGPUColorTargetState* data();
};

class WGPUVectorFeatureName {
    private:
        std::vector<WGPUFeatureName> vector;
    public:
        static WGPUVectorFeatureName* Obtain();
        int size();
        void push_back(const WGPUFeatureName& attachment);
        const WGPUFeatureName* data();
};

class WGPUVectorConstantEntry {
    private:
        std::vector<JGPU::WGPUConstantEntry> vector;
    public:
        static WGPUVectorConstantEntry* Obtain();
        int size();
        void push_back(const JGPU::WGPUConstantEntry& attachment);
        const JGPU::WGPUConstantEntry* data();
};

class WGPUVectorVertexBufferLayout {
    private:
        std::vector<JGPU::WGPUVertexBufferLayout> vector;
    public:
        static WGPUVectorVertexBufferLayout* Obtain();
        int size();
        void push_back(const JGPU::WGPUVertexBufferLayout& attachment);
        const JGPU::WGPUVertexBufferLayout* data();
};

class WGPUVectorTextureFormat {
    private:
        std::vector<WGPUTextureFormat> vector;
    public:
        static WGPUVectorTextureFormat* Obtain();
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
        std::vector<JGPU::WGPURenderBundle> vector;
    public:
        static WGPUVectorRenderBundle* Obtain();
        int size();
        void push_back(const JGPU::WGPURenderBundle& attachment);
        const JGPU::WGPURenderBundle* data();
};

class WGPUVectorRenderPassColorAttachment {
    private:
        std::vector<JGPU::WGPURenderPassColorAttachment> vector;
    public:
        static WGPUVectorRenderPassColorAttachment* Obtain();
        int size();
        void push_back(const JGPU::WGPURenderPassColorAttachment& attachment);
        const JGPU::WGPURenderPassColorAttachment* data();
        void clear();
};

class WGPUVectorVertexAttribute {
    private:
        std::vector<JGPU::WGPUVertexAttribute> vector;
    public:
        static WGPUVectorVertexAttribute* Obtain();
        int size();
        void push_back(const JGPU::WGPUVertexAttribute& attribute);
        const JGPU::WGPUVertexAttribute* data();
};

class WGPUVectorBindGroupLayout {
    private:
        std::vector<JGPU::WGPUBindGroupLayout> vector;
    public:
        static WGPUVectorBindGroupLayout* Obtain();
        int size();
        void push_back(const JGPU::WGPUBindGroupLayout& groupLayout);
        const JGPU::WGPUBindGroupLayout* data();
};

class WGPUVectorBindGroupLayoutEntry {
    private:
        std::vector<JGPU::WGPUBindGroupLayoutEntry> vector;
    public:
        static WGPUVectorBindGroupLayoutEntry* Obtain();
        int size();
        void push_back(const JGPU::WGPUBindGroupLayoutEntry& entry);
        const JGPU::WGPUBindGroupLayoutEntry* data();
};

class WGPUVectorInt {
    private:
        std::vector<uint32_t> vector;
    public:
        static WGPUVectorInt* Obtain();
        int size();
        void push_back(int attachment);
        int get(int index);
        const uint32_t* data();
};

class RequestAdapterCallback {
public:
    virtual void OnCallback(WGPURequestAdapterStatus status, JGPU::WGPUAdapter* adapter) = 0;
};

class RequestDeviceCallback {
public:
    virtual void OnCallback(WGPURequestDeviceStatus status, JGPU::WGPUDevice* device) = 0;
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
class WGPUObjectBase {
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

namespace JGPU {

class WGPUStringView : public WGPUObjectBase<WGPUStringView, ::WGPUStringView> {
    public:
        WGPUStringView() {}
        WGPUStringView(::WGPUStringView stringView) {
            Set(stringView);
        }
        WGPUStringView(const char* value) {
            Get().data = strdup(value);
            Get().length = strlen(value);
        }
        const std::string GetString();
};

class WGPUChainedStruct : public WGPUObjectBase<WGPUChainedStruct, ::WGPUChainedStruct*> {
    public:
        void SetNext(WGPUChainedStruct* value);
        void SetSType(WGPUSType type);
};

class WGPULimits : public WGPUObjectBase<WGPULimits, ::WGPULimits> {
    public:
        static WGPULimits* Obtain();
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

class WGPUBufferBindingLayout : public WGPUObjectBase<WGPUBufferBindingLayout, ::WGPUBufferBindingLayout*> {
    public:
        WGPUBufferBindingLayout() {
            static ::WGPUBufferBindingLayout temp;
            temp = ::WGPUBufferBindingLayout{};
            Set(&temp);
        }
        WGPUBufferBindingLayout(::WGPUBufferBindingLayout* obj) {
            Set(obj);
        }
        static WGPUBufferBindingLayout* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetType(WGPUBufferBindingType type);
        void SetHasDynamicOffset(int hasDynamicOffset);
        void SetMinBindingSize(int minBindingSize);
};

class WGPUSamplerBindingLayout : public WGPUObjectBase<WGPUSamplerBindingLayout, ::WGPUSamplerBindingLayout*> {
    public:
        WGPUSamplerBindingLayout() {
            static ::WGPUSamplerBindingLayout temp;
            temp = ::WGPUSamplerBindingLayout{};
            Set(&temp);
        }
        WGPUSamplerBindingLayout(::WGPUSamplerBindingLayout* obj) {
            Set(obj);
        }
        static WGPUSamplerBindingLayout* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetType(WGPUSamplerBindingType type);
};

class WGPUTextureBindingLayout : public WGPUObjectBase<WGPUTextureBindingLayout, ::WGPUTextureBindingLayout*> {
    public:
        WGPUTextureBindingLayout() {
            static ::WGPUTextureBindingLayout temp;
            temp = ::WGPUTextureBindingLayout{};
            Set(&temp);
        }
        WGPUTextureBindingLayout(::WGPUTextureBindingLayout* obj) {
            Set(obj);
        }
        static WGPUTextureBindingLayout* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetSampleType(WGPUTextureSampleType sampleType);
        void SetViewDimension(WGPUTextureViewDimension viewDimension);
        void SetMultisampled(int multisampled);
};

class WGPUStorageTextureBindingLayout : public WGPUObjectBase<WGPUStorageTextureBindingLayout, ::WGPUStorageTextureBindingLayout*> {
    public:
        WGPUStorageTextureBindingLayout() {
            static ::WGPUStorageTextureBindingLayout temp;
            temp = ::WGPUStorageTextureBindingLayout{};
            Set(&temp);
        }
        WGPUStorageTextureBindingLayout(::WGPUStorageTextureBindingLayout* obj) {
            Set(obj);
        }
        static WGPUStorageTextureBindingLayout* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetAccess(WGPUStorageTextureAccess access);
        void SetFormat(WGPUTextureFormat format);
        void SetViewDimension(WGPUTextureViewDimension viewDimension);
};

class WGPUBindGroupLayoutEntry : public WGPUObjectBase<WGPUBindGroupLayoutEntry, ::WGPUBindGroupLayoutEntry> {
    public:
        static WGPUBindGroupLayoutEntry* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetBinding(int binding);
        void SetVisibility(WGPUShaderStage visibility);
        void SetBuffer(WGPUBufferBindingLayout* buffer);
        void SetSampler(WGPUSamplerBindingLayout* sampler);
        void SetTexture(WGPUTextureBindingLayout* texture);
        void SetStorageTexture(WGPUStorageTextureBindingLayout* storageTexture);
        WGPUBufferBindingLayout GetBuffer();
        WGPUSamplerBindingLayout GetSampler();
        WGPUStorageTextureBindingLayout GetStorageTexture();
        WGPUTextureBindingLayout GetTexture();
};

class WGPURequestAdapterOptions : public WGPUObjectBase<WGPURequestAdapterOptions, ::WGPURequestAdapterOptions> {
    public:
        static WGPURequestAdapterOptions* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetFeatureLevel(WGPUFeatureLevel featureLevel);
        void SetPowerPreference(WGPUPowerPreference powerPreference);
        void SetBackendType(WGPUBackendType backendType);
        void SetCompatibleSurface(WGPUSurface* compatibleSurface);
};

class WGPUAdapterInfo : public WGPUObjectBase<WGPUAdapterInfo, ::WGPUAdapterInfo> {
    public:
        static WGPUAdapterInfo* Obtain();
        std::string GetVendor();
        int GetVendorID();
        std::string GetArchitecture();
        std::string GetDevice();
        int GetDeviceID();
        std::string GetDescription();
        WGPUBackendType GetBackendType();
        WGPUAdapterType GetAdapterType();
};

class WGPUFuture : public WGPUObjectBase<WGPUFuture, ::WGPUFuture> {
    public:
};

class WGPUFutureWaitInfo : public WGPUObjectBase<WGPUFutureWaitInfo, ::WGPUFutureWaitInfo> {
    public:
        static WGPUFutureWaitInfo* Obtain();
        void SetFuture(WGPUFuture* future);
};

class WGPUConstantEntry : public WGPUObjectBase<WGPUConstantEntry, ::WGPUConstantEntry> {
    public:
};

class WGPUVertexAttribute : public WGPUObjectBase<WGPUVertexAttribute, ::WGPUVertexAttribute> {
    public:
        static WGPUVertexAttribute* Obtain();
        void SetFormat(WGPUVertexFormat format);
        void SetOffset(int offset);
        void SetShaderLocation(int shaderLocation);
};

class WGPUVertexBufferLayout : public WGPUObjectBase<WGPUVertexBufferLayout, ::WGPUVertexBufferLayout> {
    public:
        static WGPUVertexBufferLayout* Obtain();
        void SetAttributes(WGPUVectorVertexAttribute* values);
        void SetArrayStride(int offset);
        void SetStepMode(WGPUVertexStepMode stepMode);
};

class WGPUVertexState : public WGPUObjectBase<WGPUVertexState, ::WGPUVertexState*> {
    public:
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetModule(WGPUShaderModule* shaderModule);
        void SetEntryPoint(const char* value);
        void SetConstants(WGPUVectorConstantEntry* values);
        void SetBuffers(WGPUVectorVertexBufferLayout* values);
};

class WGPUShaderSourceWGSL : public WGPUObjectBase<WGPUShaderSourceWGSL, ::WGPUShaderSourceWGSL> {
    public:
        static WGPUShaderSourceWGSL* Obtain();
        void SetCode(const char* value);
        WGPUChainedStruct GetChain();
};

class WGPUBlendComponent : public WGPUObjectBase<WGPUBlendComponent, ::WGPUBlendComponent*> {
    public:
        void SetOperation(WGPUBlendOperation operation);
        WGPUBlendOperation GetOperation();
        void SetSrcFactor(WGPUBlendFactor factor);
        WGPUBlendFactor GetSrcFactor();
        void SetDstFactor(WGPUBlendFactor factor);
        WGPUBlendFactor GetDstFactor();
};

class WGPUBlendState : public WGPUObjectBase<WGPUBlendState, ::WGPUBlendState> {
    public:
        static WGPUBlendState* Obtain();
        WGPUBlendComponent GetColor();
        WGPUBlendComponent GetAlpha();
};

class WGPUColorTargetState : public WGPUObjectBase<WGPUColorTargetState, ::WGPUColorTargetState> {
    public:
        static WGPUColorTargetState* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetFormat(WGPUTextureFormat format);
        void SetBlend(WGPUBlendState* blendState);
        void SetWriteMask(WGPUColorWriteMask writeMask);
};

class WGPUFragmentState : public WGPUObjectBase<WGPUFragmentState, ::WGPUFragmentState> {
    public:
        static WGPUFragmentState* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetEntryPoint(const char* value);
        void SetTargets(WGPUVectorColorTargetState* values);
        void SetModule(WGPUShaderModule* shaderModule);
        void SetConstants(WGPUVectorConstantEntry* values);
};

class WGPUPrimitiveState : public WGPUObjectBase<WGPUPrimitiveState, ::WGPUPrimitiveState*> {
    public:
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetTopology(WGPUPrimitiveTopology value);
        void SetStripIndexFormat(WGPUIndexFormat value);
        void SetFrontFace(WGPUFrontFace value);
        void SetCullMode(WGPUCullMode value);
};

class WGPUStencilFaceState : public WGPUObjectBase<WGPUStencilFaceState, ::WGPUStencilFaceState*> {
    public:
        void SetCompare(WGPUCompareFunction compare);
        void SetFailOp(WGPUStencilOperation failOp);
        void SetDepthFailOp(WGPUStencilOperation depthFailOp);
        void SetPassOp(WGPUStencilOperation passOp);
};

class WGPUDepthStencilState : public WGPUObjectBase<WGPUDepthStencilState, ::WGPUDepthStencilState> {
    public:
        static WGPUDepthStencilState* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetFormat(WGPUTextureFormat format);
        void SetDepthWriteEnabled(WGPUOptionalBool depthWriteEnabled);
        void SetDepthCompare(WGPUCompareFunction depthCompare);
        void SetDepthBiasSlopeScale(float depthBiasSlopeScale);
        void SetDepthBiasClamp(float depthBiasClamp);
        void SetStencilReadMask(int stencilReadMask);
        void SetStencilWriteMask(int stencilWriteMask);
        void SetDepthBias(int depthBias);
        WGPUStencilFaceState GetStencilFront();
        WGPUStencilFaceState GetStencilBack();
};

class WGPUMultisampleState : public WGPUObjectBase<WGPUMultisampleState, ::WGPUMultisampleState*> {
    public:
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetCount(int count);
        void SetMask(int mask);
        void SetAlphaToCoverageEnabled(bool alphaToCoverageEnabled);
};

class WGPUColor : public WGPUObjectBase<WGPUColor, ::WGPUColor*> {
    public:
        WGPUColor() {
            static ::WGPUColor temp;
            temp = ::WGPUColor{};
            Set(&temp);
        }
        WGPUColor(::WGPUColor* color) {
            Set(color);
        }
        static WGPUColor* Obtain();
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

class WGPUSupportedFeatures : public WGPUObjectBase<WGPUSupportedFeatures, ::WGPUSupportedFeatures> {
    public:
        static WGPUSupportedFeatures* Obtain();
        int GetFeatureCount();
        WGPUFeatureName GetFeatureAt(long index);
};

class WGPURenderPassDepthStencilAttachment : public WGPUObjectBase<WGPURenderPassDepthStencilAttachment, ::WGPURenderPassDepthStencilAttachment> {
    public:
        static WGPURenderPassDepthStencilAttachment* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetView(WGPUTextureView* textureView);
        void SetDepthLoadOp(WGPULoadOp loadOp);
        void SetDepthStoreOp(WGPUStoreOp storeOp);
        void SetDepthClearValue(float depthClearValue);
        void SetDepthReadOnly(bool depthReadOnly);
        void SetStencilLoadOp(WGPULoadOp loadOp);
        void SetStencilStoreOp(WGPUStoreOp storeOp);
        void SetStencilClearValue(int stencilClearValue);
        void SetStencilReadOnly(bool stencilReadOnly);
};

class WGPURenderPassTimestampWrites : public WGPUObjectBase<WGPURenderPassTimestampWrites, ::WGPURenderPassTimestampWrites> {
    public:
        static WGPURenderPassTimestampWrites* Obtain();
        void SetQuerySet(WGPUQuerySet* value);
        void SetBeginningOfPassWriteIndex(int value);
        void SetEndOfPassWriteIndex(int value);
};

class WGPUComputePassTimestampWrites : public WGPUObjectBase<WGPUComputePassTimestampWrites, ::WGPUComputePassTimestampWrites> {
    public:
        static WGPUComputePassTimestampWrites* Obtain();
        void SetQuerySet(WGPUQuerySet* value);
        void SetBeginningOfPassWriteIndex(int value);
        void SetEndOfPassWriteIndex(int value);
};

class WGPUOrigin3D : public WGPUObjectBase<WGPUOrigin3D, ::WGPUOrigin3D*> {
    public:
        WGPUOrigin3D() {
            static ::WGPUOrigin3D temp;
            temp = ::WGPUOrigin3D{};
            WGPUObjectBase::Set(&temp);
        }
        WGPUOrigin3D(::WGPUOrigin3D* ptr) {
            WGPUObjectBase::Set(ptr);
        }

        static WGPUOrigin3D* Obtain();
        void Set(int x, int y, int z);
        void SetX(int value);
        void SetY(int value);
        void SetZ(int value);
};

class WGPUTexelCopyTextureInfo : public WGPUObjectBase<WGPUTexelCopyTextureInfo, ::WGPUTexelCopyTextureInfo> {
    public:
        static WGPUTexelCopyTextureInfo* Obtain();
        void SetTexture(WGPUTexture* texture);
        void SetMipLevel(int mipLevel);
        WGPUOrigin3D GetOrigin();
        void SetAspect(WGPUTextureAspect aspect);
};

class WGPUExtent3D : public WGPUObjectBase<WGPUExtent3D, ::WGPUExtent3D*> {
    public:
        WGPUExtent3D() {
            static ::WGPUExtent3D temp;
            temp = ::WGPUExtent3D{};
            Set(&temp);
        }
        WGPUExtent3D(::WGPUExtent3D* ptr) {
            Set(ptr);
        }

        static WGPUExtent3D* Obtain();
        void SetWidth(int width);
        void SetHeight(int height);
        void SetDepthOrArrayLayers(int depthOrArrayLayers);
};

class WGPUTexelCopyBufferLayout : public WGPUObjectBase<WGPUTexelCopyBufferLayout, ::WGPUTexelCopyBufferLayout*> {
    public:
        WGPUTexelCopyBufferLayout() {
            static ::WGPUTexelCopyBufferLayout temp;
            temp = ::WGPUTexelCopyBufferLayout{};
            Set(&temp);
        }
        WGPUTexelCopyBufferLayout(::WGPUTexelCopyBufferLayout* ptr) {
            Set(ptr);
        }
        static WGPUTexelCopyBufferLayout* Obtain();
        void SetOffset(int offset);
        void SetBytesPerRow(int bytesPerRow);
        void SetRowsPerImage(int rowsPerImage);
};

class WGPUTexelCopyBufferInfo : public WGPUObjectBase<WGPUTexelCopyBufferInfo, ::WGPUTexelCopyBufferInfo> {
    public:
        static WGPUTexelCopyBufferInfo* Obtain();
        WGPUTexelCopyBufferLayout GetLayout();
        WGPUBuffer GetBuffer();
        void SetBuffer(WGPUBuffer* buffer);
};

class WGPUSurfaceConfiguration : public WGPUObjectBase<WGPUSurfaceConfiguration, ::WGPUSurfaceConfiguration> {
    public:
        static WGPUSurfaceConfiguration* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetWidth(long width);
        void SetHeight(long height);
        void SetFormat(WGPUTextureFormat format);
        void SetViewFormatCount(long value);
        void SetViewFormats(WGPUVectorTextureFormat* values);
        void SetUsage(WGPUTextureUsage usage);
        void SetDevice(WGPUDevice* device);
        void SetPresentMode(WGPUPresentMode presentMode);
        void SetAlphaMode(WGPUCompositeAlphaMode alphaMode);
};

class WGPUSurfaceCapabilities : public WGPUObjectBase<WGPUSurfaceCapabilities, ::WGPUSurfaceCapabilities> {
    public:
        static WGPUSurfaceCapabilities* Obtain();
        WGPUVectorTextureFormat GetFormats();
};

class WGPURenderPassColorAttachment : public WGPUObjectBase<WGPURenderPassColorAttachment, ::WGPURenderPassColorAttachment> {
    public:
        WGPURenderPassColorAttachment();
        static WGPURenderPassColorAttachment* Obtain();
        void Reset();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetResolveTarget(WGPUTextureView* textureView);
        void SetView(WGPUTextureView* textureView);
        void SetLoadOp(WGPULoadOp loadOp);
        void SetStoreOp(WGPUStoreOp storeOp);
        void SetDepthSlice(int depthSlice);
        WGPUColor GetClearValue();
};

class WGPUSurfaceTexture : public WGPUObjectBase<WGPUSurfaceTexture, ::WGPUSurfaceTexture> {
    public:
        static WGPUSurfaceTexture* Obtain();
        void GetTexture(WGPUTexture* tex);
        WGPUSurfaceGetCurrentTextureStatus GetStatus();
};

class WGPUBindGroupEntry : public WGPUObjectBase<WGPUBindGroupEntry, ::WGPUBindGroupEntry> {
public:
    static WGPUBindGroupEntry* Obtain();
    void Reset();
    void SetNextInChain(WGPUChainedStruct* chainedStruct);
    void SetBinding(int binding);
    void SetBuffer(WGPUBuffer* buffer);
    void SetOffset(int offset);
    void SetSize(int size);
    void SetSampler(WGPUSampler* sampler);
    void SetTextureView(WGPUTextureView* textureView);
};

//class WGPUInstanceCapabilities : public WGPUObjectBase<WGPUInstanceCapabilities, ::WGPUInstanceCapabilities*> {
//    public:
//        void SetTimedWaitAnyEnable(bool enable);
//        void SetTimedWaitAnyMaxCount(int timedWaitAnyMaxCount);
//};

class WGPURenderPassMaxDrawCount : public WGPUObjectBase<WGPURenderPassMaxDrawCount, ::WGPURenderPassMaxDrawCount> {
    public:
        static WGPURenderPassMaxDrawCount* Obtain();
        void SetMaxDrawCount(int maxDrawCount);
        int GetMaxDrawCount();
};

class WGPUShaderSourceSPIRV : public WGPUObjectBase<WGPUShaderSourceSPIRV, ::WGPUShaderSourceSPIRV> {
    public:
    //TODO change to vector
        static WGPUShaderSourceSPIRV* Obtain();
//        void SetCode(const int* code, int codeSize);
//        WGPUChainedStruct GetChain();
};

class WGPUSupportedWGSLLanguageFeatures : public WGPUObjectBase<WGPUSupportedWGSLLanguageFeatures, ::WGPUSupportedWGSLLanguageFeatures> {
    public:
        static WGPUSupportedWGSLLanguageFeatures* Obtain();
        int GetFeatureCount();
        WGPUWGSLLanguageFeatureName GetFeatureAt(int index);
};

class WGPUCompilationInfo : public WGPUObjectBase<WGPUCompilationInfo, ::WGPUCompilationInfo> {
    public:
        int GetMessageCount();
        WGPUCompilationMessage GetMessage(int index);
};

class WGPUCompilationMessage : public WGPUObjectBase<WGPUCompilationMessage, ::WGPUCompilationMessage> {
    public:
        static WGPUCompilationMessage* Obtain();
        std::string GetMessage();
        WGPUCompilationMessageType GetType();
        int GetLineNum();
        int GetLinePos();
        int GetOffset();
        int GetLength();
};

// ################################### DESCRIPTOR STRUCTS ###################################

class WGPUInstanceDescriptor : public WGPUObjectBase<WGPUInstanceDescriptor, ::WGPUInstanceDescriptor> {
    public:
        static WGPUInstanceDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
//        WGPUInstanceCapabilities GetFeatures(); // TODO dawn have different code
};

class WGPURenderBundleDescriptor : public WGPUObjectBase<WGPURenderBundleDescriptor, ::WGPURenderBundleDescriptor> {
    public:
        static WGPURenderBundleDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
};

class WGPURenderBundleEncoderDescriptor : public WGPUObjectBase<WGPURenderBundleEncoderDescriptor, ::WGPURenderBundleEncoderDescriptor> {
    public:
        static WGPURenderBundleEncoderDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetColorFormats(WGPUVectorTextureFormat* colorFormats);
        void SetDepthStencilFormat(WGPUTextureFormat depthStencilFormat);
        void SetSampleCount(int sampleCount);
        void SetDepthReadOnly(int depthReadOnly);
        void SetStencilReadOnly(int stencilReadOnly);
};

class WGPUQuerySetDescriptor : public WGPUObjectBase<WGPUQuerySetDescriptor, ::WGPUQuerySetDescriptor> {
    public:
        static WGPUQuerySetDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetType(WGPUQueryType type);
        void SetCount(int count);
};

class WGPUSamplerDescriptor : public WGPUObjectBase<WGPUSamplerDescriptor, ::WGPUSamplerDescriptor> {
    public:
        static WGPUSamplerDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
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

class WGPUBindGroupLayoutDescriptor : public WGPUObjectBase<WGPUBindGroupLayoutDescriptor, ::WGPUBindGroupLayoutDescriptor> {
    public:
        static WGPUBindGroupLayoutDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetEntries(WGPUVectorBindGroupLayoutEntry* entries);
};

class WGPUProgrammableStageDescriptor : public WGPUObjectBase<WGPUProgrammableStageDescriptor, ::WGPUProgrammableStageDescriptor*> {
    public:
        static WGPUProgrammableStageDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
};

class WGPUComputePipelineDescriptor : public WGPUObjectBase<WGPUComputePipelineDescriptor, ::WGPUComputePipelineDescriptor> {
    public:
        static WGPUComputePipelineDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        WGPUProgrammableStageDescriptor GetCompute();
};

class WGPUQueueDescriptor : public WGPUObjectBase<WGPUQueueDescriptor, ::WGPUQueueDescriptor*> {
    public:
        static WGPUQueueDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
};

class WGPUBufferDescriptor : public WGPUObjectBase<WGPUBufferDescriptor, ::WGPUBufferDescriptor> {
    public:
        static WGPUBufferDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetUsage(WGPUBufferUsage usage);
        void SetSize(int size);
        void SetMappedAtCreation(bool mappedAtCreation);
};

class WGPUBindGroupDescriptor : public WGPUObjectBase<WGPUBindGroupDescriptor, ::WGPUBindGroupDescriptor> {
    public:
        static WGPUBindGroupDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetLayout(WGPUBindGroupLayout* layout);
        void SetEntries(WGPUVectorBindGroupEntry* entries);
};

class WGPUPipelineLayoutDescriptor : public WGPUObjectBase<WGPUPipelineLayoutDescriptor, ::WGPUPipelineLayoutDescriptor> {
    public:
        static WGPUPipelineLayoutDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetBindGroupLayouts(WGPUVectorBindGroupLayout* bindGroupLayouts);
};

class WGPUDeviceDescriptor : public WGPUObjectBase<WGPUDeviceDescriptor, ::WGPUDeviceDescriptor> {
    public:
        static WGPUDeviceDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetRequiredLimits(WGPULimits* limits);
        void SetRequiredFeatures(WGPUVectorFeatureName* features);
        WGPUQueueDescriptor GetDefaultQueue();
};

class WGPUShaderModuleDescriptor : public WGPUObjectBase<WGPUShaderModuleDescriptor, ::WGPUShaderModuleDescriptor> {
    public:
        static WGPUShaderModuleDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
};

class WGPURenderPipelineDescriptor : public WGPUObjectBase<WGPURenderPipelineDescriptor, ::WGPURenderPipelineDescriptor> {
    public:
        static WGPURenderPipelineDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        WGPUVertexState GetVertex();
        WGPUPrimitiveState GetPrimitive();
        void SetFragment(WGPUFragmentState* fragment);
        void SetDepthStencil(WGPUDepthStencilState* depthStencilState);
        WGPUMultisampleState GetMultisample();
        void SetLayout(WGPUPipelineLayout* pipelineLayout);
};

class WGPUCommandEncoderDescriptor : public WGPUObjectBase<WGPUCommandEncoderDescriptor, ::WGPUCommandEncoderDescriptor> {
    public:
        static WGPUCommandEncoderDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
};

class WGPUCommandBufferDescriptor : public WGPUObjectBase<WGPUCommandBufferDescriptor, ::WGPUCommandBufferDescriptor> {
    public:
        static WGPUCommandBufferDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
};

class WGPURenderPassDescriptor : public WGPUObjectBase<WGPURenderPassDescriptor, ::WGPURenderPassDescriptor> {
    public:
        static WGPURenderPassDescriptor* Obtain();
        void Reset();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetColorAttachments(WGPUVectorRenderPassColorAttachment* values);
        void SetDepthStencilAttachment(WGPURenderPassDepthStencilAttachment* attachment);
        void SetOcclusionQuerySet(WGPUQuerySet* occlusionQuerySet);
        void SetTimestampWrites(WGPURenderPassTimestampWrites* timestampWrites);
};

class WGPUComputePassDescriptor : public WGPUObjectBase<WGPUComputePassDescriptor, ::WGPUComputePassDescriptor> {
    public:
        static WGPUComputePassDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetTimestampWrites(WGPUComputePassTimestampWrites* timestampWrites);
};

class WGPUTextureDescriptor : public WGPUObjectBase<WGPUTextureDescriptor, ::WGPUTextureDescriptor> {
    public:
        static WGPUTextureDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
        void SetLabel(const char* value);
        void SetUsage(WGPUTextureUsage usage);
        void SetDimension(WGPUTextureDimension dimension);
        WGPUExtent3D GetSize();
        void SetFormat(WGPUTextureFormat format);
        void SetMipLevelCount(int mipLevelCount);
        void SetSampleCount(int sampleCount);
        void SetViewFormats(WGPUVectorTextureFormat* viewFormats);
};

class WGPUTextureViewDescriptor : public WGPUObjectBase<WGPUTextureViewDescriptor, ::WGPUTextureViewDescriptor> {
    public:
        static WGPUTextureViewDescriptor* Obtain();
        void SetNextInChain(WGPUChainedStruct* chainedStruct);
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

class WGPUSampler : public WGPUObjectBase<WGPUSampler, ::WGPUSampler> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
};

class WGPURenderBundleEncoder : public WGPUObjectBase<WGPURenderBundleEncoder, ::WGPURenderBundleEncoder> {
// TODO implement methods
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WGPURenderBundleEncoder* Obtain();
        void SetPipeline(WGPURenderPipeline* renderPipeline);
        void Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance);
        void DrawIndexed(int indexCount, int instanceCount, int firstIndex, int baseVertex, int firstInstance);
        void DrawIndirect(WGPUBuffer* indirectBuffer, int indirectOffset);
        void DrawIndexedIndirect(WGPUBuffer* indirectBuffer, int indirectOffset);
        void SetBindGroup(int groupIndex, WGPUBindGroup* group, WGPUVectorInt* dynamicOffsets);
        void SetVertexBuffer(int slot, WGPUBuffer* buffer, int offset, int size);
        void SetIndexBuffer(WGPUBuffer* buffer, WGPUIndexFormat format, int offset, int size);
        void InsertDebugMarker(const char* label);
        void PopDebugGroup();
        void PushDebugGroup(const char* label);
        void SetLabel(const char* label);
        void Finish(WGPURenderBundleDescriptor* descriptor, WGPURenderBundle* bundle);
};

class WGPUTextureView : public WGPUObjectBase<WGPUTextureView, ::WGPUTextureView> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WGPUTextureView* Obtain();
};

class WGPUTexture : public WGPUObjectBase<WGPUTexture, ::WGPUTexture> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WGPUTexture* Obtain();
        void CreateView(WGPUTextureViewDescriptor* textureViewDescriptor, WGPUTextureView* textureView);
        WGPUTextureFormat GetFormat();
        void Destroy();
};

class WGPUShaderModule : public WGPUObjectBase<WGPUShaderModule, ::WGPUShaderModule> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WGPUShaderModule* Obtain();
        bool IsValid();
};

class WGPURenderPipeline : public WGPUObjectBase<WGPURenderPipeline, ::WGPURenderPipeline> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WGPURenderPipeline* Obtain();
        bool IsValid();
};

class WGPURenderPassEncoder : public WGPUObjectBase<WGPURenderPassEncoder, ::WGPURenderPassEncoder> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WGPURenderPassEncoder* Obtain();
        void End();
        void SetPipeline(WGPURenderPipeline* renderPipeline);
        void BeginOcclusionQuery(int queryIndex);
        void Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance);
        void DrawIndexed(int indexCount, int instanceCount, int firstIndex, int baseVertex, int firstInstance);
        void DrawIndexedIndirect(WGPUBuffer* indirectBuffer, int indirectOffset);
        void DrawIndirect(WGPUBuffer* indirectBuffer, int indirectOffset);
        void EndOcclusionQuery();
        void ExecuteBundles(WGPUVectorRenderBundle* values);
        void InsertDebugMarker(const char* label);
        void PopDebugGroup();
        void PushDebugGroup(const char* label);
        void SetBindGroup(int groupIndex, WGPUBindGroup* group, WGPUVectorInt* dynamicOffsets);
        void SetBlendConstant(WGPUColor* color);
        void SetIndexBuffer(WGPUBuffer* buffer, WGPUIndexFormat format, int offset, int size);
        void SetLabel(const char* label);
        void SetScissorRect(int x, int y, int width, int height);
        void SetStencilReference(int reference);
        void SetVertexBuffer(int slot, WGPUBuffer* buffer, int offset, int size);
        void SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth);
};

class WGPUQuerySet : public WGPUObjectBase<WGPUQuerySet, ::WGPUQuerySet> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void Destroy();
        int GetCount();
        WGPUQueryType GetType();
        void SetLabel(const char* label);
};

class WGPUPipelineLayout : public WGPUObjectBase<WGPUPipelineLayout, ::WGPUPipelineLayout> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
};

class WGPUInstance : public WGPUObjectBase<WGPUInstance, ::WGPUInstance> {
    private:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        bool IsValid();
        void RequestAdapter(WGPURequestAdapterOptions* options, WGPUCallbackMode mode, RequestAdapterCallback* callback);
        WGPUSurface* CreateWebSurface(const char* canvas);
        WGPUSurface* CreateWindowsSurface(void * hwnd);
        WGPUSurface* CreateLinuxSurface(bool isWayland, void * windowOrSurface, void* display);
        WGPUSurface* CreateMacSurface(void * windowHandle);
        WGPUSurface* CreateAndroidSurface(WGPUAndroidWindow* window);
        void ProcessEvents();
        WGPUWaitStatus WaitAny(WGPUVectorFutureWaitInfo* futureVector, int timeoutNS);
};

class WGPUComputePassEncoder : public WGPUObjectBase<WGPUComputePassEncoder, ::WGPUComputePassEncoder> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WGPUComputePassEncoder* Obtain();
        void SetDispatchWorkgroups(int workgroupCountX, int workgroupCountY, int workgroupCountZ);
        void DispatchWorkgroupsIndirect(WGPUBuffer* indirectBuffer, int indirectOffset);
        void End();
        void InsertDebugMarker(const char* markerLabel);
        void PopDebugGroup();
        void PushDebugGroup(const char* groupLabel);
        void SetBindGroup(int groupIndex, WGPUBindGroup* group, WGPUVectorInt* offsets);
        void SetLabel(const char* label);
        void SetPipeline(WGPUComputePipeline* pipeline);
};

class WGPUCommandBuffer : public WGPUObjectBase<WGPUCommandBuffer, ::WGPUCommandBuffer> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WGPUCommandBuffer* Obtain();
};

class WGPUCommandEncoder : public WGPUObjectBase<WGPUCommandEncoder, ::WGPUCommandEncoder> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        static WGPUCommandEncoder* Obtain();
        void BeginComputePass(WGPUComputePassDescriptor* descriptor, WGPUComputePassEncoder* encoder);
        void BeginRenderPass(WGPURenderPassDescriptor* renderPassDescriptor, WGPURenderPassEncoder* encoder);
        void ClearBuffer(WGPUBuffer* buffer, int offset, int size);
        void CopyBufferToBuffer(WGPUBuffer* source, int sourceOffset, WGPUBuffer* destination, int destinationOffset, int size);
        void CopyBufferToTexture(WGPUTexelCopyBufferInfo* source, WGPUTexelCopyTextureInfo* destination, WGPUExtent3D* copySize);
        void TextureToBuffer(WGPUTexelCopyTextureInfo* source, WGPUTexelCopyBufferInfo* destination, WGPUExtent3D* copySize);
        void CopyTextureToTexture(WGPUTexelCopyTextureInfo* source, WGPUTexelCopyTextureInfo* destination, WGPUExtent3D* copySize);
        void Finish(WGPUCommandBufferDescriptor* commandBufferDescriptor, WGPUCommandBuffer* commandBuffer);
        void InsertDebugMarker(const char* markerLabel);
        void PopDebugGroup();
        void PushDebugGroup(const char* groupLabel);
        void ResolveQuerySet(WGPUQuerySet* querySet, int firstQuery, int queryCount, WGPUBuffer* destination, int destinationOffset);
        void SetLabel(const char* label);
        void WriteTimestamp(WGPUQuerySet* querySet, int queryIndex);
};

class WGPUBuffer : public WGPUObjectBase<WGPUBuffer, ::WGPUBuffer> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void Unmap();
        int GetSize();
        WGPUBufferUsage GetUsage();
        WGPUFuture MapAsync(WGPUMapMode mode, int offset, int size, WGPUCallbackMode callbackMode, BufferMapCallback* callback);
        WGPUByteBuffer GetConstMappedRange(int offset, int size);
        void Destroy();
};

class WGPUBindGroup : public WGPUObjectBase<WGPUBindGroup, ::WGPUBindGroup> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
};

class WGPUBindGroupLayout : public WGPUObjectBase<WGPUBindGroupLayout, ::WGPUBindGroupLayout> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
};

class WGPUComputePipeline : public WGPUObjectBase<WGPUComputePipeline, ::WGPUComputePipeline> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
        WGPUBindGroupLayout GetBindGroupLayout(int groupIndex);
};

class WGPURenderBundle : public WGPUObjectBase<WGPURenderBundle, ::WGPURenderBundle> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
};

class WGPUAdapter : public WGPUObjectBase<WGPUAdapter, ::WGPUAdapter> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void RequestDevice(WGPUDeviceDescriptor* descriptor, WGPUCallbackMode mode, RequestDeviceCallback* callback, UncapturedErrorCallback* errorCallback);
        bool GetInfo(WGPUAdapterInfo* adapterInfo);
        bool HasFeature(WGPUFeatureName featureName);
        WGPUStatus GetLimits(WGPULimits* limits);
};

class WGPUSurface : public WGPUObjectBase<WGPUSurface, ::WGPUSurface> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void Unconfigure();
        void Configure(WGPUSurfaceConfiguration* config);
        void GetCapabilities(WGPUAdapter* adapter, WGPUSurfaceCapabilities* surfaceCapabilities);
        void GetCurrentTexture(WGPUSurfaceTexture* tex);
        void Present();
};

class WGPUQueue : public WGPUObjectBase<WGPUQueue, ::WGPUQueue> {
    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void SetLabel(const char* value);
        void Submit(int commandCount, WGPUCommandBuffer* commandBuffer);
        void WriteBuffer(WGPUBuffer* buffer, int bufferOffset, WGPUByteBuffer* bytes, int dataSize = -1);
        void WriteBuffer(WGPUBuffer* buffer, int bufferOffset, WGPUFloatBuffer* bytes, int dataSize = -1);
        void WriteBuffer(WGPUBuffer* buffer, int bufferOffset, void const * data, int size);
        void WriteTexture(WGPUTexelCopyTextureInfo* destination, WGPUByteBuffer* bytes, WGPUTexelCopyBufferLayout* dataLayout, WGPUExtent3D* writeSize);
};

class WGPUDevice : public WGPUObjectBase<WGPUDevice, ::WGPUDevice> {
    private:
        WGPUQueue defaultQueue;

    protected:
        void AddRefInternal();
        void ReleaseInternal();
    public:
        void CreateBindGroup(WGPUBindGroupDescriptor* descriptor, WGPUBindGroup* valueOut);
        void CreateBindGroupLayout(WGPUBindGroupLayoutDescriptor* descriptor, WGPUBindGroupLayout* valueOut);
        void CreateBuffer(WGPUBufferDescriptor* descriptor, WGPUBuffer* valueOut);
        void CreateCommandEncoder(WGPUCommandEncoderDescriptor* descriptor, WGPUCommandEncoder* valueOut);
        void CreateComputePipeline(WGPUComputePipelineDescriptor* descriptor, WGPUComputePipeline* valueOut);
        void CreatePipelineLayout(WGPUPipelineLayoutDescriptor* descriptor, WGPUPipelineLayout* valueOut);
        void CreateQuerySet(WGPUQuerySetDescriptor* descriptor, WGPUQuerySet* valueOut);
        void CreateRenderBundleEncoder(WGPURenderBundleEncoderDescriptor* descriptor, WGPURenderBundleEncoder* valueOut);
        void CreateRenderPipeline(WGPURenderPipelineDescriptor* pipelineDescriptor, WGPURenderPipeline* valueOut);
        void CreateSampler(WGPUSamplerDescriptor* descriptor, WGPUSampler* valueOut);
        void CreateShaderModule(WGPUShaderModuleDescriptor* shaderModuleDescriptor, WGPUShaderModule* valueOut);
        void CreateTexture(WGPUTextureDescriptor* descriptor, WGPUTexture* valueOut);
        void Destroy();

        void GetFeatures(WGPUSupportedFeatures* features);
        bool HasFeature(WGPUFeatureName feature);
        void GetLimits(WGPULimits* limits);
        WGPUQueue* GetQueue();
};

} // JGPU

class WGPU {
    public:
        static WGPUPlatformType GetPlatformType();
        static JGPU::WGPUInstance* CreateInstance(JGPU::WGPUInstanceDescriptor* descriptor = NULL);
        static STBImage* loadImage(WGPUByteBuffer* buffer, int desiredChannels = 0);
};