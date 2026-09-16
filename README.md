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

Use these repositories and the same version for all jWebGPU dependencies, then choose the block for your platform:

```kotlin
repositories {
    mavenCentral()
    maven("https://central.sonatype.com/repository/maven-snapshots/")
}

val jWebGPUVersion = "-SNAPSHOT"
```

### Shared core module

For code shared between platforms, add `include(":core")` to `settings.gradle.kts`. In `core/build.gradle.kts`, use the repositories and version above with:

```kotlin
plugins {
    `java-library`
}

dependencies {
    compileOnlyApi("com.github.xpenatan.jWebGPU:webgpu-core:$jWebGPUVersion")
}
```

In each platform module, add the shared project alongside its platform dependencies below:

```kotlin
dependencies {
    implementation(project(":core"))
}
```

`compileOnlyApi` exposes the shared API for compilation without packaging its stubs as the native runtime. Each platform supplies its implementation; jParser dependencies are transitive.

### Desktop JNI

```kotlin
dependencies {
    implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-jni:$jWebGPUVersion")
    runtimeOnly("com.github.xpenatan.jWebGPU:webgpu-desktop-jni-wgpu_windows_x64:$jWebGPUVersion")
}
```

### Desktop FFM

Requires JDK 25.

```kotlin
dependencies {
    compileOnly("com.github.xpenatan.jWebGPU:webgpu-core:$jWebGPUVersion")
    implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-ffm:$jWebGPUVersion")
    runtimeOnly("com.github.xpenatan.jWebGPU:webgpu-desktop-ffm-wgpu_windows_x64:$jWebGPUVersion")
}
```

For JNI/FFM, run with `--enable-native-access=ALL-UNNAMED`. Initialize through `JWebGPULoader.init(...)` before using WebGPU. Do not add `webgpu-core` to the runtime classpath; jParser dependencies are transitive.

### Android

Add `google()` to the repositories and set `minSdk = 29` or later.

```kotlin
dependencies {
    implementation("com.github.xpenatan.jWebGPU:webgpu-android-wgpu:$jWebGPUVersion")
}
```

The AAR includes the JNI runtime and native libraries. For Dawn, use `webgpu-android-dawn` instead; include only one backend AAR.

### Browser WebGPU

Use Java 17+ and a TeaVM application.

```kotlin
dependencies {
    implementation("com.github.xpenatan.jWebGPU:webgpu-web:$jWebGPUVersion")
    runtimeOnly("com.github.xpenatan.jWebGPU:webgpu-web_wasm:$jWebGPUVersion")
}
```

