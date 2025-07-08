# jWebGPU

![Build](https://github.com/xpenatan/jWebGPU/actions/workflows/snapshot.yml/badge.svg)

[![Maven Central Version](https://img.shields.io/maven-central/v/com.github.xpenatan.jWebGPU/webgpu-core)](https://central.sonatype.com/artifact/com.github.xpenatan.jWebGPU/webgpu-core)
[![Static Badge](https://img.shields.io/badge/snapshot---SNAPSHOT-red)](https://central.sonatype.com/service/rest/repository/browse/maven-snapshots/com/github/xpenatan/jWebGPU/)

A Java-based WebGPU binding that works across Desktop, Mobile, and Web platforms.

## Demo
[Android](https://play.google.com/store/apps/details?id=com.github.xpenatan.webgpu.demo)

### Platform status:

| Emscripten | Windows | Linux |       Mac       |                   Android                   | iOS  |
|:----------:|:-------:|:-----:|:---------------:|:-------------------------------------------:|:----:|
|     ✅      |    ✅    |   ✅   |        ✅        |                      ✅                      |  ❌   |
|    wasm    |   x86_64   |  x86_64  | x86_64<br>arm64 |  x86<br>x86_64<br>arm64_v8a<br>armeabi_v7a  | TODO |


## Naming Conventions
To help you navigate the jWebGPU library, here are the naming conventions used for classes, enums, and methods:

- **Custom Classes**: Start with `WGPU` (e.g., `WGPU`, `WGPUAndroidWindow`).
- **WebGPU Classes**: Start with `WGPU` (e.g., `WGPUAdapter`, `WGPUDevice`).
- **WebGPU Enums**: Start with `WGPU` (e.g., `WGPUTextureFormat`, `WGPUPowerPreference`).
- **Methods Requiring Arrays**: Start with `WGPUVector` (e.g., `WGPUVectorTextureFormat`, `WGPUVectorInt`).

## Memory Management and Static Obtain Methods
Most classes in jWebGPU, except for opaque pointer classes, provide a static `obtain` method. 
This method returns a global, reusable object to minimize allocations in both Java and native memory, eliminating the need for disposal. 
Developers should use the `obtain` method whenever possible to reuse these global instances. However, if a unique class instance with a separate memory address is required, developers must create a new instance explicitly.

## Build Steps

Change `LibExt.exampleUseRepoLibs` to false. Set to true only if you want to try the demo without building.

1) **Download Dependencies**  
   Grab all the dependency files. They'll be saved in `webgpu/webgpu-wgpu/build`.
```
./gradlew :webgpu:webgpu-wgpu:download_glfw_windows
./gradlew :webgpu:webgpu-wgpu:download_emdawnwebgpu
./gradlew :webgpu:webgpu-wgpu:download_windows_x86_64
./gradlew :webgpu:webgpu-wgpu:download_windows_aarch64
./gradlew :webgpu:webgpu-wgpu:download_macos_x86_64
./gradlew :webgpu:webgpu-wgpu:download_macos_aarch64
./gradlew :webgpu:webgpu-wgpu:download_linux_x86
./gradlew :webgpu:webgpu-wgpu:download_linux_aarch64
./gradlew :webgpu:webgpu-wgpu:download_android_x86
./gradlew :webgpu:webgpu-wgpu:download_android_i686
./gradlew :webgpu:webgpu-wgpu:download_android_armv7
./gradlew :webgpu:webgpu-wgpu:download_android_aarch64
```

2) **Build Bindings for All Platforms**  
   Make sure your PC is set up to build for each platform.  
   The native files will end up in `webgpu/webgpu-build/build/C++/libs`.
```
// Windows requires Visual Studio 2022 C++
./gradlew :webgpu:webgpu-build:wgpu_build_project_windows64

// TeaVM requires Emscripten
./gradlew :webgpu:webgpu-build:wgpu_build_project_teavm

// Android requires NDK
./gradlew :webgpu:webgpu-build:wgpu_build_project_android
```

3) **Run or Install the Demo App**  
   Try out the demo with these commands:
```
./gradlew :demos:app:desktop:webgpu_demo_app_run_desktop
./gradlew :demos:app:teavm:webgpu_demo_app_run_teavm
./gradlew :demos:app:android:installDebug
```