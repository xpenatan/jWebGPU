package com.github.xpenatan.webgpu.backend.core;

import com.github.xpenatan.webgpu.WGPU;
import com.github.xpenatan.webgpu.WGPUAdapter;
import com.github.xpenatan.webgpu.WGPUAdapterInfo;
import com.github.xpenatan.webgpu.WGPUAdapterType;
import com.github.xpenatan.webgpu.WGPUBackendType;
import com.github.xpenatan.webgpu.WGPUCallbackMode;
import com.github.xpenatan.webgpu.WGPUDevice;
import com.github.xpenatan.webgpu.WGPUDeviceDescriptor;
import com.github.xpenatan.webgpu.WGPUErrorType;
import com.github.xpenatan.webgpu.WGPUFeatureName;
import com.github.xpenatan.webgpu.WGPUInstance;
import com.github.xpenatan.webgpu.WGPUInstanceDescriptor;
import com.github.xpenatan.webgpu.WGPULimits;
import com.github.xpenatan.webgpu.WGPUPowerPreference;
import com.github.xpenatan.webgpu.WGPUQueue;
import com.github.xpenatan.webgpu.WGPURequestAdapterCallback;
import com.github.xpenatan.webgpu.WGPURequestAdapterOptions;
import com.github.xpenatan.webgpu.WGPURequestAdapterStatus;
import com.github.xpenatan.webgpu.WGPURequestDeviceCallback;
import com.github.xpenatan.webgpu.WGPURequestDeviceStatus;
import com.github.xpenatan.webgpu.WGPUSupportedFeatures;
import com.github.xpenatan.webgpu.WGPUSurface;
import com.github.xpenatan.webgpu.WGPUUncapturedErrorCallback;
import com.github.xpenatan.webgpu.WGPUVectorFeatureName;

public class WGPUApp {
    public int width;
    public int height;
    public WGPUInstance instance;
    public WGPUAdapter adapter;
    public WGPUDevice device;
    public WGPUSurface surface;
    public WGPUQueue queue;

    private InitState initState = InitState.NOT_INITIALIZED;
    private WGPUBackendType[] backends;
    private int attempt;
    private String failure;
    private final StringBuilder failures = new StringBuilder();
    private WGPURequestAdapterCallback adapterCallback;
    private WGPURequestDeviceCallback deviceCallback;
    private WGPUUncapturedErrorCallback errorCallback;
    private boolean adapterPending;
    private boolean devicePending;
    private boolean disposed;
    private boolean startupComplete;
    private boolean surfaceStartup;

    public void init() {
        init(WGPUBackendType.Undefined);
    }

    /** Each attempt owns an isolated instance. Undefined uses the loader/browser default. */
    public void init(WGPUBackendType... backends) {
        if(instance != null) throw new IllegalStateException("Dispose the previous WGPU session first");
        if(backends == null || backends.length == 0) throw new IllegalArgumentException("No backends supplied");
        this.backends = backends.clone();
        for(WGPUBackendType backend : this.backends) {
            if(backend == null) throw new IllegalArgumentException("Null backend");
        }
        attempt = 0;
        disposed = false;
        startupComplete = false;
        surfaceStartup = false;
        failure = null;
        failures.setLength(0);
        startAttempt();
    }

    public WGPUBackendType backendType() { return backends[attempt]; }

    private void startAttempt() {
        System.out.println("WGPU startup attempt: " + backendType());
        WGPUInstanceDescriptor descriptor = new WGPUInstanceDescriptor();
        try {
            descriptor.setBackendType(backendType());
            instance = WGPU.setupInstance(descriptor);
        } finally {
            descriptor.dispose();
        }
        if(instance.isValid()) {
            initState = InitState.INSTANCE_VALID;
            requestAdapter();
        }
        else {
            initState = InitState.INSTANCE_NOT_VALID;
            failure = "Instance is unavailable";
        }
    }

    private void requestAdapter() {
        WGPURequestAdapterOptions op = WGPURequestAdapterOptions.obtain();
        op.setPowerPreference(WGPUPowerPreference.HighPerformance);
        op.setBackendType(backendType());
        adapterPending = true;
        adapterCallback = new WGPURequestAdapterCallback() {
            @Override
            protected void onCallback(WGPURequestAdapterStatus status, WGPUAdapter adapter, String message) {
                System.out.println("Adapter Status: " + status);
                adapterPending = false;
                if(status == WGPURequestAdapterStatus.Success) {
                    initState = InitState.ADAPTER_VALID;
                    WGPUApp.this.adapter = adapter;
                }
                else {
                    initState = InitState.ADAPTER_NOT_VALID;
                    adapter.dispose(); // The callback owns a wrapper even when its native handle is null.
                    failure = "Adapter " + status + ": " + message;
                }
            }
        };
        instance.requestAdapter(op, WGPUCallbackMode.AllowProcessEvents, adapterCallback);
    }

