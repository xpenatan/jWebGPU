# jWebGPU

![Build](https://github.com/xpenatan/jWebGPU/actions/workflows/snapshot.yml/badge.svg)
[![Maven Central Version](https://img.shields.io/maven-central/v/com.github.xpenatan.jWebGPU/webgpu-core)](https://central.sonatype.com/namespace/com.github.xpenatan.jWebGPU)
[![Snapshot](https://img.shields.io/badge/snapshot---SNAPSHOT-red)](https://central.sonatype.com/service/rest/repository/browse/maven-snapshots/com/github/xpenatan/jWebGPU/)

Java bindings for WebGPU on desktop (JNI, FFM and TeaVM C), Android and the browser.

<img width="790" height="592" alt="jWebGPU Studio demo" src="https://github.com/user-attachments/assets/21317836-0c92-4457-9354-22b494cfc438" />

## Get started

- **Run the examples:** [jWebGPU-Examples](https://github.com/xpenatan/jWebGPU-Examples) includes desktop, Android and browser setup.
- **Try it:** [browser demo](https://xpenatan.github.io/jWebGPU/index.html?demo=studio) or [Android app](https://play.google.com/store/apps/details?id=com.github.xpenatan.webgpu.demo).

## Add to your project

For desktop JNI on Windows x64, use JDK 25 and these Gradle dependencies:

```kotlin
repositories {
    mavenCentral()
    maven("https://central.sonatype.com/repository/maven-snapshots/")
}

val jWebGPUVersion = "-SNAPSHOT"

dependencies {
    implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-jni:$jWebGPUVersion")
    runtimeOnly("com.github.xpenatan.jWebGPU:webgpu-desktop-jni-wgpu_windows_x64:$jWebGPUVersion")
}
```

Run with `--enable-native-access=ALL-UNNAMED` and initialize through `JWebGPULoader.init(...)` before using WebGPU. Keep all jWebGPU artifacts on the same version. jParser dependencies are transitive; do not add `webgpu-core` to the JNI/FFM runtime classpath.

For other platforms and complete initialization examples, use [jWebGPU-Examples](https://github.com/xpenatan/jWebGPU-Examples).

## Building from source

Use JDK 25 and the target platform's native toolchain. For Windows JNI/WGPU, install MSVC and run from this repository:

```powershell
.\gradlew.bat :webgpu:download:webgpu_download_glfw_windows :webgpu:download:webgpu_download_windows_x86_64_wgpu
.\gradlew.bat :webgpu:builder:jParser_generate :webgpu:builder:jParser_build_windows64_jni_wgpu
```

List other targets with `:webgpu:builder:tasks --group jParser --all` and their prerequisite downloads with `:webgpu:download:tasks --all`. Run the matching downloads before building. Applications using published artifacts do not need to build the bindings.

For LibGDX integration, see [gdx-webgpu](https://github.com/MonstrousSoftware/gdx-webgpu).

[Apache License 2.0](LICENSE) · [Sponsor](https://github.com/sponsors/xpenatan)
