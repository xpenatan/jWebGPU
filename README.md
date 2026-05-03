# jWebGPU

![Build](https://github.com/xpenatan/jWebGPU/actions/workflows/snapshot.yml/badge.svg)
[![Maven Central Version](https://img.shields.io/maven-central/v/com.github.xpenatan.jWebGPU/webgpu-core)](https://central.sonatype.com/namespace/com.github.xpenatan.jWebGPU)
[![Snapshot](https://img.shields.io/badge/snapshot---SNAPSHOT-red)](https://central.sonatype.com/service/rest/repository/browse/maven-snapshots/com/github/xpenatan/jWebGPU/)

jWebGPU is a Java binding stack for WebGPU APIs across desktop, Android, and web targets.

<img width="790" height="592" alt="image" src="https://github.com/user-attachments/assets/21317836-0c92-4457-9354-22b494cfc438" />

- Browser example: [Studio demo](https://xpenatan.github.io/jWebGPU/index.html?demo=studio)
- Android app: [Google Play](https://play.google.com/store/apps/details?id=com.github.xpenatan.webgpu.demo)

## What this project is for

- Build and ship WebGPU-capable Java apps across multiple runtimes.
- Reuse the same API model while swapping backend/runtime packaging per platform.
- Generate and package bindings from `webgpu-base` + IDL/native sources.

It keeps one Java-facing API shape and generates target-specific glue/code for:
- JNI desktop runtime
- FFM desktop runtime
- TeaVM/WebAssembly web runtime
- Android JNI runtime

## Table of Contents

- [Quick Start](#quick-start)
- [Project Structure](#project-structure)
- [Build Commands](#build-commands)
  - [JNI](#jni)
  - [FFM](#ffm)
  - [TeaVM Web](#teavm-web)
  - [Android JNI](#android-jni)
- [Run Examples](#run-examples)
- [CI Build Matrix](#ci-build-matrix)
- [Development Notes](#development-notes)
- [Installation](#installation)
- [License](#license)

## Quick Start

Use `./gradlew` on macOS/Linux and `gradlew.bat` on Windows.

Windows examples in this README use `gradlew.bat`.

Before local builds, ensure your environment is set up for your target:
- JNI/FFM Windows native builds: Visual Studio C++ tools.
- TeaVM web builds: emscripten.
- Android builds: Android NDK.

## Project Structure

Canonical modules are defined in `settings.gradle.kts`.

### Core library modules

- `:webgpu:webgpu-base` - hand-authored Java templates and directive blocks.
- `:webgpu:webgpu-build` - generation and native build driver (`WGPUBuild`).
- `:webgpu:webgpu-core` - generated Java API layer.
- `:webgpu:webgpu-jni` - JNI runtime packaging.
- `:webgpu:webgpu-ffm` - FFM runtime packaging.
- `:webgpu:webgpu-web` - TeaVM/Web runtime packaging.
- `:webgpu:webgpu-android` - Android runtime packaging.
- `:webgpu:webgpu-download` - native dependency download tasks.

### Example app modules

- `:demos:app:desktop-jni`
- `:demos:app:desktop-ffm`
- `:demos:app:web`
- `:demos:app:android`

## Build Commands

### JNI

#### Windows (wgpu + dawn)

```powershell
.\gradlew.bat :webgpu:webgpu-download:webgpu_download_glfw_windows
.\gradlew.bat :webgpu:webgpu-download:webgpu_download_windows_x86_64_wgpu
.\gradlew.bat :webgpu:webgpu-download:webgpu_download_windows_x86_64_dawn
.\gradlew.bat :webgpu:webgpu-build:webgpu_build_project_windows64_wgpu_jni
.\gradlew.bat :webgpu:webgpu-build:webgpu_build_project_windows64_dawn_jni
```

#### Linux (wgpu)

```bash
./gradlew :webgpu:webgpu-download:webgpu_download_linux_x86_64_wgpu
./gradlew :webgpu:webgpu-download:webgpu_download_glfw_windows
./gradlew :webgpu:webgpu-build:webgpu_build_project_linux64_wgpu_jni
```

#### macOS (wgpu x86_64 + arm64)

```bash
./gradlew :webgpu:webgpu-download:webgpu_download_macos_x86_64_wgpu
./gradlew :webgpu:webgpu-download:webgpu_download_macos_aarch64_wgpu
./gradlew :webgpu:webgpu-download:webgpu_download_glfw_windows
./gradlew :webgpu:webgpu-build:webgpu_build_project_mac64_wgpu_jni
./gradlew :webgpu:webgpu-build:webgpu_build_project_macArm_wgpu_jni
```

### FFM

#### Windows (wgpu + dawn)

```powershell
.\gradlew.bat :webgpu:webgpu-build:webgpu_build_project_windows64_wgpu_ffm
.\gradlew.bat :webgpu:webgpu-build:webgpu_build_project_windows64_dawn_ffm
```

#### Linux (wgpu)

```bash
./gradlew :webgpu:webgpu-build:webgpu_build_project_linux64_wgpu_ffm
```

#### macOS (wgpu x86_64 + arm64)

```bash
./gradlew :webgpu:webgpu-build:webgpu_build_project_mac64_wgpu_ffm
./gradlew :webgpu:webgpu-build:webgpu_build_project_macArm_wgpu_ffm
```

### TeaVM Web

```powershell
.\gradlew.bat :webgpu:webgpu-download:webgpu_download_emdawnwebgpu
.\gradlew.bat :webgpu:webgpu-build:webgpu_build_project_web_dawn_wasm
```

### Android JNI

```powershell
.\gradlew.bat :webgpu:webgpu-download:webgpu_download_android_x86_64_wgpu
.\gradlew.bat :webgpu:webgpu-download:webgpu_download_android_i686_wgpu
.\gradlew.bat :webgpu:webgpu-download:webgpu_download_android_armv7_wgpu
.\gradlew.bat :webgpu:webgpu-download:webgpu_download_android_aarch64_wgpu
.\gradlew.bat :webgpu:webgpu-build:webgpu_build_project_android_wgpu_jni
```

Generated native outputs are placed under `webgpu/webgpu-build/build/c++/libs/`.

## Run Examples

Desktop and web demo tasks are explicitly declared in demo module Gradle files.

```powershell
.\gradlew.bat :demos:app:desktop-jni:webgpu_demo_app_run_desktop_jni
.\gradlew.bat :demos:app:desktop-ffm:webgpu_demo_app_run_desktop_ffm
.\gradlew.bat :demos:app:web:webgpu_demo_app_run_web
```

Android demo install (provided by Android plugin tasking):

```powershell
.\gradlew.bat :demos:app:android:installDebug
```

## CI Build Matrix

From `.github/workflows/build_and_upload.yml`:

- Windows: JNI + FFM (`wgpu` and `dawn`).
- Linux: JNI + FFM (`wgpu`).
- macOS: JNI + FFM (`wgpu`).
- TeaVM: web build with `webgpu_build_project_web_dawn_wasm`.
- Android: JNI build with `webgpu_build_project_android_wgpu_jni`.

## Development Notes

- Primary edit locations:
  - `webgpu/webgpu-base/src/main/java/**`
  - `webgpu/webgpu-build/src/main/cpp/**`
- Avoid manual edits in generated targets:
  - `webgpu/webgpu-core/src/main/java/**`
  - `webgpu/webgpu-ffm/src/main/java/**`
  - `webgpu/webgpu-web/src/main/java/**`

## Installation

Use group ID `com.github.xpenatan.jWebGPU`.

```kotlin
dependencies {
    implementation("com.github.xpenatan.jWebGPU:webgpu-core:<version>")

    // Choose one runtime target
    implementation("com.github.xpenatan.jWebGPU:webgpu-jni:<version>")
    // implementation("com.github.xpenatan.jWebGPU:webgpu-ffm:<version>")
    // implementation("com.github.xpenatan.jWebGPU:webgpu-web:<version>")
    // implementation("com.github.xpenatan.jWebGPU:webgpu-android:<version>")
}
```

## Support

If you find this project valuable and want to fuel its continued growth, please consider [sponsoring](https://github.com/sponsors/xpenatan).

## License

jWebGPU is licensed under the [Apache License 2.0](LICENSE).
