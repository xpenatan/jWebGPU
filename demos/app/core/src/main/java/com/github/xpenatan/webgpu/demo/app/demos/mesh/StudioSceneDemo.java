package com.github.xpenatan.webgpu.demo.app.demos.mesh;

import com.github.xpenatan.webgpu.WGPUAddressMode;
import com.github.xpenatan.webgpu.WGPUBlendFactor;
import com.github.xpenatan.webgpu.WGPUBlendOperation;
import com.github.xpenatan.webgpu.WGPUBlendState;
import com.github.xpenatan.webgpu.WGPUBuffer;
import com.github.xpenatan.webgpu.WGPUBufferBindingLayout;
import com.github.xpenatan.webgpu.WGPUBufferBindingType;
import com.github.xpenatan.webgpu.WGPUBufferUsage;
import com.github.xpenatan.webgpu.WGPUBindGroup;
import com.github.xpenatan.webgpu.WGPUBindGroupDescriptor;
import com.github.xpenatan.webgpu.WGPUBindGroupEntry;
import com.github.xpenatan.webgpu.WGPUBindGroupLayout;
import com.github.xpenatan.webgpu.WGPUBindGroupLayoutDescriptor;
import com.github.xpenatan.webgpu.WGPUBindGroupLayoutEntry;
import com.github.xpenatan.webgpu.WGPUChainedStruct;
import com.github.xpenatan.webgpu.WGPUColorTargetState;
import com.github.xpenatan.webgpu.WGPUColorWriteMask;
import com.github.xpenatan.webgpu.WGPUCompareFunction;
import com.github.xpenatan.webgpu.WGPUCullMode;
import com.github.xpenatan.webgpu.WGPUCommandBuffer;
import com.github.xpenatan.webgpu.WGPUCommandBufferDescriptor;
import com.github.xpenatan.webgpu.WGPUCommandEncoder;
import com.github.xpenatan.webgpu.WGPUCommandEncoderDescriptor;
import com.github.xpenatan.webgpu.WGPUDepthStencilState;
import com.github.xpenatan.webgpu.WGPUFilterMode;
import com.github.xpenatan.webgpu.WGPUFragmentState;
import com.github.xpenatan.webgpu.WGPUFrontFace;
import com.github.xpenatan.webgpu.WGPUIndexFormat;
import com.github.xpenatan.webgpu.WGPULoadOp;
import com.github.xpenatan.webgpu.WGPUMipmapFilterMode;
import com.github.xpenatan.webgpu.WGPUOptionalBool;
import com.github.xpenatan.webgpu.WGPUPassTimestampWrites;
import com.github.xpenatan.webgpu.WGPUPipelineLayout;
import com.github.xpenatan.webgpu.WGPUPipelineLayoutDescriptor;
import com.github.xpenatan.webgpu.WGPUPrimitiveTopology;
import com.github.xpenatan.webgpu.WGPURenderPassColorAttachment;
import com.github.xpenatan.webgpu.WGPURenderPassDepthStencilAttachment;
import com.github.xpenatan.webgpu.WGPURenderPassDescriptor;
import com.github.xpenatan.webgpu.WGPURenderPassEncoder;
import com.github.xpenatan.webgpu.WGPURenderPipeline;
import com.github.xpenatan.webgpu.WGPURenderPipelineDescriptor;
import com.github.xpenatan.webgpu.WGPUShaderModule;
import com.github.xpenatan.webgpu.WGPUShaderModuleDescriptor;
import com.github.xpenatan.webgpu.WGPUShaderSourceWGSL;
import com.github.xpenatan.webgpu.WGPUShaderStage;
import com.github.xpenatan.webgpu.WGPUSType;
import com.github.xpenatan.webgpu.WGPUSampler;
import com.github.xpenatan.webgpu.WGPUSamplerBindingLayout;
import com.github.xpenatan.webgpu.WGPUSamplerBindingType;
import com.github.xpenatan.webgpu.WGPUSamplerDescriptor;
import com.github.xpenatan.webgpu.WGPUStoreOp;
import com.github.xpenatan.webgpu.WGPUTexture;
import com.github.xpenatan.webgpu.WGPUTextureAspect;
import com.github.xpenatan.webgpu.WGPUTextureBindingLayout;
import com.github.xpenatan.webgpu.WGPUTextureDescriptor;
import com.github.xpenatan.webgpu.WGPUTextureDimension;
import com.github.xpenatan.webgpu.WGPUTextureFormat;
import com.github.xpenatan.webgpu.WGPUTextureSampleType;
import com.github.xpenatan.webgpu.WGPUTextureUsage;
import com.github.xpenatan.webgpu.WGPUTextureView;
import com.github.xpenatan.webgpu.WGPUTextureViewDescriptor;
import com.github.xpenatan.webgpu.WGPUTextureViewDimension;
import com.github.xpenatan.webgpu.WGPUVectorBindGroupEntry;
import com.github.xpenatan.webgpu.WGPUVectorBindGroupLayout;
import com.github.xpenatan.webgpu.WGPUVectorBindGroupLayoutEntry;
import com.github.xpenatan.webgpu.WGPUVectorColorTargetState;
import com.github.xpenatan.webgpu.WGPUVectorCommandBuffer;
import com.github.xpenatan.webgpu.WGPUVectorConstantEntry;
import com.github.xpenatan.webgpu.WGPUVectorRenderPassColorAttachment;
import com.github.xpenatan.webgpu.WGPUVectorTextureFormat;
import com.github.xpenatan.webgpu.WGPUVectorVertexAttribute;
import com.github.xpenatan.webgpu.WGPUVectorVertexBufferLayout;
import com.github.xpenatan.webgpu.WGPUVertexAttribute;
import com.github.xpenatan.webgpu.WGPUVertexBufferLayout;
import com.github.xpenatan.webgpu.WGPUVertexFormat;
import com.github.xpenatan.webgpu.WGPUVertexStepMode;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;
import com.github.xpenatan.webgpu.backend.core.graphics.DepthTextureTarget;
import com.github.xpenatan.webgpu.backend.core.graphics.GpuBufferUtils;
import com.github.xpenatan.webgpu.backend.core.graphics.LitMeshShaderSource;
import com.github.xpenatan.webgpu.backend.core.graphics.Mat4f;
import com.github.xpenatan.webgpu.backend.core.graphics.MeshData;
import com.github.xpenatan.webgpu.backend.core.graphics.ParametricMeshGenerator;
import com.github.xpenatan.webgpu.backend.core.graphics.StudioShaderOptions;
import com.github.xpenatan.webgpu.backend.core.graphics.SurfaceFrameContext;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.List;

