package com.github.xpenatan.webgpu.demo.app;

import com.github.xpenatan.webgpu.WGPU;
import com.github.xpenatan.webgpu.WGPUBlendFactor;
import com.github.xpenatan.webgpu.WGPUBlendOperation;
import com.github.xpenatan.webgpu.WGPUBufferUsage;
import com.github.xpenatan.webgpu.WGPUByteBuffer;
import com.github.xpenatan.webgpu.WGPUByteOrder;
import com.github.xpenatan.webgpu.WGPUColorWriteMask;
import com.github.xpenatan.webgpu.WGPUCompositeAlphaMode;
import com.github.xpenatan.webgpu.WGPUCullMode;
import com.github.xpenatan.webgpu.WGPUFloatBuffer;
import com.github.xpenatan.webgpu.WGPUFrontFace;
import com.github.xpenatan.webgpu.WGPUIndexFormat;
import com.github.xpenatan.webgpu.WGPULoadOp;
import com.github.xpenatan.webgpu.WGPUPlatformType;
import com.github.xpenatan.webgpu.WGPUPresentMode;
import com.github.xpenatan.webgpu.WGPUPrimitiveTopology;
import com.github.xpenatan.webgpu.WGPUSType;
import com.github.xpenatan.webgpu.WGPUStoreOp;
import com.github.xpenatan.webgpu.WGPUTextureAspect;
import com.github.xpenatan.webgpu.WGPUTextureFormat;
import com.github.xpenatan.webgpu.WGPUTextureUsage;
import com.github.xpenatan.webgpu.WGPUTextureViewDimension;
import com.github.xpenatan.webgpu.WGPUVectorColorTargetState;
import com.github.xpenatan.webgpu.WGPUVectorRenderPassColorAttachment;
import com.github.xpenatan.webgpu.WGPUVectorTextureFormat;
import com.github.xpenatan.webgpu.WGPUVectorVertexAttribute;
import com.github.xpenatan.webgpu.WGPUVectorVertexBufferLayout;
import com.github.xpenatan.webgpu.WGPUVertexFormat;
import com.github.xpenatan.webgpu.WGPUVertexStepMode;
import com.github.xpenatan.webgpu.WGPUBlendState;
import com.github.xpenatan.webgpu.WGPUBuffer;
import com.github.xpenatan.webgpu.WGPUBufferDescriptor;
import com.github.xpenatan.webgpu.WGPUColorTargetState;
import com.github.xpenatan.webgpu.WGPUCommandBuffer;
import com.github.xpenatan.webgpu.WGPUCommandBufferDescriptor;
import com.github.xpenatan.webgpu.WGPUCommandEncoder;
import com.github.xpenatan.webgpu.WGPUCommandEncoderDescriptor;
import com.github.xpenatan.webgpu.WGPUFragmentState;
import com.github.xpenatan.webgpu.WGPURenderPassColorAttachment;
import com.github.xpenatan.webgpu.WGPURenderPassDescriptor;
import com.github.xpenatan.webgpu.WGPURenderPassEncoder;
import com.github.xpenatan.webgpu.WGPURenderPipeline;
import com.github.xpenatan.webgpu.WGPURenderPipelineDescriptor;
import com.github.xpenatan.webgpu.WGPUShaderModule;
import com.github.xpenatan.webgpu.WGPUShaderModuleDescriptor;
import com.github.xpenatan.webgpu.WGPUShaderSourceWGSL;
import com.github.xpenatan.webgpu.WGPUSurfaceCapabilities;
import com.github.xpenatan.webgpu.WGPUSurfaceConfiguration;
import com.github.xpenatan.webgpu.WGPUSurfaceTexture;
import com.github.xpenatan.webgpu.WGPUTexture;
import com.github.xpenatan.webgpu.WGPUTextureView;
import com.github.xpenatan.webgpu.WGPUTextureViewDescriptor;
import com.github.xpenatan.webgpu.WGPUVertexAttribute;
import com.github.xpenatan.webgpu.WGPUVertexBufferLayout;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;

public class AFirstVertexAttribute implements ApplicationListener {

    private WGPURenderPipeline pipeline;
    private WGPUTextureFormat surfaceFormat;
    private int vertexCount;
    private WGPUBuffer vertexBuffer;

    private WGPURenderPassDescriptor renderPassDesc;
    private WGPUVectorRenderPassColorAttachment attachments;
    private WGPURenderPassColorAttachment renderPassColorAttachment;

