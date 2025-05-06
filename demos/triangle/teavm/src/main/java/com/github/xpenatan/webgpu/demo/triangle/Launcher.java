package com.github.xpenatan.webgpu.demo.triangle;

import com.badlogic.gdx.ApplicationAdapter;
import com.github.xpenatan.gdx.backends.teavm.TeaApplication;
import com.github.xpenatan.gdx.backends.teavm.TeaApplicationConfiguration;

public class Launcher {

    public static void main(String[] args) {
        TeaApplicationConfiguration config = new TeaApplicationConfiguration("canvas");
        config.useDebugGL = false;
        config.showDownloadLogs = true;
        config.width = 0;
        config.height = 0;
        config.useGL30 = true;
        new TeaApplication(new ApplicationAdapter() {
            TriangleApp triangleApp;
            @Override
            public void create() {
                triangleApp = new TriangleApp();
                triangleApp.create();
            }

            @Override
            public void render() {
                triangleApp.render();
            }
        }, config);
    }
}