    private void requestDevice() {
        boolean hasDepthClipControl = adapter.hasFeature(WGPUFeatureName.DepthClipControl);
        WGPUAdapterInfo info = WGPUAdapterInfo.obtain();
        if(adapter.getInfo(info)) {
            WGPUBackendType backendType = info.getBackendType();
            System.out.println("BackendType: " + backendType);
            WGPUAdapterType adapterType = info.getAdapterType();
            System.out.println("AdapterType: " + adapterType);
            String vendor = info.getVendor().c_str();
            System.out.println("Vendor: " + vendor);
            String architecture = info.getArchitecture().c_str();
            System.out.println("Architecture: " + architecture);
            String description = info.getDescription().c_str();
            System.out.println("Description: " + description);
            String device = info.getDevice().c_str();
            System.out.println("Device: " + device);
            System.out.println("Has Feature DepthClipControl: " + hasDepthClipControl);
        }

        WGPUDeviceDescriptor deviceDescriptor = WGPUDeviceDescriptor.obtain();
        WGPULimits limits = WGPULimits.obtain();
        setDefaultLimits(limits);
        deviceDescriptor.setRequiredLimits(limits);
        deviceDescriptor.setLabel("My Device");

        WGPUVectorFeatureName features = WGPUVectorFeatureName.obtain();
        if(hasDepthClipControl) {
            features.push_back(WGPUFeatureName.DepthClipControl);
        }
        deviceDescriptor.setRequiredFeatures(features);

        deviceDescriptor.getDefaultQueue().setLabel("The default queue");
        configureDeviceDescriptor(deviceDescriptor);

        devicePending = true;
        deviceCallback = new WGPURequestDeviceCallback() {
            @Override
            protected void onCallback(WGPURequestDeviceStatus status, WGPUDevice device, String message) {
                System.out.println("Device Status: " + status + " message: " + message);
                devicePending = false;
                if(status == WGPURequestDeviceStatus.Success) {
                    initState = InitState.DEVICE_VALID;
                    WGPUApp.this.device = device;
                    queue = device.getQueue();
                    System.out.println("Platform: " + WGPU.getPlatformType());

                    WGPUSupportedFeatures features = WGPUSupportedFeatures.obtain();
                    device.getFeatures(features);
                    int featureCount = features.getFeatureCount();
                    System.out.println("Total Features: " + featureCount);
                    for(int i = 0; i < featureCount; i++) {
                        WGPUFeatureName featureName = features.getFeatureAt(i);
                        System.out.println("Feature name: " + featureName);
                    }

                    WGPULimits limits = WGPULimits.obtain();
                    device.getLimits(limits);
                    System.out.println("MaxTextureDimension1D: " + limits.getMaxTextureDimension1D());
                    System.out.println("MaxTextureDimension2D: " + limits.getMaxTextureDimension2D());
                    System.out.println("MaxTextureDimension3D: " + limits.getMaxTextureDimension3D());
                    System.out.println("MaxTextureArrayLayers: " + limits.getMaxTextureArrayLayers());
                }
                else {
                    initState = InitState.DEVICE_NOT_VALID;
                    device.dispose();
                    failure = "Device " + status + ": " + message;
                }
            }
        };
        errorCallback = new WGPUUncapturedErrorCallback() {
            @Override
            protected void onCallback(WGPUErrorType errorType, String message) {
                System.err.println("ErrorType: " + errorType);
                System.err.println("Error Message: " + message);
                initState = InitState.ERROR;
                failure = errorType + ": " + message;
            }
        };
        adapter.requestDevice(deviceDescriptor, WGPUCallbackMode.AllowProcessEvents, deviceCallback, errorCallback);
    }

    /** Optional example-specific device requirements, applied to every attempt. */
    protected void configureDeviceDescriptor(WGPUDeviceDescriptor descriptor) { }

    /** Call after the first successfully rendered/presented frame. Later errors are not startup retries. */
    public void startupComplete() { startupComplete = true; }

    public void beginSurfaceStartup() { surfaceStartup = true; }

    public void checkStartupErrors() {
        if(failure != null) throw new IllegalStateException(failure);
    }

    /** Call after disposing any partially created example resources, outside a native callback. */
    public void failStartup(String message) {
        if(startupComplete) throw new IllegalStateException(message);
        failure = message != null ? message : "Startup failed";
        surfaceStartup = false;
        initState = InitState.ERROR;
    }