public class StudioSceneDemo implements ApplicationListener {

    private static final int SHADOW_MAP_SIZE = 2048;
    private static final int UNIFORM_SIZE_BYTES = 56 * Float.BYTES;

    // Studio look controls are centralized here for easier tweaking.
    private final StudioShaderOptions shaderOptions = new StudioShaderOptions();

    private static class RenderItem {
        WGPUBuffer vertexBuffer;
        WGPUBuffer indexBuffer;
        int vertexBufferSize;
        int indexBufferSize;
        WGPUBuffer uniformBuffer;
        WGPUBindGroup bindGroup;
        WGPUBindGroup shadowBindGroup;
        WGPURenderPipeline pipeline;
        ByteBuffer uniformData;
        int indexCount;
        float posX;
        float posY;
        float posZ;
        float rotX;
        float rotY;
        float rotZ;
        boolean isGround;
    }

    private final SurfaceFrameContext surfaceFrameContext = new SurfaceFrameContext();
    private final DepthTextureTarget depthTextureTarget = new DepthTextureTarget();
    private final StudioFrameCapture frameCaptureColor = new StudioFrameCapture(false);
    private final StudioFrameCapture frameCaptureShadow = new StudioFrameCapture(true);
    private final List<RenderItem> items = new ArrayList<>();

    private WGPUBindGroupLayout bindGroupLayout;
    private WGPUBindGroupLayout shadowBindGroupLayout;
    private WGPUPipelineLayout pipelineLayout;
    private WGPUPipelineLayout shadowPipelineLayout;
    private WGPURenderPipeline shadowPipeline;
    private WGPUTexture shadowTexture;
    private WGPUTextureView shadowTextureView;
    private WGPUSampler shadowSampler;

    private final float[] view = new float[16];
    private final float[] projection = new float[16];
    private final float[] lightView = new float[16];
    private final float[] lightProjection = new float[16];
    private final float[] lightViewProjection = new float[16];

    private final float[] cameraPosition = new float[3];
    private final float[] lightDirection = new float[3];
    private float cameraAngle = 0;

    @Override
    public void create(WGPUApp wgpu) {
        if(wgpu.surface == null) {
            System.out.println("Surface not created");
            return;
        }

        WGPUTextureFormat surfaceFormat = surfaceFrameContext.configure(wgpu, true);
        depthTextureTarget.create(wgpu.device, wgpu.width, wgpu.height, WGPUTextureFormat.Depth24Plus);
        createShadowResources(wgpu);

        shadowBindGroupLayout = createShadowBindGroupLayout(wgpu);
        shadowPipelineLayout = createPipelineLayout(wgpu, shadowBindGroupLayout);
        bindGroupLayout = createBindGroupLayout(wgpu);
        pipelineLayout = createPipelineLayout(wgpu, bindGroupLayout);
        shadowPipeline = createShadowPipeline(wgpu);

        addSceneItems(wgpu, surfaceFormat);

        wgpu.adapter.release();
        wgpu.adapter.dispose();
        wgpu.adapter = null;
    }

