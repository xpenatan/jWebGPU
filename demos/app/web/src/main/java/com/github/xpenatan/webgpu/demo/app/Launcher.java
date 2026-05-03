package com.github.xpenatan.webgpu.demo.app;

import com.github.xpenatan.webgpu.backend.teavm.TeaVMApp;
import com.github.xpenatan.webgpu.demo.app.registry.DemoFactory;
import com.github.xpenatan.webgpu.demo.app.registry.DemoId;
import org.teavm.jso.browser.Window;

public class Launcher {
    public static void main(String[] args) {
        String demoName = getQueryParameter("demo");
        if((demoName == null || demoName.trim().isEmpty()) && args != null && args.length > 0) {
            demoName = args[0];
        }
        DemoId demoId = DemoFactory.resolveDemoId(demoName);
        new TeaVMApp(DemoFactory.create(demoId));
    }

    private static String getQueryParameter(String key) {
        String url = Window.current().getLocation().getFullURL();
        int queryStart = url.indexOf('?');
        if(queryStart < 0 || queryStart + 1 >= url.length()) {
            return null;
        }

        int hashStart = url.indexOf('#', queryStart + 1);
        String query = hashStart >= 0 ? url.substring(queryStart + 1, hashStart) : url.substring(queryStart + 1);
        String[] pairs = query.split("&");
        for(String pair : pairs) {
            int eq = pair.indexOf('=');
            if(eq <= 0) {
                continue;
            }
            String name = pair.substring(0, eq);
            if(name.equalsIgnoreCase(key)) {
                return pair.substring(eq + 1);
            }
        }
        return null;
    }
}