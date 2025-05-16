package com.github.xpenatan.webgpu.demo.triangle;


import com.github.xpenatan.jmultiplatform.core.JMultiplatform;
import org.teavm.jso.browser.AnimationFrameCallback;
import org.teavm.jso.browser.Location;
import org.teavm.jso.browser.Window;
import org.teavm.jso.dom.html.HTMLCanvasElement;
import org.teavm.jso.dom.html.HTMLDocument;

public class Launcher {

    public static void main(String[] args) {
        System.out.println("START");

        Location location = Window.current().getLocation();
        String hostPageBaseURL = location.getFullURL();
        JMultiplatform.getInstance().getMap().put("WEB_SCRIPT_PATH", hostPageBaseURL + "scripts/");

        HTMLDocument document = Window.current().getDocument();
        HTMLCanvasElement canvas = (HTMLCanvasElement)document.createElement("canvas");

        canvas.setId("webgpuCanvas");
        canvas.setWidth(800);
        canvas.setHeight(600);
        document.getBody().appendChild(canvas);

        System.out.println("CANVAS CREATED");

        TriangleApp triangleApp = new TriangleApp();
        triangleApp.create();
        Window.requestAnimationFrame(new AnimationFrameCallback() {
            @Override
            public void onAnimationFrame(double timestamp) {
                triangleApp.render();

                Window.requestAnimationFrame(this);
            }
        });
        System.out.println("END");

    }

}