    @Override
    public void render(WGPUApp wgpu) {
        if(items.isEmpty()) {
            return;
        }

        cameraAngle += 0.0001f;

        updateFrameState(wgpu);

        // Update all per-object uniforms before command encoding.
        for(RenderItem item : items) {
            updateUniforms(item, wgpu);
        }

        WGPUTextureView targetView = surfaceFrameContext.acquireCurrentView(wgpu);

        WGPUCommandEncoder encoder = WGPUCommandEncoder.obtain();
        WGPUCommandEncoderDescriptor encoderDesc = WGPUCommandEncoderDescriptor.obtain();
        encoderDesc.setLabel("studio command encoder");
        wgpu.device.createCommandEncoder(encoderDesc, encoder);

        WGPURenderPassDepthStencilAttachment shadowDepthAttachment = createShadowAttachment();

        WGPURenderPassDescriptor shadowPassDesc = WGPURenderPassDescriptor.obtain();
        shadowPassDesc.setColorAttachments(WGPUVectorRenderPassColorAttachment.NULL);
        shadowPassDesc.setDepthStencilAttachment(shadowDepthAttachment);
        shadowPassDesc.setTimestampWrites(WGPUPassTimestampWrites.NULL);

        WGPURenderPassEncoder shadowPass = WGPURenderPassEncoder.obtain();
        encoder.beginRenderPass(shadowPassDesc, shadowPass);

        for(RenderItem item : items) {
            shadowPass.setPipeline(shadowPipeline);
            shadowPass.setBindGroup(0, item.shadowBindGroup);
            shadowPass.setVertexBuffer(0, item.vertexBuffer, 0, item.vertexBufferSize);
            shadowPass.setIndexBuffer(item.indexBuffer, WGPUIndexFormat.Uint16, 0, item.indexBufferSize);
            shadowPass.drawIndexed(item.indexCount, 1, 0, 0, 0);
        }

        shadowPass.end();
        shadowPass.release();

        WGPURenderPassColorAttachment colorAttachment = WGPURenderPassColorAttachment.obtain();
        colorAttachment.setView(targetView);
        colorAttachment.setResolveTarget(WGPUTextureView.NULL);
        colorAttachment.setLoadOp(WGPULoadOp.Clear);
        colorAttachment.setStoreOp(WGPUStoreOp.Store);
        colorAttachment.getClearValue().setColor(0.045f, 0.065f, 0.10f, 1.0f);

        WGPUVectorRenderPassColorAttachment colorAttachments = WGPUVectorRenderPassColorAttachment.obtain();
        colorAttachments.clear();
        colorAttachments.push_back(colorAttachment);

        WGPURenderPassDepthStencilAttachment depthAttachment = depthTextureTarget.createAttachment();

        WGPURenderPassDescriptor renderPassDesc = WGPURenderPassDescriptor.obtain();
        renderPassDesc.setColorAttachments(colorAttachments);
        renderPassDesc.setDepthStencilAttachment(depthAttachment);
        renderPassDesc.setTimestampWrites(WGPUPassTimestampWrites.NULL);

        WGPURenderPassEncoder renderPass = WGPURenderPassEncoder.obtain();
        encoder.beginRenderPass(renderPassDesc, renderPass);

        for(RenderItem item : items) {
            renderPass.setPipeline(item.pipeline);
            renderPass.setBindGroup(0, item.bindGroup);
            renderPass.setVertexBuffer(0, item.vertexBuffer, 0, item.vertexBufferSize);
            renderPass.setIndexBuffer(item.indexBuffer, WGPUIndexFormat.Uint16, 0, item.indexBufferSize);
            renderPass.drawIndexed(item.indexCount, 1, 0, 0, 0);
        }

        renderPass.end();
        renderPass.release();

        frameCaptureColor.encodeCopyIfNeeded(wgpu, encoder, surfaceFrameContext.getCurrentTexture(), wgpu.width, wgpu.height);
        frameCaptureShadow.encodeCopyIfNeeded(wgpu, encoder, shadowTexture, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);

        WGPUCommandBuffer command = WGPUCommandBuffer.obtain();
        WGPUCommandBufferDescriptor commandDesc = WGPUCommandBufferDescriptor.obtain();
        commandDesc.setNextInChain(WGPUChainedStruct.NULL);
        commandDesc.setLabel("studio command");
        encoder.finish(commandDesc, command);
        encoder.release();

        WGPUVectorCommandBuffer commands = WGPUVectorCommandBuffer.obtain();
        commands.clear();
        commands.push_back(command);
        wgpu.queue.submit(commands);
        frameCaptureColor.mapAfterSubmit();
        frameCaptureShadow.mapAfterSubmit();
        command.release();

        targetView.release();
        surfaceFrameContext.present(wgpu);
    }

    @Override
    public void dispose() {
        for(RenderItem item : items) {
            if(item.pipeline != null) item.pipeline.release();
            if(item.bindGroup != null) item.bindGroup.release();
            if(item.shadowBindGroup != null) item.shadowBindGroup.release();
            if(item.uniformBuffer != null) item.uniformBuffer.release();
            if(item.indexBuffer != null) item.indexBuffer.release();
            if(item.vertexBuffer != null) item.vertexBuffer.release();
        }
        items.clear();

        if(pipelineLayout != null) {
            pipelineLayout.release();
            pipelineLayout = null;
        }
        if(shadowPipelineLayout != null) {
            shadowPipelineLayout.release();
            shadowPipelineLayout = null;
        }
        if(shadowPipeline != null) {
            shadowPipeline.release();
            shadowPipeline = null;
        }
        if(bindGroupLayout != null) {
            bindGroupLayout.release();
            bindGroupLayout = null;
        }
        if(shadowBindGroupLayout != null) {
            shadowBindGroupLayout.release();
            shadowBindGroupLayout = null;
        }
        if(shadowSampler != null) {
            shadowSampler.release();
            shadowSampler = null;
        }
        if(shadowTextureView != null) {
            shadowTextureView.release();
            shadowTextureView = null;
        }
        if(shadowTexture != null) {
            shadowTexture.destroy();
            shadowTexture.release();
            shadowTexture = null;
        }

        frameCaptureColor.dispose();
        frameCaptureShadow.dispose();

        depthTextureTarget.dispose();
    }

