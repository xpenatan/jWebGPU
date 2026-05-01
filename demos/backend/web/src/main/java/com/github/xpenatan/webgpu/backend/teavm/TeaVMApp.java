package com.github.xpenatan.webgpu.backend.teavm;


import com.github.xpenatan.jParser.loader.JParserLibraryLoaderListener;
import com.github.xpenatan.jmultiplatform.core.JMultiplatform;
import com.github.xpenatan.jparser.idl.IDLLoader;
import com.github.xpenatan.webgpu.JWebGPULoader;
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

    private int width = 800;
    private int height = 600;

    private ApplicationListener applicationInterface;

    private boolean loop = true;

    public TeaVMApp(ApplicationListener applicationInterface) {
        this.applicationInterface = applicationInterface;
        System.out.println("START");

        Location location = Window.current().getLocation();
        String hostPageBaseURL = location.getFullURL();
        JMultiplatform.getInstance().getMap().put("WEB_SCRIPT_PATH", hostPageBaseURL + "scripts/");

        HTMLDocument document = Window.current().getDocument();
        HTMLCanvasElement canvas = (HTMLCanvasElement)document.createElement("canvas");


        canvas.setId(TeaVMApp.canvas);
        canvas.setWidth(width);
        canvas.setHeight(height);
        document.getBody().appendChild(canvas);

        System.out.println("CANVAS CREATED");

        JWebGPULoader.init((isSuccess, e) -> {
            System.out.println("WebGPU Init Success: " + isSuccess);
            if(isSuccess) {
                wGPUInit = 1;
            }
            else {
                e.printStackTrace();
            }
        });

        wgpu = new WGPUApp();

        Window.requestAnimationFrame(new AnimationFrameCallback() {
            @Override
            public void onAnimationFrame(double timestamp) {
                try {
                    tick();
                }
                catch(Throwable t) {
                    t.printStackTrace();
                    loop = false;
                }
                if(loop) {
                    Window.requestAnimationFrame(this);
                }
            }
        });
        System.out.println("END");
    }

    private void tick() {
        if(wGPUInit == 3) {
            applicationInterface.render(wgpu);
        }
        else if(wGPUInit > 0) {
            if(wGPUInit == 1) {
                wGPUInit = 2;
                wgpu.width = width;
                wgpu.height = height;
                wgpu.init();
            }
            else if(wGPUInit == 2 && wgpu.isReady()) {
                wGPUInit = 3;
                createSurface();
                applicationInterface.create(wgpu);
            }
        }
        wgpu.update();
    }

    private void createSurface() {
        wgpu.surface = wgpu.instance.createWebSurface(TeaVMApp.canvasWGPU);
    }
}