package com.github.xpenatan.webgpu.backend.core.graphics;

public class LitMeshShaderSource {

    public static String shader(float red, float green, float blue) {
        return "struct Uniforms {\n" +
                "    mvp : mat4x4f,\n" +
                "    model : mat4x4f,\n" +
                "};\n" +
                "\n" +
                "@group(0) @binding(0)\n" +
                "var<uniform> uniforms : Uniforms;\n" +
                "\n" +
                "struct VertexInput {\n" +
                "    @location(0) position : vec3f,\n" +
                "    @location(1) normal : vec3f,\n" +
                "};\n" +
                "\n" +
                "struct VertexOutput {\n" +
                "    @builtin(position) position : vec4f,\n" +
                "    @location(0) worldNormal : vec3f,\n" +
                "};\n" +
                "\n" +
                "@vertex\n" +
                "fn vs_main(input : VertexInput) -> VertexOutput {\n" +
                "    var output : VertexOutput;\n" +
                "    output.position = uniforms.mvp * vec4f(input.position, 1.0);\n" +
                "    output.worldNormal = normalize((uniforms.model * vec4f(input.normal, 0.0)).xyz);\n" +
                "    return output;\n" +
                "}\n" +
                "\n" +
                "@fragment\n" +
                "fn fs_main(input : VertexOutput) -> @location(0) vec4f {\n" +
                "    let lightDir = normalize(vec3f(0.5, 0.8, 0.4));\n" +
                "    let lambert = max(dot(normalize(input.worldNormal), lightDir), 0.0);\n" +
                "    let baseColor = vec3f(" + red + ", " + green + ", " + blue + ");\n" +
                "    let color = baseColor * (0.18 + lambert * 0.82);\n" +
                "    return vec4f(color, 1.0);\n" +
                "}\n";
    }

    public static String studioShader(float red, float green, float blue) {
        return studioPbrShader(red, green, blue, 0.45f, 0.05f);
    }

    public static String studioPbrShader(float red, float green, float blue, float roughness, float metallic) {
        return studioPbrShader(red, green, blue, roughness, metallic, new StudioShaderOptions());
    }

    public static String studioGroundShader(float red, float green, float blue) {
        return studioGroundShader(red, green, blue, new StudioShaderOptions());
    }

    public static String studioShadowDepthShader() {
        return "struct Uniforms {\n" +
                "    mvp : mat4x4f,\n" +
                "    model : mat4x4f,\n" +
                "    lightMvp : mat4x4f,\n" +
                "    cameraPos : vec4f,\n" +
                "    lightDir : vec4f,\n" +
                "};\n" +
                "\n" +
                "@group(0) @binding(0)\n" +
                "var<uniform> uniforms : Uniforms;\n" +
                "\n" +
                "struct VertexInput {\n" +
                "    @location(0) position : vec3f,\n" +
                "    @location(1) normal : vec3f,\n" +
                "};\n" +
                "\n" +
                "struct VertexOutput {\n" +
                "    @builtin(position) position : vec4f,\n" +
                "};\n" +
                "\n" +
                "@vertex\n" +
                "fn vs_main(input : VertexInput) -> VertexOutput {\n" +
                "    var output : VertexOutput;\n" +
                "    output.position = uniforms.lightMvp * vec4f(input.position, 1.0);\n" +
                "    return output;\n" +
                "}\n";
    }

    public static String studioPbrShader(
            float red,
            float green,
            float blue,
            float roughness,
            float metallic,
            float shadowBiasSlope,
            float shadowBiasMin
    ) {
        StudioShaderOptions options = new StudioShaderOptions();
        options.pbrShadowBiasSlope = shadowBiasSlope;
        options.pbrShadowBiasMin = shadowBiasMin;
        return studioPbrShader(red, green, blue, roughness, metallic, options);
    }

