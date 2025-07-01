package com.github.xpenatan.webgpu.backend.android;

import android.app.Activity;
import android.os.Bundle;
import android.view.Choreographer;
import android.view.Display;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import com.github.xpenatan.webgpu.WGPUAndroidWindow;
import com.github.xpenatan.webgpu.JWebGPULoader;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.backend.core.WGPUApp;

public class AndroidApplication extends Activity implements Choreographer.FrameCallback {

    private int wGPUInit = 0;
    private WGPUApp wgpu;
    private ApplicationListener applicationListener;
    private Surface surface;
    private WGPUAndroidWindow androidWindow;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setupFullScreen();

        wgpu = new WGPUApp();

        JWebGPULoader.init((isSuccess, e) -> {
            System.out.println("WebGPU Init Success: " + isSuccess);
            if (isSuccess) {
                wGPUInit = 1;
            } else {
                e.printStackTrace();
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
                    androidWindow = new WGPUAndroidWindow();
                    androidWindow.initLogcat();
                    WindowManager windowManager = (WindowManager)getApplicationContext().getSystemService(WINDOW_SERVICE);
                    Display display = windowManager.getDefaultDisplay();
                    wgpu.width = display.getWidth();
                    wgpu.height = display.getHeight();
                    wgpu.init();
                }
                else if (wGPUInit == 2 && wgpu.isReady()) {
                    createSurface(surface);
                    applicationListener.create(wgpu);
                    wGPUInit = 3;
                }
            }
            wgpu.update();
        }
        Choreographer.getInstance().postFrameCallback(this);
    }

    protected void initialize(ApplicationListener applicationListener) {
        SurfaceView surfaceView = new SurfaceView(this);
        initialize(applicationListener, surfaceView, surfaceView);
    }

    protected void initialize(ApplicationListener applicationListener, SurfaceView surfaceView, View root) {
        this.applicationListener = applicationListener;
        setupSurface(surfaceView);
        setContentView(root);
    }

    private void createSurface(Surface surface) {
        androidWindow.createAndroidSurface(surface);
        wgpu.surface = wgpu.instance.createAndroidSurface(androidWindow);
    }

    private void setupFullScreen() {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        // Enable immersive fullscreen
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
        );
        decorView.setOnApplyWindowInsetsListener((v, insets) -> {
            // Remove insets so content goes edge-to-edge
            return insets.consumeSystemWindowInsets();
        });

        // Allow drawing into the display cutout area (notch/hole-punch) for API 28+
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.P) {
            WindowManager.LayoutParams lp = getWindow().getAttributes();
            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            getWindow().setAttributes(lp);
        }
    }

    private void setupSurface(SurfaceView surfaceView) {
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
}
