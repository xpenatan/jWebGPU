package com.github.xpenatan.webgpu.demo.app;

import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.SurfaceView;
import com.github.xpenatan.webgpu.backend.android.AndroidApplication;
import com.github.xpenatan.webgpu.backend.core.ApplicationListener;
import com.github.xpenatan.webgpu.demo.R;
import com.github.xpenatan.webgpu.demo.app.registry.DemoFactory;
import com.github.xpenatan.webgpu.demo.app.registry.DemoId;
import com.github.xpenatan.webgpu.demo.databinding.MainLayoutBinding;

public class MainAndroid extends AndroidApplication {

    private static final DemoId[] TOGGLE_ORDER = {
            DemoId.STUDIO,
            DemoId.HELLO_TRIANGLE,
            DemoId.FIRST_VERTEX_ATTRIBUTE,
            DemoId.PLAYING_WITH_BUFFERS,
            DemoId.CUBE,
            DemoId.CONE,
            DemoId.SPHERE,
            DemoId.TORUS
    };

    private MainLayoutBinding binding;

    private ApplicationListener webgpuApp;
    private DemoId currentDemoId;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        String selectedDemo = null;
        if(getIntent() != null) {
            selectedDemo = getIntent().getStringExtra("demo");
            if((selectedDemo == null || selectedDemo.trim().isEmpty()) && getIntent().getData() != null) {
                selectedDemo = getIntent().getData().getQueryParameter("demo");
            }
        }
        currentDemoId = DemoFactory.resolveDemoId(selectedDemo);
        webgpuApp = DemoFactory.create(currentDemoId);
        binding = MainLayoutBinding.inflate(LayoutInflater.from(this));
        SurfaceView surfaceView = binding.viewSurface;
        initialize(webgpuApp, surfaceView, binding.getRoot());
        setupButtons();
    }

    private void setupButtons() {
        DemoId previousDemo = previousDemoId(currentDemoId);
        DemoId nextDemo = nextDemoId(currentDemoId);
        binding.btnLeft.setOnClickListener(v -> launchDemo(previousDemo));
        binding.txtCurrentTest.setText(getString(R.string.btn_current_test, getDisplayName(currentDemoId)));
        binding.btnRight.setOnClickListener(v -> launchDemo(nextDemo));
    }

    private DemoId previousDemoId(DemoId current) {
        int index = toggleIndex(current);
        if(index < 0) {
            return DemoFactory.defaultDemo();
        }
        int previous = (index - 1 + TOGGLE_ORDER.length) % TOGGLE_ORDER.length;
        return TOGGLE_ORDER[previous];
    }

    private DemoId nextDemoId(DemoId current) {
        int index = toggleIndex(current);
        if(index < 0) {
            return DemoFactory.defaultDemo();
        }
        int next = (index + 1) % TOGGLE_ORDER.length;
        return TOGGLE_ORDER[next];
    }

    private int toggleIndex(DemoId current) {
        for(int i = 0; i < TOGGLE_ORDER.length; i++) {
            if(TOGGLE_ORDER[i] == current) {
                return i;
            }
        }
        return -1;
    }

    private void launchDemo(DemoId demoId) {
        Intent intent = new Intent(this, MainAndroid.class);
        intent.putExtra("demo", toDemoToken(demoId));
        startActivity(intent);
        finish();
    }

    private String toDemoToken(DemoId demoId) {
        switch(demoId) {
            case CUBE:
                return "cube";
            case CONE:
                return "cone";
            case SPHERE:
                return "sphere";
            case TORUS:
                return "torus";
            case STUDIO:
                return "studio";
            case HELLO_TRIANGLE:
                return "hello-triangle";
            case FIRST_VERTEX_ATTRIBUTE:
                return "first-vertex-attribute";
            case PLAYING_WITH_BUFFERS:
                return "playing-with-buffers";
            default:
                return "studio";
        }
    }

    private String getDisplayName(DemoId demoId) {
        switch(demoId) {
            case CUBE:
                return "Cube";
            case CONE:
                return "Cone";
            case SPHERE:
                return "Sphere";
            case TORUS:
                return "Torus";
            case STUDIO:
                return "Studio";
            case HELLO_TRIANGLE:
                return "Hello Triangle";
            case FIRST_VERTEX_ATTRIBUTE:
                return "First Vertex Attribute";
            case PLAYING_WITH_BUFFERS:
                return "Playing With Buffers";
            default:
                return "Studio";
        }
    }
}
