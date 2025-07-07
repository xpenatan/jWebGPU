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
import com.github.xpenatan.webgpu.WebGPUBlendState;
import com.github.xpenatan.webgpu.WebGPUBuffer;
import com.github.xpenatan.webgpu.WebGPUBufferDescriptor;
import com.github.xpenatan.webgpu.WebGPUColorTargetState;
import com.github.xpenatan.webgpu.WebGPUCommandBuffer;
import com.github.xpenatan.webgpu.WebGPUCommandBufferDescriptor;
import com.github.xpenatan.webgpu.WebGPUCommandEncoder;
import com.github.xpenatan.webgpu.WebGPUCommandEncoderDescriptor;
import com.github.xpenatan.webgpu.WebGPUFragmentState;
import com.github.xpenatan.webgpu.WebGPURenderPassColorAttachment;
import com.github.xpenatan.webgpu.WebGPURenderPassDescriptor;
import com.github.xpenatan.webgpu.WebGPURenderPassEncoder;
import com.github.xpenatan.webgpu.WebGPURenderPipeline;
import com.github.xpenatan.webgpu.WebGPURenderPipelineDescriptor;
import com.github.xpenatan.webgpu.WebGPUShaderModule;
import com.github.xpenatan.webgpu.WebGPUShaderModuleDescriptor;
import com.github.xpenatan.webgpu.WebGPUShaderSourceWGSL;
import com.github.xpenatan.webgpu.WebGPUSurfaceCapabilities;
import com.github.xpenatan.webgpu.WebGPUSurfaceConfiguration;
import com.github.xpenatan.webgpu.WebGPUSurfaceTexture;
import com.github.xpenatan.webgpu.WebGPUTexture;
import com.github.xpenatan.webgpu.WebGPUTextureView;
import com.github.xpenatan.webgpu.WebGPUTextureViewDescriptor;
import com.github.xpenatan.webgpu.WebGPUVertexAttribute;
import com.github.xpenatan.webgpu.WebGPUVertexBufferLayout;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;

public class AFirstVertexAttribute implements ApplicationListener {

    private WebGPURenderPipeline pipeline;
    private WGPUTextureFormat surfaceFormat;
    private int vertexCount;
    private WebGPUBuffer vertexBuffer;

