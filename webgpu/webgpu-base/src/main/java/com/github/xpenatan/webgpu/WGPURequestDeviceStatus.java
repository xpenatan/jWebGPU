package com.github.xpenatan.webgpu;

import idl.IDLEnum;
import java.util.HashMap;
import java.util.Map;

public enum WGPURequestDeviceStatus implements IDLEnum<WGPURequestDeviceStatus> {
    CUSTOM(0);

    private int value;

    private WGPURequestDeviceStatus(int value) {
        this.value = value;
    }

    public static final Map<Integer, WGPURequestDeviceStatus> MAP = new HashMap<>();

    static {
        for (WGPURequestDeviceStatus value : values()) {
            if (value != CUSTOM) {
                MAP.put(value.value, value);
            }
        }
    }

    @Override
    public int getValue() {
        return value;
    }

    @Override
    public WGPURequestDeviceStatus setValue(int value) {
        this.value = value;
        return this;
    }

    @Override
    public WGPURequestDeviceStatus getCustom() {
        return CUSTOM;
    }
}
