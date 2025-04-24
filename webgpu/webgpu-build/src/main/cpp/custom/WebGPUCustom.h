#pragma once

#include "IDLHelper.h"
//#include "dawn/webgpu.h"
#include "webgpu/webgpu.h"
//#include "webgpu/webgpu_cpp.h"
#include <iostream>


//
//using AdapterType = wgpu::AdapterType;
//using AddressMode = wgpu::AddressMode;
//using AlphaMode = wgpu::AlphaMode;
//using BackendType = wgpu::BackendType;
//using BlendFactor = wgpu::BlendFactor;
//using BlendOperation = wgpu::BlendOperation;
//using BufferBindingType = wgpu::BufferBindingType;
//using BufferMapAsyncStatus = wgpu::BufferMapAsyncStatus;
//using BufferMapState = wgpu::BufferMapState;
//using CallbackMode = wgpu::CallbackMode;
//using CompareFunction = wgpu::CompareFunction;
//using CompilationInfoRequestStatus = wgpu::CompilationInfoRequestStatus;
//using CompilationMessageType = wgpu::CompilationMessageType;
//using CompositeAlphaMode = wgpu::CompositeAlphaMode;
//using CreatePipelineAsyncStatus = wgpu::CreatePipelineAsyncStatus;
//using CullMode = wgpu::CullMode;
//using DeviceLostReason = wgpu::DeviceLostReason;
//using ErrorFilter = wgpu::ErrorFilter;
//using ErrorType = wgpu::ErrorType;
//using ExternalTextureRotation = wgpu::ExternalTextureRotation;
//using FeatureName = wgpu::FeatureName;
//using FilterMode = wgpu::FilterMode;
//using FrontFace = wgpu::FrontFace;
//using IndexFormat = wgpu::IndexFormat;
//using LoadOp = wgpu::LoadOp;
//using LoggingType = wgpu::LoggingType;
//using MapAsyncStatus = wgpu::MapAsyncStatus;
//using MipmapFilterMode = wgpu::MipmapFilterMode;
//using PopErrorScopeStatus = wgpu::PopErrorScopeStatus;
//using PowerPreference = wgpu::PowerPreference;
//using PresentMode = wgpu::PresentMode;
//using PrimitiveTopology = wgpu::PrimitiveTopology;
//using QueryType = wgpu::QueryType;
//using QueueWorkDoneStatus = wgpu::QueueWorkDoneStatus;
//using RequestDeviceStatus = wgpu::RequestDeviceStatus;
//using SType = wgpu::SType;
//using SamplerBindingType = wgpu::SamplerBindingType;
//using SharedFenceType = wgpu::SharedFenceType;
//using Status = wgpu::Status;
//using StencilOperation = wgpu::StencilOperation;
//using StorageTextureAccess = wgpu::StorageTextureAccess;
//using StoreOp = wgpu::StoreOp;
//using SurfaceGetCurrentTextureStatus = wgpu::SurfaceGetCurrentTextureStatus;
//using TextureAspect = wgpu::TextureAspect;
//using TextureDimension = wgpu::TextureDimension;
//using TextureFormat = wgpu::TextureFormat;
//using TextureSampleType = wgpu::TextureSampleType;
//using TextureViewDimension = wgpu::TextureViewDimension;
//using VertexFormat = wgpu::VertexFormat;
//using VertexStepMode = wgpu::VertexStepMode;
//using WaitStatus = wgpu::WaitStatus;
//using BufferUsage = wgpu::BufferUsage;
//using ColorWriteMask = wgpu::ColorWriteMask;
//using HeapProperty = wgpu::HeapProperty;
//using MapMode = wgpu::MapMode;
//using ShaderStage = wgpu::ShaderStage;
//using TextureUsage = wgpu::TextureUsage;
//
//using BindGroup = wgpu::BindGroup;
//using BindGroupDescriptor = wgpu::BindGroupDescriptor;
//using BindGroupEntry = wgpu::BindGroupEntry;
//using BindGroupLayout = wgpu::BindGroupLayout;
//using BindGroupLayoutDescriptor = wgpu::BindGroupLayoutDescriptor;
//using BindGroupLayoutEntry = wgpu::BindGroupLayoutEntry;
//using Bool = wgpu::Bool;
//using Buffer = wgpu::Buffer;
//using BufferDescriptor = wgpu::BufferDescriptor;
//using Color = wgpu::Color;
//using CommandBuffer = wgpu::CommandBuffer;
//using CommandBufferDescriptor = wgpu::CommandBufferDescriptor;
//using CommandEncoder = wgpu::CommandEncoder;
//using CommandEncoderDescriptor = wgpu::CommandEncoderDescriptor;
//using CompilationInfo = wgpu::CompilationInfo;
//using ComputePassDescriptor = wgpu::ComputePassDescriptor;
//using ComputePassEncoder = wgpu::ComputePassEncoder;
//using ComputePipeline = wgpu::ComputePipeline;
//using ComputePipelineDescriptor = wgpu::ComputePipelineDescriptor;
//using ConvertibleStatus = wgpu::ConvertibleStatus;
//using CopyTextureForBrowserOptions = wgpu::CopyTextureForBrowserOptions;
//using CreateComputePipelineAsyncCallbackInfo = wgpu::CreateComputePipelineAsyncCallbackInfo;
//using CreateRenderPipelineAsyncCallbackInfo = wgpu::CreateRenderPipelineAsyncCallbackInfo;
//using Device = wgpu::Device;
//using Extent3D = wgpu::Extent3D;
//using ExternalTexture = wgpu::ExternalTexture;
//using ExternalTextureDescriptor = wgpu::ExternalTextureDescriptor;
//using Future = wgpu::Future;
//using FutureWaitInfo = wgpu::FutureWaitInfo;
//using ImageCopyExternalTexture = wgpu::ImageCopyExternalTexture;
//using ImageCopyTexture = wgpu::ImageCopyTexture;
//using InstanceFeatures = wgpu::InstanceFeatures;
//using PipelineLayout = wgpu::PipelineLayout;
//using PipelineLayoutDescriptor = wgpu::PipelineLayoutDescriptor;
//using PopErrorScopeCallbackInfo = wgpu::PopErrorScopeCallbackInfo;
//using CompilationInfoCallbackInfo = wgpu::CompilationInfoCallbackInfo;
//using Queue = wgpu::Queue;
//using QuerySet = wgpu::QuerySet;
//using QuerySetDescriptor = wgpu::QuerySetDescriptor;
//using QueueWorkDoneCallbackInfo = wgpu::QueueWorkDoneCallbackInfo;
//using RenderBundle = wgpu::RenderBundle;
//using RenderBundleDescriptor = wgpu::RenderBundleDescriptor;
//using RenderBundleEncoder = wgpu::RenderBundleEncoder;
//using RenderBundleEncoderDescriptor = wgpu::RenderBundleEncoderDescriptor;
//using RenderPassDescriptor = wgpu::RenderPassDescriptor;
//using RenderPassEncoder = wgpu::RenderPassEncoder;
//using RenderPipeline = wgpu::RenderPipeline;
//using RenderPipelineDescriptor = wgpu::RenderPipelineDescriptor;
//using RequestAdapterCallbackInfo = wgpu::RequestAdapterCallbackInfo;
//using DeviceDescriptor = wgpu::DeviceDescriptor;
//using Sampler = wgpu::Sampler;
//using SamplerDescriptor = wgpu::SamplerDescriptor;
//using ShaderModule = wgpu::ShaderModule;
//using ShaderModuleDescriptor = wgpu::ShaderModuleDescriptor;
//using SharedBufferMemory = wgpu::SharedBufferMemory;
//using SharedBufferMemoryBeginAccessDescriptor = wgpu::SharedBufferMemoryBeginAccessDescriptor;
//using SharedBufferMemoryDescriptor = wgpu::SharedBufferMemoryDescriptor;
//using SharedBufferMemoryEndAccessState = wgpu::SharedBufferMemoryEndAccessState;
//using SharedBufferMemoryProperties = wgpu::SharedBufferMemoryProperties;
//using SharedFence = wgpu::SharedFence;
//using SharedFenceDescriptor = wgpu::SharedFenceDescriptor;
//using SharedFenceExportInfo = wgpu::SharedFenceExportInfo;
//using SharedTextureMemory = wgpu::SharedTextureMemory;
//using SharedTextureMemoryBeginAccessDescriptor = wgpu::SharedTextureMemoryBeginAccessDescriptor;
//using SharedTextureMemoryDescriptor = wgpu::SharedTextureMemoryDescriptor;
//using SharedTextureMemoryEndAccessState = wgpu::SharedTextureMemoryEndAccessState;
//using SharedTextureMemoryProperties = wgpu::SharedTextureMemoryProperties;
//using SupportedLimits = wgpu::SupportedLimits;
//using SwapChain = wgpu::SwapChain;
//using SwapChainDescriptor = wgpu::SwapChainDescriptor;
//using Texture = wgpu::Texture;
//using TextureDataLayout = wgpu::TextureDataLayout;
//using TextureDescriptor = wgpu::TextureDescriptor;
//using TextureView = wgpu::TextureView;
//using TextureViewDescriptor = wgpu::TextureViewDescriptor;
//using AHardwareBufferProperties = wgpu::AHardwareBufferProperties;