    @Override
    public void create(WGPUApp wgpu) {
        if(wgpu.surface != null) {
            renderPassDesc = new WGPURenderPassDescriptor();
            attachments = new WGPUVectorRenderPassColorAttachment();
            renderPassColorAttachment = new WGPURenderPassColorAttachment();

            System.out.println("Surface created");
            WGPUSurfaceCapabilities surfaceCapabilities = WGPUSurfaceCapabilities.obtain();
            wgpu.surface.getCapabilities(wgpu.adapter, surfaceCapabilities);
            WGPUVectorTextureFormat formats = surfaceCapabilities.getFormats();
            surfaceFormat = formats.get(0);
            System.out.println("surfaceFormat: " + surfaceFormat);
            initSwapChain(wgpu);

            // Release the adapter only after it has been fully utilized
            wgpu.adapter.release();
            wgpu.adapter.dispose();
            wgpu.adapter = null;

            initializePipeline(wgpu);
            initializeBuffers(wgpu);
        }
        else {
            System.out.println("Surface not created");
        }
    }

    @Override
    public void render(WGPUApp wgpu) {

        WGPUTextureView targetView = GetNextSurfaceTextureView(wgpu);

        // Create a command encoder for the draw call
        WGPUCommandEncoderDescriptor encoderDesc = WGPUCommandEncoderDescriptor.obtain();
        encoderDesc.setNextInChain(null);
        encoderDesc.setLabel("My command encoder");
        WGPUCommandEncoder encoder = WGPUCommandEncoder.obtain();
        wgpu.device.createCommandEncoder(encoderDesc, encoder);

        // Create the render pass that clears the screen with our color
        renderPassDesc.setNextInChain(null);

        // The attachment part of the render pass descriptor describes the target texture of the pass
        renderPassColorAttachment.reset();
        renderPassColorAttachment.setView(targetView);
        renderPassColorAttachment.setResolveTarget(null);
        renderPassColorAttachment.setLoadOp(WGPULoadOp.Clear);
        renderPassColorAttachment.setStoreOp(WGPUStoreOp.Store);
        renderPassColorAttachment.getClearValue().setColor(0.9f, 0.1f, 0.2f, 1.0f);

        attachments.clear();
        attachments.push_back(renderPassColorAttachment);
        renderPassDesc.reset();
        renderPassDesc.setColorAttachments(attachments);
        renderPassDesc.setDepthStencilAttachment(null);
        renderPassDesc.setTimestampWrites(null);

        WGPURenderPassEncoder renderPass = WGPURenderPassEncoder.obtain();
        encoder.beginRenderPass(renderPassDesc, renderPass);

        // Select which render pipeline to use
        renderPass.setPipeline(pipeline);

        // Set vertex buffer while encoding the render pass
        renderPass.setVertexBuffer(0, vertexBuffer, 0, vertexBuffer.getSize());

        // We use the `vertexCount` variable instead of hard-coding the vertex count
        renderPass.draw(vertexCount, 1, 0, 0);

        renderPass.end();
        renderPass.release();

        // Encode and submit the render pass
        WGPUCommandBufferDescriptor cmdBufferDescriptor = WGPUCommandBufferDescriptor.obtain();
        cmdBufferDescriptor.setNextInChain(null);
        cmdBufferDescriptor.setLabel("Command buffer");
        WGPUCommandBuffer command = WGPUCommandBuffer.obtain();
        encoder.finish(cmdBufferDescriptor, command);
        encoder.release();

        wgpu.queue.submit(1, command);
        command.release();

        // At the end of the frame
        targetView.release();

        if(WGPU.getPlatformType() != WGPUPlatformType.WGPU_Web) {
            wgpu.surface.present();
        }
    }

    private void initSwapChain(WGPUApp wgpu) {
        // Configure the surface
        WGPUSurfaceConfiguration config = WGPUSurfaceConfiguration.obtain();

        // Configuration of the textures created for the underlying swap chain
        config.setWidth(wgpu.width);
        config.setHeight(wgpu.height);
        config.setUsage(WGPUTextureUsage.RenderAttachment);
        config.setFormat(surfaceFormat);

        // And we do not need any particular view format:
        config.setViewFormats(null);
        config.setDevice(wgpu.device);
        config.setPresentMode(WGPUPresentMode.Fifo);
        config.setAlphaMode(WGPUCompositeAlphaMode.Auto);
        wgpu.surface.configure(config);
    }

