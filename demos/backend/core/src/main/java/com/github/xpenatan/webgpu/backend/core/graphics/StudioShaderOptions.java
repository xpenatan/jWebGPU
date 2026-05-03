package com.github.xpenatan.webgpu.backend.core.graphics;

public class StudioShaderOptions {
    // Shadow sampling bias.
    public float pbrShadowBiasSlope = 0.00120f;
    public float pbrShadowBiasMin = 0.00028f;
    public float groundShadowBias = 0.00024f;

    // PBR grading and light mix.
    public float pbrExposure = 1.50f;
    public float pbrContrast = 1.20f;
    public float pbrSaturation = 1.08f;
    public float pbrRimStrength = 0.045f;
    public float pbrAmbientFactor = 0.036f;
    public float pbrAmbientTintR = 0.0018f;
    public float pbrAmbientTintG = 0.0022f;
    public float pbrAmbientTintB = 0.0030f;
    public float pbrDiffuseScale = 1.05f;
    public float pbrSpecularScale = 1.28f;
    public float pbrRimTintR = 0.56f;
    public float pbrRimTintG = 0.64f;
    public float pbrRimTintB = 0.74f;

    // Ground grading and light mix.
    public float groundExposure = 1.04f;
    public float groundContrast = 1.16f;
    public float groundSaturation = 1.04f;
    public float groundBaseLight = 0.12f;
    public float groundDiffuseLightScale = 0.63f;
}
