package com.github.xpenatan.webgpu.demo.triangle;

import com.github.xpenatan.webgpu.JRenderPipelineDescriptor;
import com.github.xpenatan.webgpu.JShaderModule;
import com.github.xpenatan.webgpu.JShaderModuleDescriptor;
import com.github.xpenatan.webgpu.JShaderSourceWGSL;
import com.github.xpenatan.webgpu.JSurfaceCapabilities;
import com.github.xpenatan.webgpu.JSurfaceConfiguration;
import com.github.xpenatan.webgpu.WGPUCompositeAlphaMode;
import com.github.xpenatan.webgpu.WGPUCullMode;
import com.github.xpenatan.webgpu.WGPUFrontFace;
import com.github.xpenatan.webgpu.WGPUIndexFormat;
import com.github.xpenatan.webgpu.WGPUPresentMode;
import com.github.xpenatan.webgpu.WGPUPrimitiveTopology;
import com.github.xpenatan.webgpu.WGPUSType;
import com.github.xpenatan.webgpu.WGPUTextureFormat;
import com.github.xpenatan.webgpu.WGPUTextureUsage;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;

public class TriangleApp implements ApplicationListener {

    private WGPUTextureFormat surfaceFormat;

    @Override
    public void create(WGPUApp wgpu) {
        if(wgpu.surface != null) {
            System.out.println("Surface created");

            JSurfaceCapabilities surfaceCapabilities = new JSurfaceCapabilities();
            wgpu.surface.GetCapabilities(wgpu.adapter, surfaceCapabilities);
            surfaceFormat = surfaceCapabilities.GetFormats(0);
            System.out.println("surfaceFormat: " + surfaceFormat);
            initSwapChain(wgpu);
            initPipeline(wgpu);
        }
        else {
            System.out.println("Surface not created");
        }
    }

    @Override
    public void render(WGPUApp wgpu) {

    }

    private void initSwapChain(WGPUApp wgpu) {
        boolean vsyncEnabled = true;
        JSurfaceConfiguration config = new JSurfaceConfiguration();
//            config.SetNextInChain();
        config.SetWidth(wgpu.width);
        config.SetHeight(wgpu.height);
        config.SetFormat(surfaceFormat);
        config.SetViewFormatCount(0);
//            config.SetViewFormats(JavaWebGPU.createNullPointer());
        config.SetUsage(WGPUTextureUsage.RenderAttachment);
        config.SetDevice(wgpu.device);
        config.SetPresentMode(vsyncEnabled ? WGPUPresentMode.Fifo : WGPUPresentMode.Immediate);
        config.SetAlphaMode(WGPUCompositeAlphaMode.Auto);
        wgpu.surface.Configure(config);
    }

    public void initPipeline(WGPUApp wgpu) {
        JShaderModule shaderModule = makeShaderModule(wgpu);

        JRenderPipelineDescriptor pipelineDesc = new JRenderPipelineDescriptor();
        pipelineDesc.SetLabel("my pipeline");

        pipelineDesc.GetVertex().SetBufferCount(0); // no vertex buffer, because we define it in the shader
//        pipelineDesc.GetVertex().SetBuffers();

        pipelineDesc.GetVertex().SetModule(shaderModule);
        pipelineDesc.GetVertex().SetEntryPoint("vs_main");
        pipelineDesc.GetVertex().SetConstantCount(0);
//        pipelineDesc.GetVertex().SetConstants();

        pipelineDesc.GetPrimitive().SetTopology(WGPUPrimitiveTopology.TriangleList);
        pipelineDesc.GetPrimitive().SetStripIndexFormat(WGPUIndexFormat.Undefined);
        pipelineDesc.GetPrimitive().SetFrontFace(WGPUFrontFace.CCW);
        pipelineDesc.GetPrimitive().SetCullMode(WGPUCullMode.None);

    }

    public JShaderModule makeShaderModule(WGPUApp wgpu) {
        String shaderSource = readShaderSource();

        JShaderModuleDescriptor shaderDesc = new JShaderModuleDescriptor();
        shaderDesc.SetLabel("triangle shader");

        JShaderSourceWGSL shaderCodeDesc = new JShaderSourceWGSL();
        shaderCodeDesc.GetChain().SetNext(null);
        shaderCodeDesc.GetChain().SetSType(WGPUSType.ShaderSourceWGSL);
        shaderCodeDesc.SetCode(shaderSource);

        shaderDesc.SetNextInChain(shaderCodeDesc);
        JShaderModule shaderModule = wgpu.device.CreateShaderModule(shaderDesc);
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