    private WGPUTextureView GetNextSurfaceTextureView(WGPUApp wgpu) {
        WGPUTextureView textureViewOut = WGPUTextureView.obtain();
        WGPUSurfaceTexture surfaceTextureOut = WGPUSurfaceTexture.obtain();
        wgpu.surface.getCurrentTexture(surfaceTextureOut);
        WGPUTexture textureOut = WGPUTexture.obtain();
        surfaceTextureOut.getTexture(textureOut);
        WGPUTextureFormat textureFormat = textureOut.getFormat();
        WGPUTextureViewDescriptor viewDescriptor = WGPUTextureViewDescriptor.obtain();
        viewDescriptor.setLabel("Surface texture view");
        viewDescriptor.setFormat(textureFormat);
        viewDescriptor.setDimension(WGPUTextureViewDimension._2D);
        viewDescriptor.setBaseMipLevel(0);
        viewDescriptor.setMipLevelCount(1);
        viewDescriptor.setBaseArrayLayer(0);
        viewDescriptor.setArrayLayerCount(1);
        viewDescriptor.setAspect(WGPUTextureAspect.All);
        textureOut.createView(viewDescriptor, textureViewOut);

        // We no longer need the texture, only its view
        textureOut.release();
        return textureViewOut;
    }

    void initializePipeline(WGPUApp wgpu) {
        // Load the shader module
        WGPUShaderModuleDescriptor shaderDesc = WGPUShaderModuleDescriptor.obtain();

        // We use the extension mechanism to specify the WGSL part of the shader module descriptor
        WGPUShaderSourceWGSL shaderCodeDesc = WGPUShaderSourceWGSL.obtain();
        // Set the chained struct's header
        shaderCodeDesc.getChain().setNext(null);
        shaderCodeDesc.getChain().setSType(WGPUSType.ShaderSourceWGSL);
        // Connect the chain
        shaderDesc.setNextInChain(shaderCodeDesc.getChain());
        shaderCodeDesc.setCode(shaderSource);
        WGPUShaderModule shaderModule = WGPUShaderModule.obtain();
        wgpu.device.createShaderModule(shaderDesc, shaderModule);

        // Create the render pipeline
        WGPURenderPipelineDescriptor pipelineDesc = WGPURenderPipelineDescriptor.obtain();
        pipelineDesc.setNextInChain(null);

        // Configure the vertex pipeline
        // We use one vertex buffer
        WGPUVertexBufferLayout vertexBufferLayout = WGPUVertexBufferLayout.obtain();
        WGPUVertexAttribute positionAttrib = WGPUVertexAttribute.obtain();
        // == For each attribute, describe its layout, i.e., how to interpret the raw data ==
        // Corresponds to @location(...)
        positionAttrib.setShaderLocation(0);
        // Means vec2f in the shader
        positionAttrib.setFormat(WGPUVertexFormat.Float32x2);
        // Index of the first element
        positionAttrib.setOffset(0);

        WGPUVectorVertexAttribute attributes = WGPUVectorVertexAttribute.obtain();
        attributes.push_back(positionAttrib);
        vertexBufferLayout.setAttributes(attributes);

        // == Common to attributes from the same buffer ==
        vertexBufferLayout.setArrayStride(2 * Float.BYTES);
        vertexBufferLayout.setStepMode(WGPUVertexStepMode.Vertex);

        WGPUVectorVertexBufferLayout buffers = WGPUVectorVertexBufferLayout.obtain();
        buffers.push_back(vertexBufferLayout);
        pipelineDesc.getVertex().setBuffers(buffers);

        // NB: We define the 'shaderModule' in the second part of this chapter.
        // Here we tell that the programmable vertex shader stage is described
        // by the function called 'vs_main' in that module.
        pipelineDesc.getVertex().setModule(shaderModule);
        pipelineDesc.getVertex().setEntryPoint("vs_main");
        pipelineDesc.getVertex().setConstants(null);

        // Each sequence of 3 vertices is considered as a triangle
        pipelineDesc.getPrimitive().setTopology(WGPUPrimitiveTopology.TriangleList);

        // We'll see later how to specify the order in which vertices should be
        // connected. When not specified, vertices are considered sequentially.
        pipelineDesc.getPrimitive().setStripIndexFormat(WGPUIndexFormat.Undefined);

        // The face orientation is defined by assuming that when looking
        // from the front of the face, its corner vertices are enumerated
        // in the counter-clockwise (CCW) order.
        pipelineDesc.getPrimitive().setFrontFace(WGPUFrontFace.CCW);

        // But the face orientation does not matter much because we do not
        // cull (i.e. "hide") the faces pointing away from us (which is often
        // used for optimization).
        pipelineDesc.getPrimitive().setCullMode(WGPUCullMode.None);

        // We tell that the programmable fragment shader stage is described
        // by the function called 'fs_main' in the shader module.
        WGPUFragmentState fragmentState = WGPUFragmentState.obtain();
        fragmentState.setModule(shaderModule);
        fragmentState.setEntryPoint("fs_main");
        fragmentState.setConstants(null);

        WGPUBlendState blendState = WGPUBlendState.obtain();
        blendState.getColor().setSrcFactor(WGPUBlendFactor.SrcAlpha);
        blendState.getColor().setDstFactor(WGPUBlendFactor.OneMinusSrcAlpha);
        blendState.getColor().setOperation(WGPUBlendOperation.Add);
        blendState.getAlpha().setSrcFactor(WGPUBlendFactor.Zero);
        blendState.getAlpha().setDstFactor(WGPUBlendFactor.One);
        blendState.getAlpha().setOperation(WGPUBlendOperation.Add);

        WGPUColorTargetState colorTarget = WGPUColorTargetState.obtain();
        colorTarget.setFormat(surfaceFormat);
        colorTarget.setBlend(blendState);
        colorTarget.setWriteMask(WGPUColorWriteMask.All); // We could write to only some of the color channels.

        // We have only one target because our render pass has only one output color
        // attachment.
        WGPUVectorColorTargetState targets = WGPUVectorColorTargetState.obtain();
        targets.push_back(colorTarget);
        fragmentState.setTargets(targets);
        pipelineDesc.setFragment(fragmentState);

        // We do not use stencil/depth testing for now
        pipelineDesc.setDepthStencil(null);

        // Samples per pixel
        pipelineDesc.getMultisample().setCount(1);

        // Default value for the mask, meaning "all bits on"
        pipelineDesc.getMultisample().setMask(1);

        // Default value as well (irrelevant for count = 1 anyways)
        pipelineDesc.getMultisample().setAlphaToCoverageEnabled(false);

        pipelineDesc.setLayout(null);

        pipeline = new WGPURenderPipeline();
        wgpu.device.createRenderPipeline(pipelineDesc, pipeline);

        // We no longer need to access the shader module
        shaderModule.release();
    }