    private void addSceneItems(WGPUApp wgpu, WGPUTextureFormat surfaceFormat) {
        MeshData cone = ParametricMeshGenerator.cone(0.60f, 1.70f, 72, 1);
        MeshData cube = ParametricMeshGenerator.cube(0.90f);
        MeshData sphere = ParametricMeshGenerator.sphere(0.45f, 24, 32);
        MeshData torus = ParametricMeshGenerator.torus(0.40f, 0.18f, 48, 24);
        MeshData ground = ParametricMeshGenerator.plane(6.0f, 6.0f);

        items.add(createItem(wgpu, surfaceFormat, cone, 1.00f, 0.27f, 0.14f, 0.30f, 0.12f, -1.00f, -0.20f, 0.70f, 0.00f, 0.08f, 0.00f, false));
        items.add(createItem(wgpu, surfaceFormat, cube, 0.42f, 0.74f, 0.62f, 0.34f, 0.04f, -1.30f, -0.40f, -0.46f, 0.00f, -0.85f, 0.00f, false));
        items.add(createItem(wgpu, surfaceFormat, sphere, 0.58f, 0.32f, 0.80f, 0.12f, 0.40f, 0.60f, -0.50f, -0.40f, 0.00f, 0.00f, 0.00f, false));
        items.add(createItem(wgpu, surfaceFormat, torus, 0.10f, 0.42f, 0.92f, 0.28f, 0.30f, 0.20f, -0.70f, 0.90f, 0.00f, 0.00f, 0.00f, false));
        items.add(createItem(wgpu, surfaceFormat, ground, 0.26f, 0.24f, 0.28f, 0.98f, 0.00f, 0.0f, -0.85f, 0.0f, 0.0f, 0.0f, 0.0f, true));
    }

    private RenderItem createItem(
            WGPUApp wgpu,
            WGPUTextureFormat surfaceFormat,
            MeshData meshData,
            float r,
            float g,
            float b,
            float roughness,
            float metallic,
            float posX,
            float posY,
            float posZ,
            float rotX,
            float rotY,
            float rotZ,
            boolean isGround
    ) {
        RenderItem item = new RenderItem();
        item.posX = posX;
        item.posY = posY;
        item.posZ = posZ;
        item.rotX = rotX;
        item.rotY = rotY;
        item.rotZ = rotZ;
        item.isGround = isGround;
        item.indexCount = meshData.indices.length;

        ByteBuffer vertexData = GpuBufferUtils.createFloatBuffer(meshData.vertices);
        ByteBuffer indexData = GpuBufferUtils.createShortBuffer(meshData.indices);

        item.vertexBuffer = GpuBufferUtils.createBuffer(
                wgpu,
                vertexData.limit(),
                WGPUBufferUsage.CopyDst.or(WGPUBufferUsage.Vertex)
        );
        item.vertexBufferSize = vertexData.limit();
        GpuBufferUtils.upload(wgpu, item.vertexBuffer, vertexData);

        item.indexBuffer = GpuBufferUtils.createBuffer(
                wgpu,
                indexData.limit(),
                WGPUBufferUsage.CopyDst.or(WGPUBufferUsage.Index)
        );
        item.indexBufferSize = indexData.limit();
        GpuBufferUtils.upload(wgpu, item.indexBuffer, indexData);

        item.uniformBuffer = GpuBufferUtils.createBuffer(
                wgpu,
                UNIFORM_SIZE_BYTES,
                WGPUBufferUsage.CopyDst.or(WGPUBufferUsage.Uniform)
        );

        item.uniformData = ByteBuffer.allocateDirect(UNIFORM_SIZE_BYTES);
        item.uniformData.order(ByteOrder.LITTLE_ENDIAN);

        item.shadowBindGroup = createShadowBindGroup(wgpu, item.uniformBuffer);
        item.bindGroup = createBindGroup(wgpu, item.uniformBuffer, shadowSampler, shadowTextureView);
        item.pipeline = createRenderPipeline(
                wgpu,
                surfaceFormat,
                r,
                g,
                b,
                roughness,
                metallic,
                isGround
        );
        if(item.shadowBindGroup == null || !item.shadowBindGroup.isValid()) {
            throw new RuntimeException("StudioSceneDemo: invalid shadow bind group");
        }
        if(item.bindGroup == null || !item.bindGroup.isValid()) {
            throw new RuntimeException("StudioSceneDemo: invalid bind group");
        }
        if(item.pipeline == null || !item.pipeline.isValid()) {
            throw new RuntimeException("StudioSceneDemo: invalid render pipeline");
        }
        return item;
    }

    private WGPUBindGroupLayout createBindGroupLayout(WGPUApp wgpu) {
        WGPUBindGroupLayoutEntry uniformEntry = new WGPUBindGroupLayoutEntry();
        uniformEntry.setBinding(0);
        uniformEntry.setVisibility(vertexAndFragmentStage());

        WGPUBufferBindingLayout bindingLayout = new WGPUBufferBindingLayout();
        bindingLayout.setType(WGPUBufferBindingType.Uniform);
        bindingLayout.setHasDynamicOffset(0);
        bindingLayout.setMinBindingSize(UNIFORM_SIZE_BYTES);
        uniformEntry.setBuffer(bindingLayout);

        WGPUBindGroupLayoutEntry samplerEntry = new WGPUBindGroupLayoutEntry();
        samplerEntry.setBinding(1);
        samplerEntry.setVisibility(WGPUShaderStage.Fragment);

        WGPUSamplerBindingLayout samplerBindingLayout = new WGPUSamplerBindingLayout();
        samplerBindingLayout.setType(WGPUSamplerBindingType.Comparison);
        samplerEntry.setSampler(samplerBindingLayout);

        WGPUBindGroupLayoutEntry shadowTextureEntry = new WGPUBindGroupLayoutEntry();
        shadowTextureEntry.setBinding(2);
        shadowTextureEntry.setVisibility(WGPUShaderStage.Fragment);

        WGPUTextureBindingLayout textureBindingLayout = new WGPUTextureBindingLayout();
        textureBindingLayout.setSampleType(WGPUTextureSampleType.Depth);
        textureBindingLayout.setViewDimension(WGPUTextureViewDimension._2D);
        textureBindingLayout.setMultisampled(0);
        shadowTextureEntry.setTexture(textureBindingLayout);

        WGPUVectorBindGroupLayoutEntry entries = WGPUVectorBindGroupLayoutEntry.obtain();
        entries.clear();
        entries.push_back(uniformEntry);
        entries.push_back(samplerEntry);
        entries.push_back(shadowTextureEntry);

        WGPUBindGroupLayoutDescriptor descriptor = WGPUBindGroupLayoutDescriptor.obtain();
        descriptor.setEntries(entries);

        WGPUBindGroupLayout layout = new WGPUBindGroupLayout();
        wgpu.device.createBindGroupLayout(descriptor, layout);
        return layout;
    }