    public static String studioPbrShader(
            float red,
            float green,
            float blue,
            float roughness,
            float metallic,
            StudioShaderOptions options
    ) {
        return "struct Uniforms {\n" +
                "    mvp : mat4x4f,\n" +
                "    model : mat4x4f,\n" +
                "    lightMvp : mat4x4f,\n" +
                "    cameraPos : vec4f,\n" +
                "    lightDir : vec4f,\n" +
                "};\n" +
                "\n" +
                "@group(0) @binding(0)\n" +
                "var<uniform> uniforms : Uniforms;\n" +
                "@group(0) @binding(1)\n" +
                "var shadowSampler : sampler_comparison;\n" +
                "@group(0) @binding(2)\n" +
                "var shadowMap : texture_depth_2d;\n" +
                "\n" +
                "struct VertexInput {\n" +
                "    @location(0) position : vec3f,\n" +
                "    @location(1) normal : vec3f,\n" +
                "};\n" +
                "\n" +
                "struct VertexOutput {\n" +
                "    @builtin(position) position : vec4f,\n" +
                "    @location(0) worldPos : vec3f,\n" +
                "    @location(1) worldNormal : vec3f,\n" +
                "    @location(2) shadowPos : vec4f,\n" +
                "};\n" +
                "\n" +
                "@vertex\n" +
                "fn vs_main(input : VertexInput) -> VertexOutput {\n" +
                "    var output : VertexOutput;\n" +
                "    let world = uniforms.model * vec4f(input.position, 1.0);\n" +
                "    output.position = uniforms.mvp * vec4f(input.position, 1.0);\n" +
                "    output.worldPos = world.xyz;\n" +
                "    output.worldNormal = normalize((uniforms.model * vec4f(input.normal, 0.0)).xyz);\n" +
                "    output.shadowPos = uniforms.lightMvp * vec4f(input.position, 1.0);\n" +
                "    return output;\n" +
                "}\n" +
                "\n" +
                "fn sampleShadow(shadowPos : vec4f, normal : vec3f, lightDir : vec3f) -> f32 {\n" +
                "    let ndc = shadowPos.xyz / max(shadowPos.w, 0.00001);\n" +
                "    let uv = vec2f(ndc.x * 0.5 + 0.5, 0.5 - ndc.y * 0.5);\n" +
                "    let texel = vec2f(1.05 / 2048.0, 1.05 / 2048.0);\n" +
                "    let ndotl = max(dot(normal, lightDir), 0.0);\n" +
                "    let bias = max(" + options.pbrShadowBiasSlope + " * (1.0 - ndotl), " + options.pbrShadowBiasMin + ");\n" +
                "    let insideUv = step(0.0, uv.x) * step(uv.x, 1.0) * step(0.0, uv.y) * step(uv.y, 1.0);\n" +
                "    let insideDepth = step(0.0, ndc.z) * step(ndc.z, 1.0);\n" +
                "    let inside = insideUv * insideDepth;\n" +
                "    let safeUv = clamp(uv, vec2f(0.001, 0.001), vec2f(0.999, 0.999));\n" +
                "\n" +
                "    var visibility = 0.0;\n" +
                "    for(var y = -1; y <= 1; y = y + 1) {\n" +
                "        for(var x = -1; x <= 1; x = x + 1) {\n" +
                "            let offset = vec2f(f32(x), f32(y)) * texel;\n" +
                "            visibility = visibility + textureSampleCompare(shadowMap, shadowSampler, clamp(safeUv + offset, vec2f(0.001, 0.001), vec2f(0.999, 0.999)), ndc.z - bias);\n" +
                "        }\n" +
                "    }\n" +
                "    return mix(1.0, visibility / 9.0, inside);\n" +
                "}\n" +
                "\n" +
                "fn tonemapAces(x : vec3f) -> vec3f {\n" +
                "    let a = 2.51;\n" +
                "    let b = 0.03;\n" +
                "    let c = 2.43;\n" +
                "    let d = 0.59;\n" +
                "    let e = 0.14;\n" +
                "    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), vec3f(0.0), vec3f(1.0));\n" +
                "}\n" +
                "\n" +
                "fn applyColorGrade(color : vec3f) -> vec3f {\n" +
                "    let exposure = " + options.pbrExposure + ";\n" +
                "    let contrast = " + options.pbrContrast + ";\n" +
                "    let saturation = " + options.pbrSaturation + ";\n" +
                "    var c = color * exposure;\n" +
                "    c = (c - vec3f(0.18)) * contrast + vec3f(0.18);\n" +
                "    let luma = dot(c, vec3f(0.2126, 0.7152, 0.0722));\n" +
                "    c = mix(vec3f(luma, luma, luma), c, saturation);\n" +
                "    return max(c, vec3f(0.0));\n" +
                "}\n" +
                "\n" +
                "@fragment\n" +
                "fn fs_main(input : VertexOutput) -> @location(0) vec4f {\n" +
                "    let N = normalize(input.worldNormal);\n" +
                "    let L = normalize(uniforms.lightDir.xyz);\n" +
                "    let cameraPos = uniforms.cameraPos.xyz;\n" +
                "    let V = normalize(cameraPos - input.worldPos);\n" +
                "    let H = normalize(L + V);\n" +
                "\n" +
                "    let NdotL = max(dot(N, L), 0.0);\n" +
                "    let NdotV = max(dot(N, V), 0.001);\n" +
                "    let NdotH = max(dot(N, H), 0.001);\n" +
                "    let VdotH = max(dot(V, H), 0.001);\n" +
                "\n" +
                "    let baseColor = vec3f(" + red + ", " + green + ", " + blue + ");\n" +
                "    let F0 = mix(vec3f(0.04, 0.04, 0.04), baseColor, " + metallic + ");\n" +
                "    let alpha = max(" + roughness + " * " + roughness + ", 0.03);\n" +
                "    let alpha2 = alpha * alpha;\n" +
                "\n" +
                "    let denomD = (NdotH * NdotH) * (alpha2 - 1.0) + 1.0;\n" +
                "    let D = alpha2 / max(3.14159265 * denomD * denomD, 0.0001);\n" +
                "\n" +
                "    let k = (alpha + 1.0) * (alpha + 1.0) / 8.0;\n" +
                "    let Gv = NdotV / (NdotV * (1.0 - k) + k);\n" +
                "    let Gl = NdotL / (NdotL * (1.0 - k) + k);\n" +
                "    let G = Gv * Gl;\n" +
                "\n" +
                "    let F = F0 + (vec3f(1.0, 1.0, 1.0) - F0) * pow(1.0 - VdotH, 5.0);\n" +
                "    let specular = (D * G) * F / max(4.0 * NdotV * NdotL, 0.001);\n" +
                "\n" +
                "    let kd = (vec3f(1.0, 1.0, 1.0) - F) * (1.0 - " + metallic + ");\n" +
                "    let diffuse = kd * baseColor / 3.14159265;\n" +
                "\n" +
                "    let shadow = sampleShadow(input.shadowPos, N, L);\n" +
                "    let rim = pow(1.0 - max(dot(N, V), 0.0), 4.0) * " + options.pbrRimStrength + ";\n" +
                "    let ambient = baseColor * " + options.pbrAmbientFactor + " + vec3f(" + options.pbrAmbientTintR + ", " + options.pbrAmbientTintG + ", " + options.pbrAmbientTintB + ");\n" +
                "    let linearLight = (diffuse * " + options.pbrDiffuseScale + " + specular * " + options.pbrSpecularScale + ") * NdotL * shadow + ambient + rim * vec3f(" + options.pbrRimTintR + ", " + options.pbrRimTintG + ", " + options.pbrRimTintB + ");\n" +
                "    let graded = applyColorGrade(linearLight);\n" +
                "    let mapped = tonemapAces(graded);\n" +
                "    let color = pow(mapped, vec3f(1.0 / 2.2));\n" +
                "    return vec4f(color, 1.0);\n" +
                "}\n";
    }