    private void initializeBuffers(WGPUApp wgpu) {
        // Vertex buffer data
        // There are 2 floats per vertex, one for x and one for y.

        WGPUByteBuffer vertexData = WGPUByteBuffer.obtain(12 * Float.BYTES);

        vertexData.order(WGPUByteOrder.LittleEndian);

        WGPUFloatBuffer floatBuffer = vertexData.asFloatBuffer();

        // Define a first triangle:
        floatBuffer.put(-0.5f);
        floatBuffer.put(-0.5f);
        floatBuffer.put(+0.5f);
        floatBuffer.put(-0.5f);
        floatBuffer.put(+0.0f);
        floatBuffer.put(+0.5f);

        // Add a second triangle:
        floatBuffer.put(-0.55f);
        floatBuffer.put(-0.5f);
        floatBuffer.put(-0.05f);
        floatBuffer.put(+0.5f);
        floatBuffer.put(-0.55f);
        floatBuffer.put(+0.5f);

        for(int i = 0; i < floatBuffer.getLimit(); i++) {
            float v = floatBuffer.get(i);
            System.out.println(i + " Value: " + v);
        }

        vertexCount = floatBuffer.getLimit() / 2;

        // Create vertex buffer
        WGPUBufferDescriptor bufferDesc = WGPUBufferDescriptor.obtain();
        bufferDesc.setNextInChain(null);
        bufferDesc.setSize(vertexData.getLimit());
        bufferDesc.setUsage(WGPUBufferUsage.CopyDst.or(WGPUBufferUsage.Vertex)); // Vertex usage here!
        bufferDesc.setMappedAtCreation(false);
        vertexBuffer = new WGPUBuffer();
        wgpu.device.createBuffer(bufferDesc, vertexBuffer);

//        WGPU.testWriteBuffer(wgpu.queue, vertexBuffer);
        // Upload geometry data to the buffer
        wgpu.queue.writeBuffer(vertexBuffer,0, vertexData);
    }

    public String shaderSource =
            "@vertex\n" +
            "fn vs_main(@location(0) in_vertex_position: vec2f) -> @builtin(position) vec4f {\n" +
            "    return vec4f(in_vertex_position, 0.0, 1.0);\n" +
            "}\n" +
            "\n" +
            "@fragment\n" +
            "fn fs_main() -> @location(0) vec4f {\n" +
            "    return vec4f(0.0, 0.4, 1.0, 1.0);\n" +
            "}";
}
