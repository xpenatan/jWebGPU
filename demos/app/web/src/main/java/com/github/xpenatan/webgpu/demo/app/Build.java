package com.github.xpenatan.webgpu.demo.app;

import java.io.File;
import java.util.Arrays;
import java.util.List;
import org.teavm.diagnostics.Problem;
import org.teavm.tooling.TeaVMTool;
import org.teavm.tooling.TeaVMToolException;
import org.teavm.tooling.TeaVMTargetType;
import org.teavm.vm.TeaVMOptimizationLevel;

public class Build {

    public static void main(String[] args) throws TeaVMToolException {
        TeaVMTool tool = new TeaVMTool();
        tool.setObfuscated(false);
        tool.setOptimizationLevel(TeaVMOptimizationLevel.SIMPLE);
        tool.setClassLoader(Build.class.getClassLoader());
        tool.setIncremental(false);
        tool.setTargetType(TeaVMTargetType.JAVASCRIPT);
        tool.setMainClass(Launcher.class.getName());
        File target = new File("build/dist/webapp");
        System.out.println("TeaVM target dir: " + target.getAbsolutePath());
        tool.setTargetDirectory(target);
        tool.setTargetFileName("app.js");
        tool.setDebugInformationGenerated(false);
        tool.generate();
        List<Problem> severeProblems = tool.getProblemProvider().getSevereProblems();
        if(!severeProblems.isEmpty()) {
            for(Problem problem : severeProblems) {
                System.err.println("TeaVM severe: " + problem.getText() + " params=" + Arrays.toString(problem.getParams()));
            }
            throw new TeaVMToolException("TeaVM generated severe problems: " + severeProblems.size());
        }
        File output = new File(target, "app.js");
        System.out.println("TeaVM app.js exists=" + output.exists() + " length=" + output.length());
    }
}