    private WGPUBindGroupLayout createShadowBindGroupLayout(WGPUApp wgpu) {
        WGPUBindGroupLayoutEntry uniformEntry = new WGPUBindGroupLayoutEntry();
        uniformEntry.setBinding(0);
        uniformEntry.setVisibility(WGPUShaderStage.Vertex);

        WGPUBufferBindingLayout bindingLayout = new WGPUBufferBindingLayout();
        bindingLayout.setType(WGPUBufferBindingType.Uniform);
        bindingLayout.setHasDynamicOffset(0);
        bindingLayout.setMinBindingSize(UNIFORM_SIZE_BYTES);
        uniformEntry.setBuffer(bindingLayout);

        WGPUVectorBindGroupLayoutEntry entries = WGPUVectorBindGroupLayoutEntry.obtain();
        entries.clear();
        entries.push_back(uniformEntry);

        WGPUBindGroupLayoutDescriptor descriptor = WGPUBindGroupLayoutDescriptor.obtain();
        descriptor.setEntries(entries);

        WGPUBindGroupLayout layout = new WGPUBindGroupLayout();
        wgpu.device.createBindGroupLayout(descriptor, layout);
        return layout;
    }

    private WGPUPipelineLayout createPipelineLayout(WGPUApp wgpu, WGPUBindGroupLayout groupLayout) {
        WGPUVectorBindGroupLayout layouts = WGPUVectorBindGroupLayout.obtain();
        layouts.push_back(groupLayout);

        WGPUPipelineLayoutDescriptor descriptor = WGPUPipelineLayoutDescriptor.obtain();
        descriptor.setBindGroupLayouts(layouts);

        WGPUPipelineLayout result = new WGPUPipelineLayout();
        wgpu.device.createPipelineLayout(descriptor, result);
        return result;
    }

    private WGPUBindGroup createBindGroup(WGPUApp wgpu, WGPUBuffer uniformBuffer, WGPUSampler sampler, WGPUTextureView textureView) {
        WGPUBindGroupEntry uniformEntry = new WGPUBindGroupEntry();
        uniformEntry.setBinding(0);
        uniformEntry.setBuffer(uniformBuffer);
        uniformEntry.setOffset(0);
        uniformEntry.setSize(UNIFORM_SIZE_BYTES);

        WGPUBindGroupEntry samplerEntry = new WGPUBindGroupEntry();
        samplerEntry.setBinding(1);
        samplerEntry.setSampler(sampler);

        WGPUBindGroupEntry textureEntry = new WGPUBindGroupEntry();
        textureEntry.setBinding(2);
        textureEntry.setTextureView(textureView);

        WGPUVectorBindGroupEntry entries = WGPUVectorBindGroupEntry.obtain();
        entries.clear();
        entries.push_back(uniformEntry);
        entries.push_back(samplerEntry);
        entries.push_back(textureEntry);

        WGPUBindGroupDescriptor descriptor = WGPUBindGroupDescriptor.obtain();
        descriptor.setLayout(bindGroupLayout);
        descriptor.setEntries(entries);

        WGPUBindGroup bindGroup = new WGPUBindGroup();
        wgpu.device.createBindGroup(descriptor, bindGroup);
        return bindGroup;
    }

    private WGPUBindGroup createShadowBindGroup(WGPUApp wgpu, WGPUBuffer uniformBuffer) {
        WGPUBindGroupEntry uniformEntry = new WGPUBindGroupEntry();
        uniformEntry.setBinding(0);
        uniformEntry.setBuffer(uniformBuffer);
        uniformEntry.setOffset(0);
        uniformEntry.setSize(UNIFORM_SIZE_BYTES);

        WGPUVectorBindGroupEntry entries = WGPUVectorBindGroupEntry.obtain();
        entries.clear();
        entries.push_back(uniformEntry);

        WGPUBindGroupDescriptor descriptor = WGPUBindGroupDescriptor.obtain();
        descriptor.setLayout(shadowBindGroupLayout);
        descriptor.setEntries(entries);

        WGPUBindGroup bindGroup = new WGPUBindGroup();
        wgpu.device.createBindGroup(descriptor, bindGroup);
        return bindGroup;
    }