Package the JavaScript/WASM resources with the TeaVM output and serve over localhost or HTTPS. The [browser example](https://github.com/xpenatan/jWebGPU-Examples#browser) includes the build and server configuration.

### Desktop TeaVM C

Use a TeaVM C application and the target platform's native compiler.

```kotlin
dependencies {
    implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-c-wgpu_windows_x64:$jWebGPUVersion")
}
```

This artifact brings the Java bindings and C runtime transitively.

For all desktop native artifacts, replace `windows_x64` with `linux_x64`, `mac_x64` or `mac_arm64` to match the target. Replace `wgpu` with `dawn` to use Dawn; load it with `JWebGPULoader.init(JWebGPUBackend.DAWN, listener)`. The browser chooses its own backend.

Complete desktop JNI/FFM, Android and browser applications are in [jWebGPU-Examples](https://github.com/xpenatan/jWebGPU-Examples).

## Building from source

### 1. Install tools and generate bindings

Use **JDK 25** with `JAVA_HOME` set. Install the tools for the target you want to build:

| Target | Required tools |
| --- | --- |
| Windows x64 | Visual Studio C++ Build Tools and Windows SDK; use an x64 Developer terminal |
| Linux x64 | GCC/G++ and binutils (`build-essential` on Ubuntu/Debian) |
| macOS x64 / arm64 | Xcode Command Line Tools (`xcode-select --install`) |
| Android | Android SDK 36 and NDK r27; set `ANDROID_HOME` and `ANDROID_NDK_HOME` to their installed directories |
| Browser | Emscripten SDK 6.0.3; activate it and load `./emsdk_env.ps1` in PowerShell, `emsdk_env.bat` in cmd, or `source ./emsdk_env.sh` on Linux/macOS so `EMSDK` is set |

The Android NDK and Emscripten versions above match the [build workflow](.github/workflows/build_and_upload.yml). Build desktop natives on their corresponding OS; Android CI uses Linux, and web CI uses Linux with Emscripten.

```sh
git clone https://github.com/xpenatan/jWebGPU.git
cd jWebGPU
```

Run from this checkout. Commands below use the Windows wrapper; on Linux/macOS use `./gradlew` instead of `.\gradlew.bat` and run `chmod +x gradlew` if needed.

```powershell
.\gradlew.bat :webgpu:builder:jParser_generate
```

### 2. Download and build your target

Run the downloads before the native build; build tasks do not download dependencies automatically. The desktop and Android commands below select **WGPU**. For **Dawn**, use the same commands with `_wgpu` replaced by `_dawn`. Build either backend or repeat for both.

For every desktop target, download the shared GLFW headers first. Despite its name, this task supplies the headers for Windows, Linux and macOS:

```powershell
.\gradlew.bat :webgpu:download:webgpu_download_glfw_windows
```

Each desktop block builds JNI, FFM and TeaVM C. Omit the tasks for bridges you do not need.

**Windows x64**

```powershell
.\gradlew.bat :webgpu:download:webgpu_download_windows_x86_64_wgpu
.\gradlew.bat :webgpu:builder:jParser_build_windows64_jni_wgpu :webgpu:builder:jParser_build_windows64_ffm_wgpu :webgpu:builder:jParser_build_windows64_teavm_c_wgpu
```

**Linux x64**

```sh
./gradlew :webgpu:download:webgpu_download_linux_x86_64_wgpu
./gradlew :webgpu:builder:jParser_build_linux64_jni_wgpu :webgpu:builder:jParser_build_linux64_ffm_wgpu :webgpu:builder:jParser_build_linux64_teavm_c_wgpu
```

**macOS Intel (x64)**

```sh
./gradlew :webgpu:download:webgpu_download_macos_x86_64_wgpu
./gradlew :webgpu:builder:jParser_build_mac64_jni_wgpu :webgpu:builder:jParser_build_mac64_ffm_wgpu :webgpu:builder:jParser_build_mac64_teavm_c_wgpu
```

**macOS Apple Silicon (arm64)**

```sh
./gradlew :webgpu:download:webgpu_download_macos_aarch64_wgpu
./gradlew :webgpu:builder:jParser_build_macArm_jni_wgpu :webgpu:builder:jParser_build_macArm_ffm_wgpu :webgpu:builder:jParser_build_macArm_teavm_c_wgpu
```

**Android JNI** — download all four ABIs before building the native libraries:

```powershell
.\gradlew.bat :webgpu:download:webgpu_download_android_i686_wgpu :webgpu:download:webgpu_download_android_x86_64_wgpu
.\gradlew.bat :webgpu:download:webgpu_download_android_armv7_wgpu :webgpu:download:webgpu_download_android_aarch64_wgpu
.\gradlew.bat :webgpu:builder:jParser_build_android_jni_wgpu
```

**Browser WebGPU** — uses Emdawnwebgpu, without a WGPU/Dawn variant switch:

```powershell
.\gradlew.bat :webgpu:download:webgpu_download_emdawnwebgpu
.\gradlew.bat :webgpu:builder:jParser_build_web_wasm
```

### 3. Use the build

Native output is in `webgpu/builder/build/c++/libs/`.

To run the local build, clone [jWebGPU-Examples](https://github.com/xpenatan/jWebGPU-Examples) beside this checkout, set `useLocalJWebGPU=true` and `jWebGPUPath=../jWebGPU` in its `gradle.properties`, then follow its platform run commands. `includeBuild` compiles the Java modules and packages the required artifacts automatically; no separate JAR tasks or publishing are needed. Rebuild the bindings and native bridge after source changes.

For LibGDX integration, see [gdx-webgpu](https://github.com/MonstrousSoftware/gdx-webgpu).

[Apache License 2.0](LICENSE) · [Sponsor](https://github.com/sponsors/xpenatan)
