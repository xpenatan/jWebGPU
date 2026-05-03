package com.github.xpenatan.webgpu.demo.app;

import com.github.xpenatan.webgpu.backend.desktop.GLFWApp;
import com.github.xpenatan.webgpu.demo.app.registry.DemoFactory;
import com.github.xpenatan.webgpu.demo.app.registry.DemoId;

public class Main {
    public static void main(String[] args) {
        String selectedArg = args != null && args.length > 0 ? args[0] : null;
        DemoId demoId = DemoFactory.resolveDemoId(selectedArg);
        new GLFWApp(DemoFactory.create(demoId));
    }
}
