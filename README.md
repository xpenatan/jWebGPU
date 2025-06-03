# jWebGPU

![Build](https://github.com/xpenatan/jWebGPU/actions/workflows/snapshot.yml/badge.svg)

[![Maven Central Version](https://img.shields.io/maven-central/v/com.github.xpenatan.jWebGPU/webgpu-core)](https://central.sonatype.com/artifact/com.github.xpenatan.jWebGPU/webgpu-core)
[![Static Badge](https://img.shields.io/badge/snapshot---SNAPSHOT-red)](https://central.sonatype.com/service/rest/repository/browse/maven-snapshots/com/github/xpenatan/jWebGPU/)

A Java-based WebGPU binding that works across Desktop, Mobile, and Web platforms.

## Build steps

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
// Windows requires Visual studio 2022 C++
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