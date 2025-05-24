package com.github.xpenatan.webgpu.backend.core;

import com.github.xpenatan.webgpu.JAdapter;
import com.github.xpenatan.webgpu.JAdapterInfo;
import com.github.xpenatan.webgpu.JDevice;
import com.github.xpenatan.webgpu.JDeviceDescriptor;
import com.github.xpenatan.webgpu.JInstance;
import com.github.xpenatan.webgpu.JLimits;
import com.github.xpenatan.webgpu.JQueue;
import com.github.xpenatan.webgpu.JRenderPipeline;
import com.github.xpenatan.webgpu.JRequestAdapterOptions;
import com.github.xpenatan.webgpu.JSupportedFeatures;
import com.github.xpenatan.webgpu.JSurface;
import com.github.xpenatan.webgpu.JWebGPU;
import com.github.xpenatan.webgpu.RequestAdapterCallback;
import com.github.xpenatan.webgpu.RequestDeviceCallback;
import com.github.xpenatan.webgpu.UncapturedErrorCallback;
import com.github.xpenatan.webgpu.WGPUAdapterType;
import com.github.xpenatan.webgpu.WGPUBackendType;
import com.github.xpenatan.webgpu.WGPUCallbackMode;
import com.github.xpenatan.webgpu.WGPUErrorType;
import com.github.xpenatan.webgpu.WGPUFeatureName;
import com.github.xpenatan.webgpu.WGPURequestAdapterStatus;
import com.github.xpenatan.webgpu.WGPURequestDeviceStatus;

public class WGPUApp {
    public int width;
    public int height;
    public JInstance instance;
    public JAdapter adapter;
    public JDevice device;
    public JSurface surface;
    public JRenderPipeline renderPipeline;
    public JQueue queue;

    private boolean isReady;

    public void init() {
        instance = new JInstance();
        JRequestAdapterOptions op = new JRequestAdapterOptions();
        WGPUCallbackMode mode = WGPUCallbackMode.AllowProcessEvents;
        RequestAdapterCallback callback = new RequestAdapterCallback() {
            @Override
            protected void OnCallback(WGPURequestAdapterStatus status, JAdapter adapter) {
                WGPUApp.this.adapter = adapter;
                JAdapterInfo info = new JAdapterInfo();
                if(adapter.GetInfo(info)) {
                    WGPUBackendType backendType = info.GetBackendType();
                    System.out.println("BackendType: " + backendType);
                    WGPUAdapterType adapterType = info.GetAdapterType();
                    System.out.println("AdapterType: " + adapterType);
                    String vendor = info.GetVendor().c_str();
                    System.out.println("Vendor: " + vendor);
                    String architecture = info.GetArchitecture().c_str();
                    System.out.println("Architecture: " + architecture);
                    String description = info.GetDescription().c_str();
                    System.out.println("Description: " + description);
                    String device = info.GetDevice().c_str();
                    System.out.println("Device: " + device);
                    System.out.println("Has Feature DepthClipControl: " + adapter.HasFeature(WGPUFeatureName.DepthClipControl));
                }

                JDeviceDescriptor deviceDescriptor = new JDeviceDescriptor();
//                JLimits limits = new JLimits();
//                setDefaultLimits(limits);
                deviceDescriptor.SetLabel("My Device");
                deviceDescriptor.SetRequiredLimits(null);
                deviceDescriptor.SetRequiredFeatureCount(0);
                deviceDescriptor.GetDefaultQueue().SetLabel("The default queue");

                adapter.RequestDevice(deviceDescriptor, mode, new RequestDeviceCallback() {
                    @Override
                    protected void OnCallback(WGPURequestDeviceStatus status, JDevice device) {
                        WGPUApp.this.device = device;
                        queue = device.GetQueue();
                        System.out.println("isReady: " + status);
                        System.out.println("Platform: " + JWebGPU.GetPlatformType());

                        JSupportedFeatures features = new JSupportedFeatures();
                        device.GetFeatures(features);
                        int featureCount = features.GetFeatureCount();
                        System.out.println("Total Features: " + featureCount);
                        for(int i = 0; i < featureCount; i++) {
                            WGPUFeatureName featureName = features.GetFeatureAt(i);
                            System.out.println("Feature name: " + featureName);
                        }
                        features.dispose();

                        JLimits limits = new JLimits();
                        device.GetLimits(limits);
                        System.out.println("Device limits: " + featureCount);
                        System.out.println("MaxTextureDimension1D: " + limits.GetMaxTextureDimension1D());
                        System.out.println("MaxTextureDimension2D: " + limits.GetMaxTextureDimension2D());
                        System.out.println("MaxTextureDimension3D: " + limits.GetMaxTextureDimension3D());
                        System.out.println("MaxTextureArrayLayers: " + limits.GetMaxTextureArrayLayers());
                        device.dispose();

                        isReady = true;
                    }
                }, new UncapturedErrorCallback() {
                    @Override
                    protected void OnCallback(WGPUErrorType errorType, String message) {
                        System.err.println("ErrorType: " + errorType);
                        System.err.println("Error Message: " + message);
                    }
                });
            }
        };
        instance.RequestAdapter(op, mode, callback);
    }

    public void update() {
        instance.ProcessEvents();
    }

    public boolean isReady() {
        return isReady;
    }

    final static int WGPU_LIMIT_U32_UNDEFINED = -1;
    final static int WGPU_LIMIT_U64_UNDEFINED = -1;

    public void setDefaultLimits (JLimits limits) {
        limits.SetMaxTextureDimension1D(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxTextureDimension2D(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxTextureDimension3D(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxTextureArrayLayers(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxBindGroups(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxBindGroupsPlusVertexBuffers(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxBindingsPerBindGroup(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxDynamicUniformBuffersPerPipelineLayout(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxDynamicStorageBuffersPerPipelineLayout(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxSampledTexturesPerShaderStage(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxSamplersPerShaderStage(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxStorageBuffersPerShaderStage(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxStorageTexturesPerShaderStage(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxUniformBuffersPerShaderStage(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxUniformBufferBindingSize(WGPU_LIMIT_U64_UNDEFINED);
        limits.SetMaxStorageBufferBindingSize(WGPU_LIMIT_U64_UNDEFINED);
        limits.SetMinUniformBufferOffsetAlignment(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMinStorageBufferOffsetAlignment(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxVertexBuffers(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxBufferSize(WGPU_LIMIT_U64_UNDEFINED);
        limits.SetMaxVertexAttributes(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxVertexBufferArrayStride(WGPU_LIMIT_U32_UNDEFINED);
//        limits.SetMaxInterStageShaderComponents(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxInterStageShaderVariables(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxColorAttachments(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxColorAttachmentBytesPerSample(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxComputeWorkgroupStorageSize(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxComputeInvocationsPerWorkgroup(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxComputeWorkgroupSizeX(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxComputeWorkgroupSizeY(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxComputeWorkgroupSizeZ(WGPU_LIMIT_U32_UNDEFINED);
        limits.SetMaxComputeWorkgroupsPerDimension(WGPU_LIMIT_U32_UNDEFINED);
    }

}