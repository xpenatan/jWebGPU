package com.github.xpenatan.webgpu.backend.teavm;


import com.github.xpenatan.jmultiplatform.core.JMultiplatform;
import com.github.xpenatan.webgpu.WebGPULoader;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;
import org.teavm.jso.browser.AnimationFrameCallback;
import org.teavm.jso.browser.Location;
import org.teavm.jso.browser.Window;
import org.teavm.jso.dom.html.HTMLCanvasElement;
import org.teavm.jso.dom.html.HTMLDocument;

public class TeaVMApp {
    private static String canvas = "webgpuCanvas";
    private static String canvasWGPU = "#" + canvas;

    private WGPUApp wgpu;

    private int wGPUInit = 0;

    public TeaVMApp(ApplicationListener applicationInterface) {

        System.out.println("START");

        Location location = Window.current().getLocation();
        String hostPageBaseURL = location.getFullURL();
        JMultiplatform.getInstance().getMap().put("WEB_SCRIPT_PATH", hostPageBaseURL + "scripts/");

        HTMLDocument document = Window.current().getDocument();
        HTMLCanvasElement canvas = (HTMLCanvasElement)document.createElement("canvas");

        canvas.setId(TeaVMApp.canvas);
        canvas.setWidth(800);
        canvas.setHeight(600);
        document.getBody().appendChild(canvas);

        System.out.println("CANVAS CREATED");

        WebGPULoader.init((isSuccess, e) -> {
            System.out.println("WebGPU Success: " + isSuccess);
            if(isSuccess) {
                wGPUInit = 1;
            }
            else {
                e.printStackTrace();
            }
        });

        Window.requestAnimationFrame(new AnimationFrameCallback() {
            @Override
            public void onAnimationFrame(double timestamp) {
                if(wGPUInit > 0) {
                    if(wGPUInit == 1) {
                        wGPUInit = 2;
                        wgpu = new WGPUApp();
                        wgpu.init();
                    }
                    if(wGPUInit == 2 && wgpu.isReady()) {
                        wGPUInit = 3;
                        createSurface();
                        applicationInterface.create(wgpu);
                    }
                    if(wGPUInit == 3) {
                        applicationInterface.render(wgpu);
                    }
                    wgpu.update();
                }
                Window.requestAnimationFrame(this);
            }
        });
        System.out.println("END");
    }

    private void createSurface() {
        wgpu.surface = wgpu.instance.CreateWebSurface(TeaVMApp.canvasWGPU);
    }
}