    @Override
    public void create(WGPUApp wgpu) {
        if(wgpu.surface != null) {
            System.out.println("Surface created");
            WebGPUSurfaceCapabilities surfaceCapabilities = new WebGPUSurfaceCapabilities();
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

        WebGPUTextureView targetView = GetNextSurfaceTextureView(wgpu);

        // Create a command encoder for the draw call
        WebGPUCommandEncoderDescriptor encoderDesc = new WebGPUCommandEncoderDescriptor();
        encoderDesc.setNextInChain(null);
        encoderDesc.setLabel("My command encoder");
        WebGPUCommandEncoder encoder = new WebGPUCommandEncoder();
        wgpu.device.createCommandEncoder(encoderDesc, encoder);

        // Create the render pass that clears the screen with our color
        WebGPURenderPassDescriptor renderPassDesc = new WebGPURenderPassDescriptor();
        renderPassDesc.setNextInChain(null);

        // The attachment part of the render pass descriptor describes the target texture of the pass
        WebGPURenderPassColorAttachment renderPassColorAttachment = new WebGPURenderPassColorAttachment();
        renderPassColorAttachment.setView(targetView);
        renderPassColorAttachment.setResolveTarget(null);
        renderPassColorAttachment.setLoadOp(WGPULoadOp.Clear);
        renderPassColorAttachment.setStoreOp(WGPUStoreOp.Store);
        renderPassColorAttachment.getClearValue().setColor(0.9f, 0.1f, 0.2f, 1.0f);

        WGPUVectorRenderPassColorAttachment attachments = new WGPUVectorRenderPassColorAttachment();
        attachments.push_back(renderPassColorAttachment);
        renderPassDesc.setColorAttachments(attachments);
        renderPassDesc.setDepthStencilAttachment(null);
        renderPassDesc.setTimestampWrites(null);

        WebGPURenderPassEncoder renderPass = new WebGPURenderPassEncoder();
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
        WebGPUCommandBufferDescriptor cmdBufferDescriptor = new WebGPUCommandBufferDescriptor();
        cmdBufferDescriptor.setNextInChain(null);
        cmdBufferDescriptor.setLabel("Command buffer");
        WebGPUCommandBuffer command = new WebGPUCommandBuffer();
        encoder.finish(cmdBufferDescriptor, command);
        encoder.release();

        wgpu.queue.submit(1, command);
        command.release();
        command.dispose();

        // At the end of the frame
        targetView.release();

        if(WGPU.getPlatformType() != WGPUPlatformType.WGPU_Web) {
            wgpu.surface.present();
        }
    }

    private void initSwapChain(WGPUApp wgpu) {
        // Configure the surface
        WebGPUSurfaceConfiguration config = new WebGPUSurfaceConfiguration();

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

    private WebGPUTextureView GetNextSurfaceTextureView(WGPUApp wgpu) {
        WebGPUTextureView textureViewOut = new WebGPUTextureView();
        WebGPUSurfaceTexture surfaceTextureOut = new WebGPUSurfaceTexture();
        wgpu.surface.getCurrentTexture(surfaceTextureOut);
        WebGPUTexture textureOut = new WebGPUTexture();
        surfaceTextureOut.getTexture(textureOut);
        WGPUTextureFormat textureFormat = textureOut.getFormat();
        WebGPUTextureViewDescriptor viewDescriptor = new WebGPUTextureViewDescriptor();
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
        WebGPUShaderModuleDescriptor shaderDesc = new WebGPUShaderModuleDescriptor();

        // We use the extension mechanism to specify the WGSL part of the shader module descriptor
        WebGPUShaderSourceWGSL shaderCodeDesc = new WebGPUShaderSourceWGSL();
        // Set the chained struct's header
        shaderCodeDesc.getChain().setNext(null);
        shaderCodeDesc.getChain().setSType(WGPUSType.ShaderSourceWGSL);
        // Connect the chain
        shaderDesc.setNextInChain(shaderCodeDesc.getChain());
        shaderCodeDesc.setCode(shaderSource);
        WebGPUShaderModule shaderModule = new WebGPUShaderModule();
        wgpu.device.createShaderModule(shaderDesc, shaderModule);

        // Create the render pipeline
        WebGPURenderPipelineDescriptor pipelineDesc = new WebGPURenderPipelineDescriptor();
        pipelineDesc.setNextInChain(null);

        // Configure the vertex pipeline
        // We use one vertex buffer
        WebGPUVertexBufferLayout vertexBufferLayout = new WebGPUVertexBufferLayout();
        WebGPUVertexAttribute positionAttrib = new WebGPUVertexAttribute();
        // == For each attribute, describe its layout, i.e., how to interpret the raw data ==
        // Corresponds to @location(...)
        positionAttrib.setShaderLocation(0);
        // Means vec2f in the shader
        positionAttrib.setFormat(WGPUVertexFormat.Float32x2);
        // Index of the first element
        positionAttrib.setOffset(0);

        WGPUVectorVertexAttribute attributes = new WGPUVectorVertexAttribute();
        attributes.push_back(positionAttrib);
        vertexBufferLayout.setAttributes(attributes);

        // == Common to attributes from the same buffer ==
        vertexBufferLayout.setArrayStride(2 * Float.BYTES);
        vertexBufferLayout.setStepMode(WGPUVertexStepMode.Vertex);

        WGPUVectorVertexBufferLayout buffers = new WGPUVectorVertexBufferLayout();
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
        WebGPUFragmentState fragmentState = new WebGPUFragmentState();
        fragmentState.setModule(shaderModule);
        fragmentState.setEntryPoint("fs_main");
        fragmentState.setConstants(null);

        WebGPUBlendState blendState = new WebGPUBlendState();
        blendState.getColor().setSrcFactor(WGPUBlendFactor.SrcAlpha);
        blendState.getColor().setDstFactor(WGPUBlendFactor.OneMinusSrcAlpha);
        blendState.getColor().setOperation(WGPUBlendOperation.Add);
        blendState.getAlpha().setSrcFactor(WGPUBlendFactor.Zero);
        blendState.getAlpha().setDstFactor(WGPUBlendFactor.One);
        blendState.getAlpha().setOperation(WGPUBlendOperation.Add);

        WebGPUColorTargetState colorTarget = new WebGPUColorTargetState();
        colorTarget.setFormat(surfaceFormat);
        colorTarget.setBlend(blendState);
        colorTarget.setWriteMask(WGPUColorWriteMask.All); // We could write to only some of the color channels.

        // We have only one target because our render pass has only one output color
        // attachment.
        WGPUVectorColorTargetState targets = new WGPUVectorColorTargetState();
        targets.push_back(colorTarget);
        fragmentState.setTargets(targets);
        pipelineDesc.setFragment(fragmentState);

        // We do not use stencil/depth testing for now
        pipelineDesc.setDepthStencil(null);

        // Samples per pixel
        pipelineDesc.getMultisample().setCount(1);

        // Default value for the mask, meaning "all bits on"
        pipelineDesc.getMultisample().setMask(0);

        // Default value as well (irrelevant for count = 1 anyways)
        pipelineDesc.getMultisample().setAlphaToCoverageEnabled(false);

        pipelineDesc.setLayout(null);

        pipeline = new WebGPURenderPipeline();
        wgpu.device.createRenderPipeline(pipelineDesc, pipeline);

        // We no longer need to access the shader module
        shaderModule.release();
    }

    private void initializeBuffers(WGPUApp wgpu) {
        // Vertex buffer data
        // There are 2 floats per vertex, one for x and one for y.

        WGPUByteBuffer vertexData = new WGPUByteBuffer(12 * Float.BYTES);

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

        vertexCount = floatBuffer.getLimit() / 2;

        // Create vertex buffer
        WebGPUBufferDescriptor bufferDesc = new WebGPUBufferDescriptor();
        bufferDesc.setNextInChain(null);
        bufferDesc.setSize(vertexData.getLimit());
        int value = WGPUBufferUsage.CopyDst.getValue() | WGPUBufferUsage.Vertex.getValue();
        bufferDesc.setUsage(WGPUBufferUsage.CopyDst.or(WGPUBufferUsage.Vertex)); // Vertex usage here!
        bufferDesc.setMappedAtCreation(false);
        vertexBuffer = new WebGPUBuffer();
        wgpu.device.createBuffer(bufferDesc, vertexBuffer);
        bufferDesc.setLabel("Output buffer");
        bufferDesc.setUsage(WGPUBufferUsage.CopyDst.or(WGPUBufferUsage.MapRead));
        WebGPUBuffer buffer2 = new WebGPUBuffer();
        wgpu.device.createBuffer(bufferDesc, buffer2);

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