    public void update() {
        if(backends == null) return;
        if(instance != null && instance.isValid()) {
            instance.processEvents();
        }
        if(adapterCallback != null && !adapterPending) {
            adapterCallback.dispose();
            adapterCallback = null;
        }
        if(deviceCallback != null && !devicePending) {
            deviceCallback.dispose();
            deviceCallback = null;
        }
        if(disposed) {
            if(!adapterPending && !devicePending) releaseAttempt();
            return;
        }
        if(failure != null && !adapterPending && !devicePending) {
            if(surfaceStartup) checkStartupErrors(); // The platform must dispose example resources before retrying.
            if(failures.length() > 0) failures.append("; ");
            failures.append(backendType()).append(": ").append(failure);
            System.err.println("WGPU startup failed: " + backendType() + ": " + failure);
            if(startupComplete) throw new IllegalStateException(failures.toString());
            releaseAttempt();
            failure = null;
            if(attempt + 1 == backends.length) {
                initState = InitState.INSTANCE_NOT_VALID;
                throw new IllegalStateException("All WGPU startup attempts failed: " + failures);
            }
            attempt++;
            startAttempt();
        } else if(initState == InitState.ADAPTER_VALID && !devicePending) {
            requestDevice();
        }
    }

    /** Rendering resources must be disposed first. Pending request callbacks retain this session until update drains them. */
    public void dispose() {
        disposed = true;
        update();
    }

    private void releaseAttempt() {
        if(surface != null) {
            surface.unconfigure();
            surface.release();
            surface.dispose();
            surface = null;
        }
        if(queue != null) { queue.release(); queue = null; } // Wrapper is borrowed from the device.
        if(device != null) {
            device.destroy();
            device.release();
            device.dispose();
            device = null;
        }
        if(errorCallback != null) { errorCallback.dispose(); errorCallback = null; }
        if(adapter != null) { adapter.release(); adapter.dispose(); adapter = null; }
        if(instance != null) {
            if(instance.isValid()) instance.release();
            instance.dispose();
            instance = null;
        }
        initState = InitState.NOT_INITIALIZED;
        System.out.println("WGPU startup attempt released: " + backendType());
    }

    public boolean isReady() {
        return !disposed && initState == InitState.DEVICE_VALID;
    }

    public boolean isNotSupport() {
        return initState.status < 0;
    }

    final static int WGPU_LIMIT_U32_UNDEFINED = -1;
    final static int WGPU_LIMIT_U64_UNDEFINED = -1;

    public void setDefaultLimits (WGPULimits limits) {
        limits.setMaxTextureDimension1D(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxTextureDimension2D(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxTextureDimension3D(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxTextureArrayLayers(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxBindGroups(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxBindGroupsPlusVertexBuffers(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxBindingsPerBindGroup(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxDynamicUniformBuffersPerPipelineLayout(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxDynamicStorageBuffersPerPipelineLayout(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxSampledTexturesPerShaderStage(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxSamplersPerShaderStage(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxStorageBuffersPerShaderStage(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxStorageTexturesPerShaderStage(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxUniformBuffersPerShaderStage(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxUniformBufferBindingSize(WGPU_LIMIT_U64_UNDEFINED);
        limits.setMaxStorageBufferBindingSize(WGPU_LIMIT_U64_UNDEFINED);
        limits.setMinUniformBufferOffsetAlignment(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMinStorageBufferOffsetAlignment(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxVertexBuffers(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxBufferSize(WGPU_LIMIT_U64_UNDEFINED);
        limits.setMaxVertexAttributes(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxVertexBufferArrayStride(WGPU_LIMIT_U32_UNDEFINED);
//        limits.setMaxInterStageShaderComponents(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxInterStageShaderVariables(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxColorAttachments(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxColorAttachmentBytesPerSample(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxComputeWorkgroupStorageSize(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxComputeInvocationsPerWorkgroup(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxComputeWorkgroupSizeX(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxComputeWorkgroupSizeY(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxComputeWorkgroupSizeZ(WGPU_LIMIT_U32_UNDEFINED);
        limits.setMaxComputeWorkgroupsPerDimension(WGPU_LIMIT_U32_UNDEFINED);
    }

    enum InitState {
        NOT_INITIALIZED(0),
        ERROR(1),
        INSTANCE_VALID(2),
        ADAPTER_VALID(3),
        DEVICE_VALID(4),
        INSTANCE_NOT_VALID(-1),
        ADAPTER_NOT_VALID(-2),
        DEVICE_NOT_VALID(-3);

        int status;

        InitState(int status) {
            this.status = status;
        }
    }
}
