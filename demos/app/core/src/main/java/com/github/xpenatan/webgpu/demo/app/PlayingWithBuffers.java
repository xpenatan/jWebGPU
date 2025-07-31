package com.github.xpenatan.webgpu.demo.app;

import com.github.xpenatan.webgpu.BufferMapCallback;
import com.github.xpenatan.webgpu.WGPU;
import com.github.xpenatan.webgpu.WGPUBlendFactor;
import com.github.xpenatan.webgpu.WGPUBlendOperation;
import com.github.xpenatan.webgpu.WGPUBufferUsage;
import com.github.xpenatan.webgpu.WGPUByteBuffer;
import com.github.xpenatan.webgpu.WGPUCallbackMode;
import com.github.xpenatan.webgpu.WGPUColorWriteMask;
import com.github.xpenatan.webgpu.WGPUCompositeAlphaMode;
import com.github.xpenatan.webgpu.WGPUCullMode;
import com.github.xpenatan.webgpu.WGPUFloatBuffer;
import com.github.xpenatan.webgpu.WGPUFrontFace;
import com.github.xpenatan.webgpu.WGPUIndexFormat;
import com.github.xpenatan.webgpu.WGPULoadOp;
import com.github.xpenatan.webgpu.WGPUMapAsyncStatus;
import com.github.xpenatan.webgpu.WGPUMapMode;
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
import com.github.xpenatan.webgpu.WGPUBlendState;
import com.github.xpenatan.webgpu.WGPUBuffer;
import com.github.xpenatan.webgpu.WGPUBufferDescriptor;
import com.github.xpenatan.webgpu.WGPUColorTargetState;
import com.github.xpenatan.webgpu.WGPUCommandBuffer;
import com.github.xpenatan.webgpu.WGPUCommandBufferDescriptor;
import com.github.xpenatan.webgpu.WGPUCommandEncoder;
import com.github.xpenatan.webgpu.WGPUCommandEncoderDescriptor;
import com.github.xpenatan.webgpu.WGPUFragmentState;
import com.github.xpenatan.webgpu.WGPUFuture;
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
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class PlayingWithBuffers implements ApplicationListener {

    private WGPURenderPipeline pipeline;
    private WGPUTextureFormat surfaceFormat;

    @Override
    public void create(WGPUApp wgpu) {
        if(wgpu.surface != null) {
            System.out.println("Surface created");
            WGPUSurfaceCapabilities surfaceCapabilities = new WGPUSurfaceCapabilities();
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
            playingWithBuffers(wgpu);

            testWGPUBuffer();
        }
        else {
            System.out.println("Surface not created");
        }
    }

    private void testWGPUBuffer() {
        WGPUByteBuffer buffer = new WGPUByteBuffer(4 * Float.BYTES);
        WGPUFloatBuffer floatBuffer = buffer.asFloatBuffer();
        floatBuffer.put(0, 1);
        floatBuffer.put(1, 2);
        floatBuffer.put(2, 3);
        floatBuffer.put(3, 4);

        for(int i = 0; i < floatBuffer.getLimit(); i++) {
            float b = floatBuffer.get(i);
            System.out.println("[" + i + "]: " + b);
        }

        float [] data = new float[4];
        data[0] = 4;
        data[1] = 3;
        data[2] = 2;
        data[3] = 1;
        floatBuffer.put(data, 0, 4);

        for(int i = 0; i < floatBuffer.getLimit(); i++) {
            float b = floatBuffer.get(i);
            System.out.println("[" + i + "]: " + b);
        }

        buffer.dispose();
    }

    @Override
    public void render(WGPUApp wgpu) {

        WGPUTextureView targetView = GetNextSurfaceTextureView(wgpu);

        // Create a command encoder for the draw call
        WGPUCommandEncoderDescriptor encoderDesc = WGPUCommandEncoderDescriptor.obtain();
        encoderDesc.setNextInChain(null);
        encoderDesc.setLabel("My command encoder");
        WGPUCommandEncoder encoder = new WGPUCommandEncoder();
        wgpu.device.createCommandEncoder(encoderDesc, encoder);

        // Create the render pass that clears the screen with our color
        WGPURenderPassDescriptor renderPassDesc = WGPURenderPassDescriptor.obtain();
        renderPassDesc.setNextInChain(null);

        // The attachment part of the render pass descriptor describes the target texture of the pass
        WGPURenderPassColorAttachment renderPassColorAttachment = WGPURenderPassColorAttachment.obtain();
        renderPassColorAttachment.setView(targetView);
        renderPassColorAttachment.setResolveTarget(null);
        renderPassColorAttachment.setLoadOp(WGPULoadOp.Clear);
        renderPassColorAttachment.setStoreOp(WGPUStoreOp.Store);
        renderPassColorAttachment.getClearValue().setColor(0.9f, 0.1f, 0.2f, 1.0f);

        WGPUVectorRenderPassColorAttachment attachments = WGPUVectorRenderPassColorAttachment.obtain();
        attachments.push_back(renderPassColorAttachment);
        renderPassDesc.setColorAttachments(attachments);
        renderPassDesc.setDepthStencilAttachment(null);
        renderPassDesc.setTimestampWrites(null);

        WGPURenderPassEncoder renderPass = WGPURenderPassEncoder.obtain();
        encoder.beginRenderPass(renderPassDesc, renderPass);

        // Select which render pipeline to use
        renderPass.setPipeline(pipeline);
        // Draw 1 instance of a 3-vertices shape
        renderPass.draw(3, 1, 0, 0);

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

    @Override
    public void dispose() {

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
        textureOut.release();
        return textureViewOut;
    }

    void initializePipeline(WGPUApp wgpu) {
        // Load the shader module
        WGPUShaderModuleDescriptor shaderDesc = new WGPUShaderModuleDescriptor();

        // We use the extension mechanism to specify the WGSL part of the shader module descriptor
        WGPUShaderSourceWGSL shaderCodeDesc = WGPUShaderSourceWGSL.obtain();
        // Set the chained struct's header
        shaderCodeDesc.getChain().setNext(null);
        shaderCodeDesc.getChain().setSType(WGPUSType.ShaderSourceWGSL);
        // Connect the chain
        shaderDesc.setNextInChain(shaderCodeDesc.getChain());
        shaderCodeDesc.setCode(shaderSource);
        WGPUShaderModule shaderModule = new WGPUShaderModule();
        wgpu.device.createShaderModule(shaderDesc, shaderModule);

        // Create the render pipeline
        WGPURenderPipelineDescriptor pipelineDesc = WGPURenderPipelineDescriptor.obtain();
        pipelineDesc.setNextInChain(null);

        // We do not use any vertex buffer for this first simplistic example
        pipelineDesc.getVertex().setBuffers(null);

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
        blendState.getColor().setSrcFactor(WGPUBlendFactor.Zero);
        blendState.getColor().setDstFactor(WGPUBlendFactor.One);
        blendState.getColor().setOperation(WGPUBlendOperation.Add);

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

    void playingWithBuffers(WGPUApp wgpu) {
        // Experimentation for the "Playing with buffer" chapter
        WGPUBufferDescriptor bufferDesc = WGPUBufferDescriptor.obtain();
        bufferDesc.setNextInChain(null);
        bufferDesc.setLabel("Some GPU-side data buffer");
        bufferDesc.setUsage(WGPUBufferUsage.CopyDst.or(WGPUBufferUsage.CopySrc));
        bufferDesc.setSize(16);
        bufferDesc.setMappedAtCreation(false);
        WGPUBuffer buffer1 = wgpu.device.createBuffer(bufferDesc);
        bufferDesc.setLabel("Output buffer");
        bufferDesc.setUsage(WGPUBufferUsage.CopyDst.or(WGPUBufferUsage.MapRead));
        WGPUBuffer buffer2 = wgpu.device.createBuffer(bufferDesc);

        // Create some CPU-side data buffer (of size 16 bytes)
        ByteBuffer numbers = ByteBuffer.allocateDirect(16);
        for (int i = 0; i < 16; ++i) {
            numbers.put(i, (byte)i);
        }
        // `numbers` now contains [ 0, 1, 2, ... ]

        // Copy this from `numbers` (RAM) to `buffer1` (VRAM)
        wgpu.queue.writeBuffer(buffer1, 0, numbers, numbers.limit());

        WGPUCommandEncoder encoder = new WGPUCommandEncoder();
        wgpu.device.createCommandEncoder(null, encoder);

        // After creating the command encoder
        encoder.copyBufferToBuffer(buffer1, 0, buffer2, 0, 16);

        WGPUCommandBuffer command = new WGPUCommandBuffer();
        encoder.finish(null, command);
        encoder.release();
        wgpu.queue.submit(1, command);
        command.release();

        boolean [] ready = new boolean[1];

        WGPUFuture webGPUFuture = buffer2.mapAsync(WGPUMapMode.Read, 0, 16, WGPUCallbackMode.AllowProcessEvents, new BufferMapCallback() {
            @Override
            protected void onCallback(WGPUMapAsyncStatus status, String message) {
                ready[0] = true;
                System.out.println("Buffer 2 mapped with status " + status);
                if(status != WGPUMapAsyncStatus.Success) return;

                // Get a pointer to wherever the driver mapped the GPU memory to the RAM
                ByteBuffer byteBuffer = ByteBuffer.allocateDirect(16);
                byteBuffer.order(ByteOrder.LITTLE_ENDIAN);
                buffer2.getConstMappedRange(0, 16, byteBuffer);

                System.out.print("bufferData = [");
                for(int i = 0; i < 16; ++i) {
                    if(i > 0) System.out.print(", ");
                    byte b = byteBuffer.get(i);
                    System.out.print(b);
                }
                System.out.println("]");

                // Then do not forget to unmap the memory
                buffer2.unmap();

                buffer1.release();
                buffer1.dispose();
                buffer2.release();
                buffer2.dispose();
            }
        });
    }

    private String shaderSource =
            "@vertex\n" +
            "fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4f {\n" +
            "    var p = vec2f(0.0, 0.0);\n" +
            "    if (in_vertex_index == 0u) {\n" +
            "        p = vec2f(-0.5, -0.5);\n" +
            "    } else if (in_vertex_index == 1u) {\n" +
            "        p = vec2f(0.5, -0.5);\n" +
            "    } else {\n" +
            "        p = vec2f(0.0, 0.5);\n" +
            "    }\n" +
            "    return vec4f(p, 0.0, 1.0);\n" +
            "}\n" +
            "\n" +
            "@fragment\n" +
            "fn fs_main() -> @location(0) vec4f {\n" +
            "    return vec4f(0.0, 0.4, 1.0, 1.0);\n" +
            "}";
}