    private WGPURenderPipeline createRenderPipeline(
            WGPUApp wgpu,
            WGPUTextureFormat surfaceFormat,
            float r,
            float g,
            float b,
            float roughness,
            float metallic,
            boolean isGround
    ) {
        String shaderCode = isGround
                ? LitMeshShaderSource.studioGroundShader(r, g, b, shaderOptions)
                : LitMeshShaderSource.studioPbrShader(r, g, b, roughness, metallic, shaderOptions);
        WGPUShaderModule shaderModule = createShaderModule(wgpu, shaderCode);

        WGPURenderPipelineDescriptor pipelineDesc = WGPURenderPipelineDescriptor.obtain();
        pipelineDesc.setLayout(pipelineLayout);
        pipelineDesc.setLabel("studio pipeline");

        WGPUVertexAttribute positionAttribute = new WGPUVertexAttribute();
        positionAttribute.setShaderLocation(0);
        positionAttribute.setFormat(WGPUVertexFormat.Float32x3);
        positionAttribute.setOffset(0);

        WGPUVertexAttribute normalAttribute = new WGPUVertexAttribute();
        normalAttribute.setShaderLocation(1);
        normalAttribute.setFormat(WGPUVertexFormat.Float32x3);
        normalAttribute.setOffset(3 * Float.BYTES);

        WGPUVectorVertexAttribute vertexAttributes = WGPUVectorVertexAttribute.obtain();
        vertexAttributes.clear();
        vertexAttributes.push_back(positionAttribute);
        vertexAttributes.push_back(normalAttribute);

        WGPUVertexBufferLayout vertexBufferLayout = WGPUVertexBufferLayout.obtain();
        vertexBufferLayout.setArrayStride(MeshData.VERTEX_STRIDE_BYTES);
        vertexBufferLayout.setStepMode(WGPUVertexStepMode.Vertex);
        vertexBufferLayout.setAttributes(vertexAttributes);

        WGPUVectorVertexBufferLayout vertexBuffers = WGPUVectorVertexBufferLayout.obtain();
        vertexBuffers.clear();
        vertexBuffers.push_back(vertexBufferLayout);

        pipelineDesc.getVertex().setBuffers(vertexBuffers);
        pipelineDesc.getVertex().setModule(shaderModule);
        pipelineDesc.getVertex().setEntryPoint("vs_main");
        pipelineDesc.getVertex().setConstants(WGPUVectorConstantEntry.NULL);

        pipelineDesc.getPrimitive().setTopology(WGPUPrimitiveTopology.TriangleList);
        pipelineDesc.getPrimitive().setFrontFace(WGPUFrontFace.CCW);
        pipelineDesc.getPrimitive().setCullMode(WGPUCullMode.None);

        WGPUFragmentState fragmentState = WGPUFragmentState.obtain();
        fragmentState.setModule(shaderModule);
        fragmentState.setEntryPoint("fs_main");
        fragmentState.setConstants(WGPUVectorConstantEntry.NULL);

        WGPUBlendState blendState = WGPUBlendState.obtain();
        blendState.getColor().setSrcFactor(WGPUBlendFactor.One);
        blendState.getColor().setDstFactor(WGPUBlendFactor.Zero);
        blendState.getColor().setOperation(WGPUBlendOperation.Add);
        blendState.getAlpha().setSrcFactor(WGPUBlendFactor.One);
        blendState.getAlpha().setDstFactor(WGPUBlendFactor.Zero);
        blendState.getAlpha().setOperation(WGPUBlendOperation.Add);

        WGPUColorTargetState colorTarget = WGPUColorTargetState.obtain();
        colorTarget.setFormat(surfaceFormat);
        colorTarget.setBlend(blendState);
        colorTarget.setWriteMask(WGPUColorWriteMask.All);

        WGPUVectorColorTargetState colorTargets = WGPUVectorColorTargetState.obtain();
        colorTargets.clear();
        colorTargets.push_back(colorTarget);
        fragmentState.setTargets(colorTargets);
        pipelineDesc.setFragment(fragmentState);

        WGPUDepthStencilState depthStencilState = depthTextureTarget.createDepthStencilState();
        pipelineDesc.setDepthStencil(depthStencilState);
        pipelineDesc.getMultisample().setCount(1);
        pipelineDesc.getMultisample().setMask(-1);
        pipelineDesc.getMultisample().setAlphaToCoverageEnabled(false);

        WGPURenderPipeline pipeline = new WGPURenderPipeline();
        wgpu.device.createRenderPipeline(pipelineDesc, pipeline);

        shaderModule.release();
        return pipeline;
    }

