package com.github.xpenatan.webgpu.demo.app;

import com.github.xpenatan.webgpu.JBlendState;
import com.github.xpenatan.webgpu.JColorTargetState;
import com.github.xpenatan.webgpu.JCommandBuffer;
import com.github.xpenatan.webgpu.JCommandBufferDescriptor;
import com.github.xpenatan.webgpu.JCommandEncoder;
import com.github.xpenatan.webgpu.JCommandEncoderDescriptor;
import com.github.xpenatan.webgpu.JFragmentState;
import com.github.xpenatan.webgpu.JPlatformType;
import com.github.xpenatan.webgpu.JRenderPassColorAttachment;
import com.github.xpenatan.webgpu.JRenderPassDescriptor;
import com.github.xpenatan.webgpu.JRenderPassEncoder;
import com.github.xpenatan.webgpu.JRenderPipeline;
import com.github.xpenatan.webgpu.JRenderPipelineDescriptor;
import com.github.xpenatan.webgpu.JShaderModule;
import com.github.xpenatan.webgpu.JShaderModuleDescriptor;
import com.github.xpenatan.webgpu.JShaderSourceWGSL;
import com.github.xpenatan.webgpu.JSurfaceCapabilities;
import com.github.xpenatan.webgpu.JSurfaceConfiguration;
import com.github.xpenatan.webgpu.JSurfaceTexture;
import com.github.xpenatan.webgpu.JTexture;
import com.github.xpenatan.webgpu.JTextureView;
import com.github.xpenatan.webgpu.JTextureViewDescriptor;
import com.github.xpenatan.webgpu.JVectorColorTargetState;
import com.github.xpenatan.webgpu.JVectorRenderPassColorAttachment;
import com.github.xpenatan.webgpu.JWGPU;
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
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;

public class WebGPUApp implements ApplicationListener {

    private WGPUTextureFormat surfaceFormat;

    private JCommandEncoder encoder;
    private JRenderPassEncoder renderPass;
    private JCommandBuffer command;

    private float r = 0.0f;
    private float g = 0.0f;
    private float b = 1.0f;

