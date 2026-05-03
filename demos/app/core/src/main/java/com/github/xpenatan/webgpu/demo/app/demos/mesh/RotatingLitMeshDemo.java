package com.github.xpenatan.webgpu.demo.app.demos.mesh;

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
import com.github.xpenatan.webgpu.WGPUCommandBuffer;
import com.github.xpenatan.webgpu.WGPUCommandBufferDescriptor;
import com.github.xpenatan.webgpu.WGPUCommandEncoder;
import com.github.xpenatan.webgpu.WGPUCommandEncoderDescriptor;
import com.github.xpenatan.webgpu.WGPUDepthStencilState;
import com.github.xpenatan.webgpu.WGPUFragmentState;
import com.github.xpenatan.webgpu.WGPUFrontFace;
import com.github.xpenatan.webgpu.WGPUIndexFormat;
import com.github.xpenatan.webgpu.WGPULoadOp;
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
import com.github.xpenatan.webgpu.WGPUStoreOp;
import com.github.xpenatan.webgpu.WGPUTextureFormat;
import com.github.xpenatan.webgpu.WGPUTextureView;
import com.github.xpenatan.webgpu.WGPUVectorBindGroupEntry;
import com.github.xpenatan.webgpu.WGPUVectorBindGroupLayout;
import com.github.xpenatan.webgpu.WGPUVectorBindGroupLayoutEntry;
import com.github.xpenatan.webgpu.WGPUVectorColorTargetState;
import com.github.xpenatan.webgpu.WGPUVectorCommandBuffer;
import com.github.xpenatan.webgpu.WGPUVectorConstantEntry;
import com.github.xpenatan.webgpu.WGPUVectorRenderPassColorAttachment;
import com.github.xpenatan.webgpu.WGPUVectorVertexAttribute;
import com.github.xpenatan.webgpu.WGPUVectorVertexBufferLayout;
import com.github.xpenatan.webgpu.WGPUVertexAttribute;
import com.github.xpenatan.webgpu.WGPUVertexBufferLayout;
import com.github.xpenatan.webgpu.WGPUVertexFormat;
import com.github.xpenatan.webgpu.WGPUVertexStepMode;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;
import com.github.xpenatan.webgpu.backend.core.graphics.CameraTransforms;
import com.github.xpenatan.webgpu.backend.core.graphics.DepthTextureTarget;
import com.github.xpenatan.webgpu.backend.core.graphics.GpuBufferUtils;
import com.github.xpenatan.webgpu.backend.core.graphics.LitMeshShaderSource;
import com.github.xpenatan.webgpu.backend.core.graphics.MeshData;
import com.github.xpenatan.webgpu.backend.core.graphics.SurfaceFrameContext;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class RotatingLitMeshDemo implements ApplicationListener {

    private static final int UNIFORM_SIZE_BYTES = 32 * Float.BYTES;

    private final MeshData meshData;
    private final float shaderRed;
    private final float shaderGreen;
    private final float shaderBlue;

    private final SurfaceFrameContext surfaceFrameContext = new SurfaceFrameContext();
    private final DepthTextureTarget depthTextureTarget = new DepthTextureTarget();

    private WGPURenderPipeline pipeline;
    private WGPUBuffer vertexBuffer;
    private WGPUBuffer indexBuffer;
    private WGPUBuffer uniformBuffer;
    private WGPUBindGroup bindGroup;

    private ByteBuffer uniformData;
    private int indexCount;
    private float angle;

    public RotatingLitMeshDemo(MeshData meshData, float shaderRed, float shaderGreen, float shaderBlue) {
        this.meshData = meshData;
        this.shaderRed = shaderRed;
        this.shaderGreen = shaderGreen;
        this.shaderBlue = shaderBlue;
    }

    @Override
    public void create(WGPUApp wgpu) {
        if(wgpu.surface == null) {
            System.out.println("Surface not created");
            return;
        }

        WGPUTextureFormat surfaceFormat = surfaceFrameContext.configure(wgpu, true);
        depthTextureTarget.create(wgpu.device, wgpu.width, wgpu.height, WGPUTextureFormat.Depth24Plus);

        setupBuffers(wgpu);
        setupPipeline(wgpu, surfaceFormat);

        wgpu.adapter.release();
        wgpu.adapter.dispose();
        wgpu.adapter = null;
    }

    @Override
    public void render(WGPUApp wgpu) {
        if(pipeline == null) {
            return;
        }

        updateUniforms(wgpu);

        WGPUTextureView targetView = surfaceFrameContext.acquireCurrentView(wgpu);

        WGPUCommandEncoder encoder = WGPUCommandEncoder.obtain();
        WGPUCommandEncoderDescriptor encoderDesc = WGPUCommandEncoderDescriptor.obtain();
        encoderDesc.setLabel("mesh command encoder");
        wgpu.device.createCommandEncoder(encoderDesc, encoder);

        WGPURenderPassColorAttachment colorAttachment = WGPURenderPassColorAttachment.obtain();
        colorAttachment.setView(targetView);
        colorAttachment.setResolveTarget(WGPUTextureView.NULL);
        colorAttachment.setLoadOp(WGPULoadOp.Clear);
        colorAttachment.setStoreOp(WGPUStoreOp.Store);
        colorAttachment.getClearValue().setColor(0.06f, 0.09f, 0.14f, 1.0f);

        WGPUVectorRenderPassColorAttachment colorAttachments = WGPUVectorRenderPassColorAttachment.obtain();
        colorAttachments.push_back(colorAttachment);

        WGPURenderPassDepthStencilAttachment depthAttachment = depthTextureTarget.createAttachment();

        WGPURenderPassDescriptor renderPassDesc = WGPURenderPassDescriptor.obtain();
        renderPassDesc.setColorAttachments(colorAttachments);
        renderPassDesc.setDepthStencilAttachment(depthAttachment);
        renderPassDesc.setTimestampWrites(WGPUPassTimestampWrites.NULL);

        WGPURenderPassEncoder renderPass = WGPURenderPassEncoder.obtain();
        encoder.beginRenderPass(renderPassDesc, renderPass);
        renderPass.setPipeline(pipeline);
        renderPass.setBindGroup(0, bindGroup);
        renderPass.setVertexBuffer(0, vertexBuffer, 0, vertexBuffer.getSize());
        renderPass.setIndexBuffer(indexBuffer, WGPUIndexFormat.Uint16, 0, indexBuffer.getSize());
        renderPass.drawIndexed(indexCount, 1, 0, 0, 0);
        renderPass.end();
        renderPass.release();

        WGPUCommandBuffer command = WGPUCommandBuffer.obtain();
        WGPUCommandBufferDescriptor commandDesc = WGPUCommandBufferDescriptor.obtain();
        commandDesc.setNextInChain(WGPUChainedStruct.NULL);
        commandDesc.setLabel("mesh command");
        encoder.finish(commandDesc, command);
        encoder.release();

        WGPUVectorCommandBuffer commands = WGPUVectorCommandBuffer.obtain();
        commands.push_back(command);
        wgpu.queue.submit(commands);
        command.release();

        targetView.release();
        surfaceFrameContext.present(wgpu);
    }

    @Override
    public void dispose() {
        if(pipeline != null) {
            pipeline.release();
            pipeline = null;
        }
        if(bindGroup != null) {
            bindGroup.release();
            bindGroup = null;
        }
        if(uniformBuffer != null) {
            uniformBuffer.release();
            uniformBuffer = null;
        }
        if(indexBuffer != null) {
            indexBuffer.release();
            indexBuffer = null;
        }
        if(vertexBuffer != null) {
            vertexBuffer.release();
            vertexBuffer = null;
        }
        depthTextureTarget.dispose();
    }

    private void setupBuffers(WGPUApp wgpu) {
        ByteBuffer vertexData = GpuBufferUtils.createFloatBuffer(meshData.vertices);
        ByteBuffer indexData = GpuBufferUtils.createShortBuffer(meshData.indices);

        vertexBuffer = GpuBufferUtils.createBuffer(
                wgpu,
                vertexData.limit(),
                WGPUBufferUsage.CopyDst.or(WGPUBufferUsage.Vertex)
        );
        GpuBufferUtils.upload(wgpu, vertexBuffer, vertexData);

        indexBuffer = GpuBufferUtils.createBuffer(
                wgpu,
                indexData.limit(),
                WGPUBufferUsage.CopyDst.or(WGPUBufferUsage.Index)
        );
        GpuBufferUtils.upload(wgpu, indexBuffer, indexData);

        uniformBuffer = GpuBufferUtils.createBuffer(
                wgpu,
                UNIFORM_SIZE_BYTES,
                WGPUBufferUsage.CopyDst.or(WGPUBufferUsage.Uniform)
        );

        uniformData = ByteBuffer.allocateDirect(UNIFORM_SIZE_BYTES);
        uniformData.order(ByteOrder.LITTLE_ENDIAN);
        indexCount = meshData.indices.length;
    }

    private void setupPipeline(WGPUApp wgpu, WGPUTextureFormat surfaceFormat) {
        WGPUBindGroupLayout bindGroupLayout = createBindGroupLayout(wgpu);
        WGPUPipelineLayout pipelineLayout = createPipelineLayout(wgpu, bindGroupLayout);
        pipeline = createRenderPipeline(wgpu, surfaceFormat, pipelineLayout);
        bindGroup = createBindGroup(wgpu, bindGroupLayout);
    }

    private WGPUBindGroupLayout createBindGroupLayout(WGPUApp wgpu) {
        WGPUBindGroupLayoutEntry layoutEntry = WGPUBindGroupLayoutEntry.obtain();
        layoutEntry.setBinding(0);
        layoutEntry.setVisibility(WGPUShaderStage.Vertex);

        WGPUBufferBindingLayout bindingLayout = WGPUBufferBindingLayout.obtain();
        bindingLayout.setType(WGPUBufferBindingType.Uniform);
        bindingLayout.setHasDynamicOffset(0);
        bindingLayout.setMinBindingSize(UNIFORM_SIZE_BYTES);
        layoutEntry.setBuffer(bindingLayout);

        WGPUVectorBindGroupLayoutEntry entries = WGPUVectorBindGroupLayoutEntry.obtain();
        entries.push_back(layoutEntry);

        WGPUBindGroupLayoutDescriptor descriptor = WGPUBindGroupLayoutDescriptor.obtain();
        descriptor.setEntries(entries);

        WGPUBindGroupLayout layout = new WGPUBindGroupLayout();
        wgpu.device.createBindGroupLayout(descriptor, layout);
        return layout;
    }

    private WGPUPipelineLayout createPipelineLayout(WGPUApp wgpu, WGPUBindGroupLayout bindGroupLayout) {
        WGPUVectorBindGroupLayout layouts = WGPUVectorBindGroupLayout.obtain();
        layouts.push_back(bindGroupLayout);

        WGPUPipelineLayoutDescriptor descriptor = WGPUPipelineLayoutDescriptor.obtain();
        descriptor.setBindGroupLayouts(layouts);

        WGPUPipelineLayout pipelineLayout = new WGPUPipelineLayout();
        wgpu.device.createPipelineLayout(descriptor, pipelineLayout);
        return pipelineLayout;
    }

    private WGPURenderPipeline createRenderPipeline(WGPUApp wgpu, WGPUTextureFormat surfaceFormat, WGPUPipelineLayout pipelineLayout) {
        String shaderCode = LitMeshShaderSource.shader(shaderRed, shaderGreen, shaderBlue);
        WGPUShaderModule shaderModule = createShaderModule(wgpu, shaderCode);

        WGPURenderPipelineDescriptor pipelineDesc = WGPURenderPipelineDescriptor.obtain();
        pipelineDesc.setLayout(pipelineLayout);
        pipelineDesc.setLabel("rotating mesh pipeline");

        WGPUVertexAttribute positionAttribute = new WGPUVertexAttribute();
        positionAttribute.setShaderLocation(0);
        positionAttribute.setFormat(WGPUVertexFormat.Float32x3);
        positionAttribute.setOffset(0);

        WGPUVertexAttribute normalAttribute = new WGPUVertexAttribute();
        normalAttribute.setShaderLocation(1);
        normalAttribute.setFormat(WGPUVertexFormat.Float32x3);
        normalAttribute.setOffset(3 * Float.BYTES);

        WGPUVectorVertexAttribute vertexAttributes = WGPUVectorVertexAttribute.obtain();
        vertexAttributes.push_back(positionAttribute);
        vertexAttributes.push_back(normalAttribute);

        WGPUVertexBufferLayout vertexBufferLayout = WGPUVertexBufferLayout.obtain();
        vertexBufferLayout.setArrayStride(MeshData.VERTEX_STRIDE_BYTES);
        vertexBufferLayout.setStepMode(WGPUVertexStepMode.Vertex);
        vertexBufferLayout.setAttributes(vertexAttributes);

        WGPUVectorVertexBufferLayout vertexBuffers = WGPUVectorVertexBufferLayout.obtain();
        vertexBuffers.push_back(vertexBufferLayout);

        pipelineDesc.getVertex().setBuffers(vertexBuffers);
        pipelineDesc.getVertex().setModule(shaderModule);
        pipelineDesc.getVertex().setEntryPoint("vs_main");
        pipelineDesc.getVertex().setConstants(WGPUVectorConstantEntry.NULL);

        pipelineDesc.getPrimitive().setTopology(WGPUPrimitiveTopology.TriangleList);
        pipelineDesc.getPrimitive().setFrontFace(WGPUFrontFace.CCW);

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
        colorTargets.push_back(colorTarget);
        fragmentState.setTargets(colorTargets);
        pipelineDesc.setFragment(fragmentState);

        WGPUDepthStencilState depthStencilState = depthTextureTarget.createDepthStencilState();
        pipelineDesc.setDepthStencil(depthStencilState);
        pipelineDesc.getMultisample().setCount(1);
        pipelineDesc.getMultisample().setMask(-1);
        pipelineDesc.getMultisample().setAlphaToCoverageEnabled(false);

        WGPURenderPipeline renderPipeline = new WGPURenderPipeline();
        wgpu.device.createRenderPipeline(pipelineDesc, renderPipeline);

        shaderModule.release();
        return renderPipeline;
    }

    private WGPUBindGroup createBindGroup(WGPUApp wgpu, WGPUBindGroupLayout bindGroupLayout) {
        WGPUBindGroupEntry entry = WGPUBindGroupEntry.obtain();
        entry.setBinding(0);
        entry.setBuffer(uniformBuffer);
        entry.setOffset(0);
        entry.setSize(UNIFORM_SIZE_BYTES);

        WGPUVectorBindGroupEntry entries = WGPUVectorBindGroupEntry.obtain();
        entries.push_back(entry);

        WGPUBindGroupDescriptor descriptor = WGPUBindGroupDescriptor.obtain();
        descriptor.setLayout(bindGroupLayout);
        descriptor.setEntries(entries);

        WGPUBindGroup result = new WGPUBindGroup();
        wgpu.device.createBindGroup(descriptor, result);
        return result;
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

    private void updateUniforms(WGPUApp wgpu) {
        angle += 0.01f;

        float[] model = new float[16];
        float[] mvp = new float[16];
        float aspect = (float)wgpu.width / (float)wgpu.height;
        CameraTransforms.buildModelViewProjection(model, mvp, angle, aspect);

        FloatBuffer floatBuffer = uniformData.asFloatBuffer();
        floatBuffer.position(0);
        floatBuffer.put(mvp);
        floatBuffer.put(model);

        wgpu.queue.writeBuffer(uniformBuffer, 0, uniformData, UNIFORM_SIZE_BYTES);
    }
}


