package com.github.xpenatan.webgpu.demo.app;

import com.github.xpenatan.webgpu.JWebGPUBackend;
import com.github.xpenatan.webgpu.JWebGPULoader;
import com.github.xpenatan.webgpu.WGPUChainedStruct;
import com.github.xpenatan.webgpu.WGPUDepthStencilState;
import com.github.xpenatan.webgpu.WGPUPipelineLayout;
import com.github.xpenatan.webgpu.WGPURenderPassDepthStencilAttachment;
import com.github.xpenatan.webgpu.WGPURenderPassTimestampWrites;
import com.github.xpenatan.webgpu.WGPUVectorCommandBuffer;
import com.github.xpenatan.webgpu.WGPUVectorConstantEntry;
import com.github.xpenatan.webgpu.WGPUVectorTextureFormat;
import com.github.xpenatan.webgpu.WGPUBlendState;
import com.github.xpenatan.webgpu.WGPUColorTargetState;
import com.github.xpenatan.webgpu.WGPUCommandBuffer;
import com.github.xpenatan.webgpu.WGPUCommandBufferDescriptor;
import com.github.xpenatan.webgpu.WGPUCommandEncoder;
import com.github.xpenatan.webgpu.WGPUCommandEncoderDescriptor;
import com.github.xpenatan.webgpu.WGPUFragmentState;
import com.github.xpenatan.webgpu.WGPUPlatformType;
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
import com.github.xpenatan.webgpu.WGPUVectorColorTargetState;
import com.github.xpenatan.webgpu.WGPUVectorRenderPassColorAttachment;
import com.github.xpenatan.webgpu.WGPU;
import com.github.xpenatan.webgpu.WGPUBlendFactor;
import com.github.xpenatan.webgpu.WGPUBlendOperation;
import com.github.xpenatan.webgpu.WGPUColorWriteMask;
import com.github.xpenatan.webgpu.WGPUCompositeAlphaMode;
import com.github.xpenatan.webgpu.WGPUCullMode;
import com.github.xpenatan.webgpu.WGPUFrontFace;
import com.github.xpenatan.webgpu.WGPUIndexFormat;
import com.github.xpenatan.webgpu.WGPULoadOp;
import com.github.xpenatan.webgpu.WGPUPresentMode;
import com.github.xpenatan.webgpu.WGPUPrimitiveTopology;
import com.github.xpenatan.webgpu.WGPUSType;
import com.github.xpenatan.webgpu.WGPUStoreOp;
import com.github.xpenatan.webgpu.WGPUTextureAspect;
import com.github.xpenatan.webgpu.WGPUTextureFormat;
import com.github.xpenatan.webgpu.WGPUTextureUsage;
import com.github.xpenatan.webgpu.WGPUTextureViewDimension;
import com.github.xpenatan.webgpu.WGPUVectorVertexBufferLayout;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;

public class HelloTriangle implements ApplicationListener {

    private WGPURenderPipeline pipeline;
    private WGPUTextureFormat surfaceFormat;

    private WGPUCommandEncoder encoder;
    private WGPURenderPassEncoder renderPass;
    private WGPUCommandBuffer command;

    private float r = 0.0f;
    private float g = 0.0f;
    private float b = 1.0f;

