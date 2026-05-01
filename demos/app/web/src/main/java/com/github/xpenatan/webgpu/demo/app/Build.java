package com.github.xpenatan.webgpu.demo.app;

import java.io.File;
import org.teavm.tooling.TeaVMTool;
import org.teavm.tooling.TeaVMToolException;
import org.teavm.vm.TeaVMOptimizationLevel;

public class Build {

    public static void main(String[] args) throws TeaVMToolException {
        TeaVMTool tool = new TeaVMTool();
        tool.setObfuscated(false);
        tool.setOptimizationLevel(TeaVMOptimizationLevel.SIMPLE);
        tool.setMainClass(Launcher.class.getName());
        File target = new File("build/dist/webapp");
        tool.setTargetDirectory(target);
        tool.setTargetFileName("app.js");
        tool.setDebugInformationGenerated(false);
        tool.generate();
    }
}