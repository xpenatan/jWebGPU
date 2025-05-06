#pragma once

#include "IDLHelper.h"
//#include "webgpu/webgpu.h"
#include "webgpu/webgpu_cpp.h"
#include <iostream>

// ENUM

using WGSLLanguageFeatureName = wgpu::WGSLLanguageFeatureName;
using AdapterType = wgpu::AdapterType;
using AddressMode = wgpu::AddressMode;
using BackendType = wgpu::BackendType;
using BlendFactor = wgpu::BlendFactor;
using BlendOperation = wgpu::BlendOperation;
using BufferBindingType = wgpu::BufferBindingType;
using BufferMapState = wgpu::BufferMapState;
using CallbackMode = wgpu::CallbackMode;
using CompareFunction = wgpu::CompareFunction;
using CompilationInfoRequestStatus = wgpu::CompilationInfoRequestStatus;
using CompilationMessageType = wgpu::CompilationMessageType;
using CompositeAlphaMode = wgpu::CompositeAlphaMode;
using CreatePipelineAsyncStatus = wgpu::CreatePipelineAsyncStatus;
using CullMode = wgpu::CullMode;
using DeviceLostReason = wgpu::DeviceLostReason;
using ErrorFilter = wgpu::ErrorFilter;
using ErrorType = wgpu::ErrorType;
using FeatureLevel = wgpu::FeatureLevel;
using FeatureName = wgpu::FeatureName;
using FilterMode = wgpu::FilterMode;
using FrontFace = wgpu::FrontFace;
using IndexFormat = wgpu::IndexFormat;
using LoadOp = wgpu::LoadOp;
using MapAsyncStatus = wgpu::MapAsyncStatus;
using MipmapFilterMode = wgpu::MipmapFilterMode;
using PopErrorScopeStatus = wgpu::PopErrorScopeStatus;
using PowerPreference = wgpu::PowerPreference;
using PredefinedColorSpace = wgpu::PredefinedColorSpace;
using PresentMode = wgpu::PresentMode;
using PrimitiveTopology = wgpu::PrimitiveTopology;
using QueryType = wgpu::QueryType;
using QueueWorkDoneStatus = wgpu::QueueWorkDoneStatus;
using RequestAdapterStatus = wgpu::RequestAdapterStatus;
using RequestDeviceStatus = wgpu::RequestDeviceStatus;
using SType = wgpu::SType;
using SamplerBindingType = wgpu::SamplerBindingType;
using Status = wgpu::Status;
using StencilOperation = wgpu::StencilOperation;
using StorageTextureAccess = wgpu::StorageTextureAccess;
using StoreOp = wgpu::StoreOp;
using SubgroupMatrixComponentType = wgpu::SubgroupMatrixComponentType;
using SurfaceGetCurrentTextureStatus = wgpu::SurfaceGetCurrentTextureStatus;
using TextureAspect = wgpu::TextureAspect;
using TextureDimension = wgpu::TextureDimension;
using TextureFormat = wgpu::TextureFormat;
using TextureSampleType = wgpu::TextureSampleType;
using TextureViewDimension = wgpu::TextureViewDimension;
using ToneMappingMode = wgpu::ToneMappingMode;
using VertexFormat = wgpu::VertexFormat;
using VertexStepMode = wgpu::VertexStepMode;
using WaitStatus = wgpu::WaitStatus;
using BufferUsage = wgpu::BufferUsage;
using ColorWriteMask = wgpu::ColorWriteMask;
using MapMode = wgpu::MapMode;
using ShaderStage = wgpu::ShaderStage;
using TextureUsage = wgpu::TextureUsage;

using Instance = wgpu::Instance;
using Future = wgpu::Future;
using Adapter = wgpu::Adapter;
using RequestAdapterOptions = wgpu::RequestAdapterOptions;

class RequestAdapterCallback {
public:
    virtual void OnCallback(RequestAdapterStatus status, Adapter* adapter) = 0;

};

class JInstance {

    public:
        wgpu::Instance instance_;

        JInstance() {
             instance_ = wgpu::CreateInstance();
        }

        void RequestAdapter(RequestAdapterOptions* options, CallbackMode mode, RequestAdapterCallback* callback) {
            // There is a problem here can keeping instance reference the callback is not working

            wgpu::Instance result = wgpu::CreateInstance();
            Future future = result.RequestAdapter(options, mode,
                    [&](wgpu::RequestAdapterStatus status, wgpu::Adapter ad, wgpu::StringView message) {
                         std::cout << "CallBack 1: " << std::endl;
                    });
//            instance_.WaitAny(future, UINT64_MAX);

//            wgpu::Instance instance = wgpu::CreateInstance();
//            wgpu::RequestAdapterOptions op = {};
//            instance.RequestAdapter(&op, wgpu::CallbackMode::WaitAnyOnly,
//                    [&](wgpu::RequestAdapterStatus status, wgpu::Adapter ad, wgpu::StringView message) {
//                         std::cout << "CallBack Set111: " << std::endl;
//                    });
        }
};

class InstanceDescriptor {

    public:
        WGPUInstanceDescriptor * descriptor;
};

class JWebGPU {
    private:

    public:
        static void Set() {
            wgpu::Instance instance = wgpu::CreateInstance();
            wgpu::RequestAdapterOptions options = {};
            instance.RequestAdapter(&options, wgpu::CallbackMode::WaitAnyOnly,
                    [&](wgpu::RequestAdapterStatus status, wgpu::Adapter ad, wgpu::StringView message) {
                         std::cout << "CallBack Set: " << std::endl;
                    });
        }

//        static JInstance* CreateInstance() {
//            JInstance* instance = new JInstance();
//            auto result = wgpu::CreateInstance();
//            instance->instance_ = result;
//            return instance;
//        }
};