    private WGPURenderPipeline createShadowPipeline(WGPUApp wgpu) {
        WGPUShaderModule shaderModule = createShaderModule(wgpu, LitMeshShaderSource.studioShadowDepthShader());

        WGPURenderPipelineDescriptor pipelineDesc = WGPURenderPipelineDescriptor.obtain();
        pipelineDesc.setLayout(shadowPipelineLayout);
        pipelineDesc.setLabel("studio shadow pipeline");

        WGPUVertexAttribute positionAttribute = new WGPUVertexAttribute();
        positionAttribute.setShaderLocation(0);
        positionAttribute.setFormat(WGPUVertexFormat.Float32x3);
        positionAttribute.setOffset(0);

        WGPUVertexAttribute normalAttribute = new WGPUVertexAttribute();
        normalAttribute.setShaderLocation(1);
        normalAttribute.setFormat(WGPUVertexFormat.Float32x3);
        normalAttribute.setOffset(3 * Float.BYTES);

        WGPUVectorVertexAttribute vertexAttributes = WGPUVectorVertexAttribute.obtain();
        vertexAttributes.clear();
        vertexAttributes.push_back(positionAttribute);
        vertexAttributes.push_back(normalAttribute);

        WGPUVertexBufferLayout vertexBufferLayout = WGPUVertexBufferLayout.obtain();
        vertexBufferLayout.setArrayStride(MeshData.VERTEX_STRIDE_BYTES);
        vertexBufferLayout.setStepMode(WGPUVertexStepMode.Vertex);
        vertexBufferLayout.setAttributes(vertexAttributes);

        WGPUVectorVertexBufferLayout vertexBuffers = WGPUVectorVertexBufferLayout.obtain();
        vertexBuffers.clear();
        vertexBuffers.push_back(vertexBufferLayout);

        pipelineDesc.getVertex().setBuffers(vertexBuffers);
        pipelineDesc.getVertex().setModule(shaderModule);
        pipelineDesc.getVertex().setEntryPoint("vs_main");
        pipelineDesc.getVertex().setConstants(WGPUVectorConstantEntry.NULL);

        pipelineDesc.getPrimitive().setTopology(WGPUPrimitiveTopology.TriangleList);
        pipelineDesc.getPrimitive().setFrontFace(WGPUFrontFace.CCW);
        pipelineDesc.getPrimitive().setCullMode(WGPUCullMode.None);

        WGPUDepthStencilState depthStencilState = WGPUDepthStencilState.obtain();
        depthStencilState.setFormat(WGPUTextureFormat.Depth32Float);
        depthStencilState.setDepthWriteEnabled(WGPUOptionalBool.True);
        depthStencilState.setDepthCompare(WGPUCompareFunction.Less);
        depthStencilState.setDepthBias(1);
        depthStencilState.setDepthBiasClamp(0.0f);
        depthStencilState.setDepthBiasSlopeScale(0.8f);
        depthStencilState.setStencilReadMask(0xFFFFFFFF);
        depthStencilState.setStencilWriteMask(0xFFFFFFFF);
        pipelineDesc.setDepthStencil(depthStencilState);

        pipelineDesc.getMultisample().setCount(1);
        pipelineDesc.getMultisample().setMask(-1);
        pipelineDesc.getMultisample().setAlphaToCoverageEnabled(false);

        WGPURenderPipeline pipeline = new WGPURenderPipeline();
        wgpu.device.createRenderPipeline(pipelineDesc, pipeline);

        shaderModule.release();
        return pipeline;
    }

    private void createShadowResources(WGPUApp wgpu) {
        WGPUTextureDescriptor shadowTextureDesc = WGPUTextureDescriptor.obtain();
        shadowTextureDesc.setLabel("studio shadow map");
        shadowTextureDesc.setFormat(WGPUTextureFormat.Depth32Float);
        shadowTextureDesc.setUsage(WGPUTextureUsage.RenderAttachment.or(WGPUTextureUsage.TextureBinding).or(WGPUTextureUsage.CopySrc));
        shadowTextureDesc.setDimension(WGPUTextureDimension._2D);
        shadowTextureDesc.setMipLevelCount(1);
        shadowTextureDesc.setSampleCount(1);
        shadowTextureDesc.setViewFormats(WGPUVectorTextureFormat.NULL);

        shadowTextureDesc.getSize().setWidth(SHADOW_MAP_SIZE);
        shadowTextureDesc.getSize().setHeight(SHADOW_MAP_SIZE);
        shadowTextureDesc.getSize().setDepthOrArrayLayers(1);

        shadowTexture = new WGPUTexture();
        wgpu.device.createTexture(shadowTextureDesc, shadowTexture);

        WGPUTextureViewDescriptor shadowViewDesc = WGPUTextureViewDescriptor.obtain();
        shadowViewDesc.setLabel("studio shadow map view");
        shadowViewDesc.setFormat(WGPUTextureFormat.Depth32Float);
        shadowViewDesc.setDimension(WGPUTextureViewDimension._2D);
        shadowViewDesc.setBaseMipLevel(0);
        shadowViewDesc.setMipLevelCount(1);
        shadowViewDesc.setBaseArrayLayer(0);
        shadowViewDesc.setArrayLayerCount(1);
        shadowViewDesc.setAspect(WGPUTextureAspect.DepthOnly);

        shadowTextureView = new WGPUTextureView();
        shadowTexture.createView(shadowViewDesc, shadowTextureView);

        WGPUSamplerDescriptor samplerDesc = WGPUSamplerDescriptor.obtain();
        samplerDesc.setAddressModeU(WGPUAddressMode.ClampToEdge);
        samplerDesc.setAddressModeV(WGPUAddressMode.ClampToEdge);
        samplerDesc.setAddressModeW(WGPUAddressMode.ClampToEdge);
        samplerDesc.setMagFilter(WGPUFilterMode.Linear);
        samplerDesc.setMinFilter(WGPUFilterMode.Linear);
        samplerDesc.setMipmapFilter(WGPUMipmapFilterMode.Nearest);
        samplerDesc.setCompare(WGPUCompareFunction.LessEqual);
        samplerDesc.setLodMinClamp(0.0f);
        samplerDesc.setLodMaxClamp(1.0f);
        samplerDesc.setMaxAnisotropy(1);

        shadowSampler = new WGPUSampler();
        wgpu.device.createSampler(samplerDesc, shadowSampler);
    }