    @Override
    public void create(WGPUApp wgpu) {
        encoder = new JCommandEncoder();
        renderPass = new JRenderPassEncoder();
        command = new JCommandBuffer();

        if(wgpu.surface != null) {
            System.out.println("Surface created");
            JSurfaceCapabilities surfaceCapabilities = new JSurfaceCapabilities();
            wgpu.surface.GetCapabilities(wgpu.adapter, surfaceCapabilities);
            surfaceFormat = surfaceCapabilities.GetFormats(0);
            System.out.println("surfaceFormat: " + surfaceFormat);
            initSwapChain(wgpu);

            // Release the adapter only after it has been fully utilized
            wgpu.adapter.Release();
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
        JTextureView textureView = JTextureView.Obtain();
        GetNextSurfaceTextureView(wgpu, textureView);

        JCommandEncoderDescriptor encoderDesc = JCommandEncoderDescriptor.Obtain();
        encoderDesc.SetLabel("My command encoder");
        wgpu.device.CreateCommandEncoder(encoderDesc, encoder);

        JRenderPassColorAttachment renderPassColorAttachment = JRenderPassColorAttachment.Obtain();
        renderPassColorAttachment.SetView(textureView);
        renderPassColorAttachment.SetResolveTarget(null);
        renderPassColorAttachment.SetLoadOp(WGPULoadOp.Clear);
        renderPassColorAttachment.SetStoreOp(WGPUStoreOp.Store);
        renderPassColorAttachment.GetClearValue().SetColor(r, g, b, 1.0f);

        JVectorRenderPassColorAttachment colorAttachmentVector = JVectorRenderPassColorAttachment.Obtain();
        colorAttachmentVector.push_back(renderPassColorAttachment);

        JRenderPassDescriptor renderPassDesc  = JRenderPassDescriptor.Obtain();
        renderPassDesc.SetColorAttachments(colorAttachmentVector);
        renderPassDesc.SetDepthStencilAttachment(null);
        renderPassDesc.SetTimestampWrites(null);
        encoder.BeginRenderPass(renderPassDesc, renderPass);

        renderPass.SetPipeline(wgpu.renderPipeline);
        renderPass.Draw(3, 1, 0, 0);

        renderPass.End();
        renderPass.Release();

        JCommandBufferDescriptor cmdBufferDescriptor = JCommandBufferDescriptor.Obtain();
        cmdBufferDescriptor.SetNextInChain(null);
        cmdBufferDescriptor.SetLabel("Command buffer");
        encoder.Finish(cmdBufferDescriptor, command);
        encoder.Release();

        wgpu.queue.Submit(1, command);
        command.Release();

        textureView.Release();

        if(JWGPU.GetPlatformType() != JPlatformType.WGPU_Web) {
            wgpu.surface.Present();
        }
    }

    public void setColor(float r, float g, float b) {
        this.r = r;
        this.g = g;
        this.b = b;
    }

    private void GetNextSurfaceTextureView(WGPUApp wgpu, JTextureView textureViewOut) {
        JSurfaceTexture surfaceTextureOut = JSurfaceTexture.Obtain();
        wgpu.surface.GetCurrentTexture(surfaceTextureOut);
        JTexture textureOut = JTexture.Obtain();
        surfaceTextureOut.GetTexture(textureOut);
        WGPUTextureFormat textureFormat = textureOut.GetFormat();
        JTextureViewDescriptor viewDescriptor = JTextureViewDescriptor.Obtain();
        viewDescriptor.SetLabel("Surface texture view");
        viewDescriptor.SetFormat(textureFormat);
        viewDescriptor.SetDimension(WGPUTextureViewDimension._2D);
        viewDescriptor.SetBaseMipLevel(0);
        viewDescriptor.SetMipLevelCount(1);
        viewDescriptor.SetBaseArrayLayer(0);
        viewDescriptor.SetArrayLayerCount(1);
        viewDescriptor.SetAspect(WGPUTextureAspect.All);
        textureOut.CreateView(viewDescriptor, textureViewOut);
        textureOut.Release();
    }

    private void initSwapChain(WGPUApp wgpu) {
        boolean vsyncEnabled = true;
        JSurfaceConfiguration config = JSurfaceConfiguration.Obtain();
        config.SetWidth(wgpu.width);
        config.SetHeight(wgpu.height);
        config.SetFormat(surfaceFormat);
        config.SetViewFormats(null);
        config.SetUsage(WGPUTextureUsage.RenderAttachment);
        config.SetDevice(wgpu.device);
        config.SetPresentMode(vsyncEnabled ? WGPUPresentMode.Fifo : WGPUPresentMode.Immediate);
        config.SetAlphaMode(WGPUCompositeAlphaMode.Auto);
        wgpu.surface.Configure(config);
    }

    public void initPipeline(WGPUApp wgpu) {
        JShaderModule shaderModule = makeShaderModule(wgpu);

        JRenderPipelineDescriptor pipelineDesc = JRenderPipelineDescriptor.Obtain();
        pipelineDesc.SetLabel("my pipeline");

        pipelineDesc.GetVertex().SetBuffers(null);
        pipelineDesc.GetVertex().SetModule(shaderModule);
        pipelineDesc.GetVertex().SetEntryPoint("vs_main");
        pipelineDesc.GetVertex().SetConstants(null);

        pipelineDesc.GetPrimitive().SetTopology(WGPUPrimitiveTopology.TriangleList);
        pipelineDesc.GetPrimitive().SetStripIndexFormat(WGPUIndexFormat.Undefined);
        pipelineDesc.GetPrimitive().SetFrontFace(WGPUFrontFace.CCW);
        pipelineDesc.GetPrimitive().SetCullMode(WGPUCullMode.None);

        JFragmentState fragmentState = JFragmentState.Obtain();
        fragmentState.SetNextInChain(null);
        fragmentState.SetModule(shaderModule);
        fragmentState.SetEntryPoint("fs_main");
        fragmentState.SetConstants(null);

        // blending
        JBlendState blendState = JBlendState.Obtain();
        blendState.GetColor().SetSrcFactor(WGPUBlendFactor.SrcAlpha);
        blendState.GetColor().SetDstFactor(WGPUBlendFactor.OneMinusSrcAlpha);
        blendState.GetColor().SetOperation(WGPUBlendOperation.Add);
        blendState.GetAlpha().SetSrcFactor(WGPUBlendFactor.One);
        blendState.GetAlpha().SetDstFactor(WGPUBlendFactor.Zero);
        blendState.GetAlpha().SetOperation(WGPUBlendOperation.Add);

        JColorTargetState colorTarget = JColorTargetState.Obtain();
        colorTarget.SetFormat(surfaceFormat); // match output surface
        colorTarget.SetBlend(blendState);
        colorTarget.SetWriteMask(WGPUColorWriteMask.All);

        JVectorColorTargetState colorStateTargets = JVectorColorTargetState.Obtain();
        colorStateTargets.push_back(colorTarget);
        fragmentState.SetTargets(colorStateTargets);

        pipelineDesc.SetFragment(fragmentState);
        pipelineDesc.SetDepthStencil(null); // no depth or stencil buffer
        pipelineDesc.GetMultisample().SetCount(1);
        pipelineDesc.GetMultisample().SetMask(-1);
        pipelineDesc.GetMultisample().SetAlphaToCoverageEnabled(0);
        pipelineDesc.SetLayout(null);

        JRenderPipeline renderPipeline = new JRenderPipeline();
        wgpu.device.CreateRenderPipeline(pipelineDesc, renderPipeline);
        wgpu.renderPipeline = renderPipeline;

        shaderModule.Release();

        System.out.println("RenderPipeline created");
    }

    public JShaderModule makeShaderModule(WGPUApp wgpu) {
        String shaderSource = readShaderSource();

        JShaderModuleDescriptor shaderDesc = JShaderModuleDescriptor.Obtain();
        shaderDesc.SetLabel("triangle shader");

        JShaderSourceWGSL shaderCodeDesc = JShaderSourceWGSL.Obtain();
        shaderCodeDesc.SetNext(null);
        shaderCodeDesc.SetSType(WGPUSType.ShaderSourceWGSL);
        shaderCodeDesc.SetCode(shaderSource);

        shaderDesc.SetNextInChain(shaderCodeDesc.GetChain());
        JShaderModule shaderModule = JShaderModule.Obtain();
        wgpu.device.CreateShaderModule(shaderDesc, shaderModule);
        return shaderModule;
    }

    private String readShaderSource () {

        return "// triangleShader.wgsl\n" + "\n" + "@vertex\n"
                + "fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4f {\n"
                + "    var p = vec2f(0.0, 0.0);\n" + "    if (in_vertex_index == 0u) {\n" + "        p = vec2f(-0.5, -0.5);\n"
                + "    } else if (in_vertex_index == 1u) {\n" + "        p = vec2f(0.5, -0.5);\n" + "    } else {\n"
                + "        p = vec2f(0.0, 0.5);\n" + "    }\n" + "    return vec4f(p, 0.0, 1.0);\n" + "}\n" + "\n" + "@fragment\n"
                + "fn fs_main() -> @location(0) vec4f {\n" + "    return vec4f(0.0, 0.4, 1.0, 1.0);\n" + "}";
    }
}