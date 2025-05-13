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

using WInstance = wgpu::Instance;
using WFuture = wgpu::Future;
using WAdapter = wgpu::Adapter;
using WRequestAdapterOptions = wgpu::RequestAdapterOptions;
using WDeviceDescriptor = wgpu::DeviceDescriptor;
using WDevice = wgpu::Device;
using WQueue = wgpu::Queue;

class JAdapter;
class JDevice;

class RequestAdapterCallback {
public:
    virtual void OnCallback(RequestAdapterStatus status, JAdapter* adapter) = 0;

};

class RequestDeviceCallback {
public:
    virtual void OnCallback(RequestDeviceStatus status, JDevice* device) = 0;

};

class JDevice {

    private:

    public:
        WDevice device;

        JDevice() {
        }

        WDevice& Get() {
            return device;
        }
};

class JAdapter {

    private:
        RequestDeviceCallback* callback;

        static void onDeviceRequest(RequestDeviceStatus status, WDevice dev, wgpu::StringView message, JAdapter* userdata) {
            JDevice* device = new JDevice();
            device->device = std::move(dev);
            userdata->callback->OnCallback(status, device);
            userdata->callback = NULL;
        }

    public:
        WAdapter adapter;

        JAdapter() {
        }

        WAdapter& Get() {
            return adapter;
        }

        void RequestDevice(WDeviceDescriptor* options, CallbackMode mode, RequestDeviceCallback* callback) {
            this->callback = callback;
            adapter.RequestDevice(options, mode, onDeviceRequest, this);
        }
};


class JInstance {

    private:
        WInstance instance;
        RequestAdapterCallback* callback;

        static void onAdapterRequest(RequestAdapterStatus status, WAdapter ad, wgpu::StringView message, JInstance* userdata) {
            JAdapter* adapter = new JAdapter();
            adapter->adapter = std::move(ad);
            userdata->callback->OnCallback(status, adapter);
            userdata->callback = NULL;
        }

    public:
        JInstance() {
             instance = wgpu::CreateInstance();
        }

        WInstance& Get() {
            return instance;
        }

        void RequestAdapter(WRequestAdapterOptions* options, CallbackMode mode, RequestAdapterCallback* callback) {
            if(instance) {
                this->callback = callback;
                WFuture future = instance.RequestAdapter(options, mode, onAdapterRequest, this);
            }
        }
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