    private WGPURenderPassDepthStencilAttachment createShadowAttachment() {
        WGPURenderPassDepthStencilAttachment attachment = WGPURenderPassDepthStencilAttachment.obtain();
        attachment.setView(shadowTextureView);
        attachment.setDepthLoadOp(WGPULoadOp.Clear);
        attachment.setDepthStoreOp(WGPUStoreOp.Store);
        attachment.setDepthClearValue(1.0f);
        attachment.setDepthReadOnly(false);
        attachment.setStencilLoadOp(WGPULoadOp.Undefined);
        attachment.setStencilStoreOp(WGPUStoreOp.Undefined);
        attachment.setStencilClearValue(0);
        attachment.setStencilReadOnly(true);
        return attachment;
    }

    private WGPUShaderModule createShaderModule(WGPUApp wgpu, String shaderCode) {
        WGPUShaderSourceWGSL shaderSource = WGPUShaderSourceWGSL.obtain();
        shaderSource.getChain().setNext(WGPUChainedStruct.NULL);
        shaderSource.getChain().setSType(WGPUSType.ShaderSourceWGSL);
        shaderSource.setCode(shaderCode);

        WGPUShaderModuleDescriptor shaderDesc = WGPUShaderModuleDescriptor.obtain();
        shaderDesc.setNextInChain(shaderSource.getChain());

        WGPUShaderModule shaderModule = WGPUShaderModule.obtain();
        wgpu.device.createShaderModule(shaderDesc, shaderModule);
        return shaderModule;
    }

    private void updateFrameState(WGPUApp wgpu) {
        float eyeX = (float)Math.cos(cameraAngle) * 3.9f;
        float eyeZ = (float)Math.sin(cameraAngle) * 3.9f;
        cameraPosition[0] = eyeX;
        cameraPosition[1] = 1.58f;
        cameraPosition[2] = eyeZ;

        Mat4f.lookAt(view, eyeX, cameraPosition[1], eyeZ, 0.0f, -0.18f, 0.1f, 0.0f, 1.0f, 0.0f);
        Mat4f.perspective(projection, (float)Math.toRadians(42.0), (float)wgpu.width / (float)wgpu.height, 0.1f, 25.0f);

        float centerX = 0.0f;
        float centerY = -0.30f;
        float centerZ = 0.10f;

        float lightDirX = -0.10f;
        float lightDirY = 0.58f;
        float lightDirZ = -0.40f;
        float lightDirLen = (float)Math.sqrt(lightDirX * lightDirX + lightDirY * lightDirY + lightDirZ * lightDirZ);
        lightDirX /= lightDirLen;
        lightDirY /= lightDirLen;
        lightDirZ /= lightDirLen;

        float lightDistance = 10.0f;
        float lightPosX = centerX + lightDirX * lightDistance;
        float lightPosY = centerY + lightDirY * lightDistance;
        float lightPosZ = centerZ + lightDirZ * lightDistance;

        Mat4f.lookAt(lightView, lightPosX, lightPosY, lightPosZ, centerX, centerY, centerZ, 0.0f, 1.0f, 0.0f);
        Mat4f.orthographic(lightProjection, -8.0f, 8.0f, -8.0f, 8.0f, 0.5f, 30.0f);
        Mat4f.multiply(lightViewProjection, lightProjection, lightView);

        lightDirection[0] = lightDirX;
        lightDirection[1] = lightDirY;
        lightDirection[2] = lightDirZ;
    }

    private void updateUniforms(RenderItem item, WGPUApp wgpu) {
        float[] translation = new float[16];
        float[] rotationX = new float[16];
        float[] rotationY = new float[16];
        float[] rotationZ = new float[16];
        float[] rotationTmp = new float[16];
        float[] rotation = new float[16];
        float[] model = new float[16];
        float[] lightMvp = new float[16];
        float[] temp = new float[16];
        float[] mvp = new float[16];

        Mat4f.identity(translation);
        translation[12] = item.posX;
        translation[13] = item.posY;
        translation[14] = item.posZ;

        Mat4f.rotateX(rotationX, item.rotX);
        Mat4f.rotateY(rotationY, item.rotY);
        Mat4f.rotateZ(rotationZ, item.rotZ);
        Mat4f.multiply(rotationTmp, rotationY, rotationX);
        Mat4f.multiply(rotation, rotationZ, rotationTmp);
        Mat4f.multiply(model, translation, rotation);

        Mat4f.multiply(temp, view, model);
        Mat4f.multiply(mvp, projection, temp);
        Mat4f.multiply(lightMvp, lightViewProjection, model);

        FloatBuffer floatBuffer = item.uniformData.asFloatBuffer();
        floatBuffer.position(0);
        floatBuffer.put(mvp);
        floatBuffer.put(model);
        floatBuffer.put(lightMvp);
        floatBuffer.put(cameraPosition[0]);
        floatBuffer.put(cameraPosition[1]);
        floatBuffer.put(cameraPosition[2]);
        floatBuffer.put(0.0f);
        floatBuffer.put(lightDirection[0]);
        floatBuffer.put(lightDirection[1]);
        floatBuffer.put(lightDirection[2]);
        floatBuffer.put(0.0f);

        wgpu.queue.writeBuffer(item.uniformBuffer, 0, item.uniformData, UNIFORM_SIZE_BYTES);
    }

    private WGPUShaderStage vertexAndFragmentStage() {
        return WGPUShaderStage.CUSTOM.setValue(WGPUShaderStage.Vertex.getValue() | WGPUShaderStage.Fragment.getValue());
    }
}