    @Override
    public void create(WGPUApp wgpu) {
        encoder = new WGPUCommandEncoder();
        renderPass = new WGPURenderPassEncoder();
        command = new WGPUCommandBuffer();

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
            initPipeline(wgpu);
        }
        else {
            System.out.println("Surface not created");
        }
    }

    @Override
    public void render(WGPUApp wgpu) {
        WGPUTextureView textureView = GetNextSurfaceTextureView(wgpu);

        WGPUCommandEncoderDescriptor encoderDesc = WGPUCommandEncoderDescriptor.obtain();
        encoderDesc.setLabel("My command encoder");
        wgpu.device.createCommandEncoder(encoderDesc, encoder);

        WGPURenderPassColorAttachment renderPassColorAttachment = WGPURenderPassColorAttachment.obtain();
        renderPassColorAttachment.setView(textureView);
        renderPassColorAttachment.setResolveTarget(WGPUTextureView.NULL);
        renderPassColorAttachment.setLoadOp(WGPULoadOp.Clear);
        renderPassColorAttachment.setStoreOp(WGPUStoreOp.Store);
        renderPassColorAttachment.getClearValue().setColor(r, g, b, 1.0f);

        WGPUVectorRenderPassColorAttachment colorAttachmentVector = WGPUVectorRenderPassColorAttachment.obtain();
        colorAttachmentVector.push_back(renderPassColorAttachment);

        WGPURenderPassDescriptor renderPassDesc  = WGPURenderPassDescriptor.obtain();
        renderPassDesc.setColorAttachments(colorAttachmentVector);
        renderPassDesc.setDepthStencilAttachment(WGPURenderPassDepthStencilAttachment.NULL);
        renderPassDesc.setTimestampWrites(WGPURenderPassTimestampWrites.NULL);
        encoder.beginRenderPass(renderPassDesc, renderPass);

        renderPass.setPipeline(pipeline);
        renderPass.draw(3, 1, 0, 0);

        renderPass.end();
        renderPass.release();

        WGPUCommandBufferDescriptor cmdBufferDescriptor = WGPUCommandBufferDescriptor.obtain();
        cmdBufferDescriptor.setNextInChain(WGPUChainedStruct.NULL);
        cmdBufferDescriptor.setLabel("Command buffer");
        encoder.finish(cmdBufferDescriptor, command);
        encoder.release();

        WGPUVectorCommandBuffer commands = WGPUVectorCommandBuffer.obtain();
        commands.push_back(command);
        wgpu.queue.submit(commands);
        command.release();

        textureView.release();

        if(WGPU.getPlatformType() != WGPUPlatformType.WGPU_Web) {
            wgpu.surface.present();
        }
    }

    @Override
    public void dispose() {

    }

    public void setColor(float r, float g, float b) {
        this.r = r;
        this.g = g;
        this.b = b;
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
        if(JWebGPULoader.getBackend() == JWebGPUBackend.DAWN) {
            textureOut.release();
        }
        return textureViewOut;
    }

    private void initSwapChain(WGPUApp wgpu) {
        boolean vsyncEnabled = true;
        WGPUSurfaceConfiguration config = WGPUSurfaceConfiguration.obtain();
        config.setWidth(wgpu.width);
        config.setHeight(wgpu.height);
        config.setFormat(surfaceFormat);
        config.setViewFormats(WGPUVectorTextureFormat.NULL);
        config.setUsage(WGPUTextureUsage.RenderAttachment);
        config.setDevice(wgpu.device);
        config.setPresentMode(vsyncEnabled ? WGPUPresentMode.Fifo : WGPUPresentMode.Immediate);
        config.setAlphaMode(WGPUCompositeAlphaMode.Auto);
        wgpu.surface.configure(config);
    }

    public void initPipeline(WGPUApp wgpu) {
        WGPUShaderModule shaderModule = makeShaderModule(wgpu);

        WGPURenderPipelineDescriptor pipelineDesc = WGPURenderPipelineDescriptor.obtain();
        pipelineDesc.setLabel("my pipeline");

        pipelineDesc.getVertex().setBuffers(WGPUVectorVertexBufferLayout.NULL);
        pipelineDesc.getVertex().setModule(shaderModule);
        pipelineDesc.getVertex().setEntryPoint("vs_main");
        pipelineDesc.getVertex().setConstants(WGPUVectorConstantEntry.NULL);

        pipelineDesc.getPrimitive().setTopology(WGPUPrimitiveTopology.TriangleList);
        pipelineDesc.getPrimitive().setStripIndexFormat(WGPUIndexFormat.Undefined);
        pipelineDesc.getPrimitive().setFrontFace(WGPUFrontFace.CCW);
        pipelineDesc.getPrimitive().setCullMode(WGPUCullMode.None);

        WGPUFragmentState fragmentState = WGPUFragmentState.obtain();
        fragmentState.setNextInChain(WGPUChainedStruct.NULL);
        fragmentState.setModule(shaderModule);
        fragmentState.setEntryPoint("fs_main");
        fragmentState.setConstants(WGPUVectorConstantEntry.NULL);

        // blending
        WGPUBlendState blendState = WGPUBlendState.obtain();
        blendState.getColor().setSrcFactor(WGPUBlendFactor.SrcAlpha);
        blendState.getColor().setDstFactor(WGPUBlendFactor.OneMinusSrcAlpha);
        blendState.getColor().setOperation(WGPUBlendOperation.Add);
        blendState.getAlpha().setSrcFactor(WGPUBlendFactor.One);
        blendState.getAlpha().setDstFactor(WGPUBlendFactor.Zero);
        blendState.getAlpha().setOperation(WGPUBlendOperation.Add);

        WGPUColorTargetState colorTarget = WGPUColorTargetState.obtain();
        colorTarget.setFormat(surfaceFormat); // match output surface
        colorTarget.setBlend(blendState);
        colorTarget.setWriteMask(WGPUColorWriteMask.All);

        WGPUVectorColorTargetState colorStateTargets = WGPUVectorColorTargetState.obtain();
        colorStateTargets.push_back(colorTarget);
        fragmentState.setTargets(colorStateTargets);

        pipelineDesc.setFragment(fragmentState);
        pipelineDesc.setDepthStencil(WGPUDepthStencilState.NULL); // no depth or stencil buffer
        pipelineDesc.getMultisample().setCount(1);
        pipelineDesc.getMultisample().setMask(-1);
        pipelineDesc.getMultisample().setAlphaToCoverageEnabled(false);
        pipelineDesc.setLayout(WGPUPipelineLayout.NULL);

        pipeline = new WGPURenderPipeline();
        wgpu.device.createRenderPipeline(pipelineDesc, pipeline);

        shaderModule.release();

        System.out.println("RenderPipeline created");
    }

    public WGPUShaderModule makeShaderModule(WGPUApp wgpu) {
        String shaderSource = readShaderSource();

        WGPUShaderModuleDescriptor shaderDesc = WGPUShaderModuleDescriptor.obtain();
        shaderDesc.setLabel("triangle shader");

        WGPUShaderSourceWGSL shaderCodeDesc = WGPUShaderSourceWGSL.obtain();
        shaderCodeDesc.getChain().setNext(WGPUChainedStruct.NULL);
        shaderCodeDesc.getChain().setSType(WGPUSType.ShaderSourceWGSL);
        shaderCodeDesc.setCode(shaderSource);

        shaderDesc.setNextInChain(shaderCodeDesc.getChain());
        WGPUShaderModule shaderModule = WGPUShaderModule.obtain();
        wgpu.device.createShaderModule(shaderDesc, shaderModule);
        return shaderModule;
    }

    private String readShaderSource () {
        String triangleShader =
                "// triangleShader.wgsl\n" +
                "\n" +
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
        return triangleShader;
    }
}
