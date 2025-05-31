package com.github.xpenatan.webgpu.backend.android;

import android.app.Activity;
import android.os.Bundle;
import android.view.Choreographer;
import android.view.Display;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;
import com.github.xpenatan.jparser.loader.JParserLibraryLoaderListener;
import com.github.xpenatan.webgpu.JAndroidWindow;
import com.github.xpenatan.webgpu.WebGPULoader;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;

public class AndroidApplication extends Activity implements Choreographer.FrameCallback {

    private int wGPUInit = 0;
    private WGPUApp wgpu;
    private ApplicationListener applicationListener;
    private Surface surface;
    private JAndroidWindow androidWindow;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        SurfaceView surfaceView = new SurfaceView(this);
        setContentView(surfaceView);

        WebGPULoader.init((isSuccess, e) -> {
            System.out.println("WebGPU Success: " + isSuccess);
            if (isSuccess) {
                wGPUInit = 1;
            } else {
                e.printStackTrace();
            }
        });

        surfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {

            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                surface = holder.getSurface();
                Choreographer.getInstance().postFrameCallback(AndroidApplication.this);
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
            }
        });
    }

    @Override
    public void doFrame(long frameTimeNanos) {
        if(surface != null) {
            if (wGPUInit == 3) {
                applicationListener.render(wgpu);
            } else if (wGPUInit > 0) {
                if (wGPUInit == 1) {
                    wGPUInit = 2;
                    androidWindow = new JAndroidWindow();
                    androidWindow.InitLogcat();
                    WindowManager windowManager = (WindowManager)getApplicationContext().getSystemService(WINDOW_SERVICE);
                    Display display = windowManager.getDefaultDisplay();
                    wgpu = new WGPUApp();
                    wgpu.width = display.getWidth();
                    wgpu.height = display.getHeight();
                    wgpu.init();
                }
                if (wGPUInit == 2 && wgpu.isReady()) {
                    createSurface(surface);
                    applicationListener.create(wgpu);
                    wGPUInit = 3;
                }
                wgpu.update();
            }
        }
        Choreographer.getInstance().postFrameCallback(this);
    }

    protected void initialize(ApplicationListener applicationListener) {
        this.applicationListener = applicationListener;
    }

    private void createSurface(Surface surface) {
        androidWindow.createAndroidSurface(surface);
        wgpu.surface = wgpu.instance.CreateAndroidSurface(androidWindow);
    }
}