//using IDLArrayTextureFormat = IDLArray<TextureFormat*>;

//using RequestAdapterOptions = wgpu::RequestAdapterOptions;
//using Instance = wgpu::Instance;
//using InstanceDescriptor = wgpu::InstanceDescriptor;
//using Adapter = wgpu::Adapter;
//using AdapterInfo = wgpu::AdapterInfo;
//
//using Surface = wgpu::Surface;
//using SurfaceCapabilities = wgpu::SurfaceCapabilities;
//using SurfaceConfiguration = wgpu::SurfaceConfiguration;
//using SurfaceDescriptor = wgpu::SurfaceDescriptor;
//using SurfaceTexture = wgpu::SurfaceTexture;
//
//using RequestAdapterStatus = wgpu::RequestAdapterStatus;

class Adapter {

    public:
        WGPUAdapter adapter;
};


class RequestAdapterCallback {
public:
    virtual void OnCallback(WGPURequestAdapterStatus status, Adapter* adapter) = 0;

};

class Instance {
    public:
        WGPUInstance instance;

//        WGPUFuture RequestAdapter(WGPURequestAdapterOptions* options, WGPUCallbackMode mode, RequestAdapterCallback* callback) {
//            WGPURequestAdapterCallbackInfo callbackInfo = {};
//            callbackInfo.mode = mode;
//            callbackInfo.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, void* callback_param, void*) {
//                auto cb = reinterpret_cast<RequestAdapterCallback*>(callback_param);
//                Adapter* adp = new Adapter();
//                adp->adapter = adapter;
//                cb->OnCallback(status, adp);
//            };
//            callbackInfo.userdata1 = reinterpret_cast<void*>(+callback);
//            return wgpuInstanceRequestAdapter(instance, options, callbackInfo);
//        }
};

class InstanceDescriptor {

    public:
        WGPUInstanceDescriptor * descriptor;
};

class WebGPU {

    public:
        static void Set() {
//                dawnProcSetProcs(&dawn::native::GetProcs());
//                wgpu::InstanceDescriptor instanceDescriptor = {};
//                instanceDescriptor.nextInChain = nullptr;
//                instanceDescriptor.capabilities.timedWaitAnyEnable = true;
//                wgpu::Instance instance = wgpu::CreateInstance(&instanceDescriptor);
        }

        static Instance* CreateInstance(InstanceDescriptor* descriptor = NULL) {
            Instance* instance = new Instance();
            WGPUInstanceDescriptor* wgpuDescriptor = descriptor != NULL ? descriptor->descriptor : NULL;
            WGPUInstanceDescriptor instanceDesc = {  };
            instanceDesc.nextInChain = NULL;
            WGPUInstance wgpuInstance = wgpuCreateInstance(&instanceDesc);
            instance->instance = wgpuInstance;
            return instance;
        }

//        static WGPUFuture* wgpuInstanceRequestAdapter(WGPUInstanceDescriptor* descriptor = NULL) {
//            return wgpuCreateInstance(descriptor);
//        }
};