    public static String studioGroundShader(float red, float green, float blue, float shadowBias) {
        StudioShaderOptions options = new StudioShaderOptions();
        options.groundShadowBias = shadowBias;
        return studioGroundShader(red, green, blue, options);
    }

    public static String studioGroundShader(float red, float green, float blue, StudioShaderOptions options) {
        return "struct Uniforms {\n" +
                "    mvp : mat4x4f,\n" +
                "    model : mat4x4f,\n" +
                "    lightMvp : mat4x4f,\n" +
                "    cameraPos : vec4f,\n" +
                "    lightDir : vec4f,\n" +
                "};\n" +
                "\n" +
                "@group(0) @binding(0)\n" +
                "var<uniform> uniforms : Uniforms;\n" +
                "@group(0) @binding(1)\n" +
                "var shadowSampler : sampler_comparison;\n" +
                "@group(0) @binding(2)\n" +
                "var shadowMap : texture_depth_2d;\n" +
                "\n" +
                "struct VertexInput {\n" +
                "    @location(0) position : vec3f,\n" +
                "    @location(1) normal : vec3f,\n" +
                "};\n" +
                "\n" +
                "struct VertexOutput {\n" +
                "    @builtin(position) position : vec4f,\n" +
                "    @location(0) worldPos : vec3f,\n" +
                "    @location(1) worldNormal : vec3f,\n" +
                "    @location(2) shadowPos : vec4f,\n" +
                "};\n" +
                "\n" +
                "@vertex\n" +
                "fn vs_main(input : VertexInput) -> VertexOutput {\n" +
                "    var output : VertexOutput;\n" +
                "    let world = uniforms.model * vec4f(input.position, 1.0);\n" +
                "    output.position = uniforms.mvp * vec4f(input.position, 1.0);\n" +
                "    output.worldPos = world.xyz;\n" +
                "    output.worldNormal = normalize((uniforms.model * vec4f(input.normal, 0.0)).xyz);\n" +
                "    output.shadowPos = uniforms.lightMvp * vec4f(input.position, 1.0);\n" +
                "    return output;\n" +
                "}\n" +
                "\n" +
                "fn sampleShadow(shadowPos : vec4f) -> f32 {\n" +
                "    let ndc = shadowPos.xyz / max(shadowPos.w, 0.00001);\n" +
                "    let uv = vec2f(ndc.x * 0.5 + 0.5, 0.5 - ndc.y * 0.5);\n" +
                "    let texel = vec2f(1.0 / 2048.0, 1.0 / 2048.0);\n" +
                "    let bias = " + options.groundShadowBias + ";\n" +
                "    let insideUv = step(0.0, uv.x) * step(uv.x, 1.0) * step(0.0, uv.y) * step(uv.y, 1.0);\n" +
                "    let insideDepth = step(0.0, ndc.z) * step(ndc.z, 1.0);\n" +
                "    let inside = insideUv * insideDepth;\n" +
                "    let safeUv = clamp(uv, vec2f(0.001, 0.001), vec2f(0.999, 0.999));\n" +
                "\n" +
                "    var visibility = 0.0;\n" +
                "    for(var y = -1; y <= 1; y = y + 1) {\n" +
                "        for(var x = -1; x <= 1; x = x + 1) {\n" +
                "            let offset = vec2f(f32(x), f32(y)) * texel;\n" +
                "            visibility = visibility + textureSampleCompare(shadowMap, shadowSampler, clamp(safeUv + offset, vec2f(0.001, 0.001), vec2f(0.999, 0.999)), ndc.z - bias);\n" +
                "        }\n" +
                "    }\n" +
                "    return mix(1.0, visibility / 9.0, inside);\n" +
                "}\n" +
                "\n" +
                "fn tonemapAces(x : vec3f) -> vec3f {\n" +
                "    let a = 2.51;\n" +
                "    let b = 0.03;\n" +
                "    let c = 2.43;\n" +
                "    let d = 0.59;\n" +
                "    let e = 0.14;\n" +
                "    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), vec3f(0.0), vec3f(1.0));\n" +
                "}\n" +
                "\n" +
                "fn applyColorGradeGround(color : vec3f) -> vec3f {\n" +
                "    let exposure = " + options.groundExposure + ";\n" +
                "    let contrast = " + options.groundContrast + ";\n" +
                "    let saturation = " + options.groundSaturation + ";\n" +
                "    var c = color * exposure;\n" +
                "    c = (c - vec3f(0.18)) * contrast + vec3f(0.18);\n" +
                "    let luma = dot(c, vec3f(0.2126, 0.7152, 0.0722));\n" +
                "    c = mix(vec3f(luma, luma, luma), c, saturation);\n" +
                "    return max(c, vec3f(0.0));\n" +
                "}\n" +
                "\n" +
                "@fragment\n" +
                "fn fs_main(input : VertexOutput) -> @location(0) vec4f {\n" +
                "    let N = normalize(input.worldNormal);\n" +
                "    let L = normalize(uniforms.lightDir.xyz);\n" +
                "    let base = vec3f(" + red + ", " + green + ", " + blue + ");\n" +
                "    let diffuse = max(dot(N, L), 0.0);\n" +
                "    let shadow = sampleShadow(input.shadowPos);\n" +
                "    let lit = " + options.groundBaseLight + " + diffuse * " + options.groundDiffuseLightScale + " * shadow;\n" +
                "    let linearColor = base * lit;\n" +
                "    let graded = applyColorGradeGround(linearColor);\n" +
                "    let mapped = tonemapAces(graded);\n" +
                "    let color = pow(mapped, vec3f(1.0 / 2.2));\n" +
                "    return vec4f(color, 1.0);\n" +
                "}\n";
    }

    public static String studioDebugClipShader(float red, float green, float blue) {
        return "struct VertexInput {\n" +
                "    @location(0) position : vec3f,\n" +
                "    @location(1) normal : vec3f,\n" +
                "};\n" +
                "\n" +
                "struct VertexOutput {\n" +
                "    @builtin(position) position : vec4f,\n" +
                "};\n" +
                "\n" +
                "@vertex\n" +
                "fn vs_main(input : VertexInput) -> VertexOutput {\n" +
                "    var output : VertexOutput;\n" +
                "    output.position = vec4f(input.position * vec3f(0.8, 0.8, 0.4), 1.0);\n" +
                "    return output;\n" +
                "}\n" +
                "\n" +
                "@fragment\n" +
                "fn fs_main() -> @location(0) vec4f {\n" +
                "    return vec4f(" + red + ", " + green + ", " + blue + ", 1.0);\n" +
                "}\n";
    }
}


