# jWebGPU

![Build](https://github.com/xpenatan/jWebGPU/actions/workflows/snapshot.yml/badge.svg)
[![Maven Central Version](https://img.shields.io/maven-central/v/com.github.xpenatan.jWebGPU/webgpu-core)](https://central.sonatype.com/namespace/com.github.xpenatan.jWebGPU)
[![Snapshot](https://img.shields.io/badge/snapshot---SNAPSHOT-red)](https://central.sonatype.com/service/rest/repository/browse/maven-snapshots/com/github/xpenatan/jWebGPU/)

jWebGPU is a Java WebGPU binding stack for desktop, mobile, and web targets.

It provides a consistent Java API shape with backend/runtime-specific packaging for JVM desktop, Android, and TeaVM/WebAssembly workflows.

## Highlights

- Multi-target architecture: desktop (JNI/FFM), Android, and TeaVM/WebAssembly.
- Backend support for `wgpu-native` and `Dawn` (platform-dependent availability).
- Generator-driven workflow with reusable base templates and generated target modules.
- Published artifacts under `com.github.xpenatan.jWebGPU` on Maven Central and snapshots.

## Table of Contents

- [Demo](#demo)
- [Platform Support](#platform-support)
- [Installation](#installation)
- [Requirements](#requirements)
- [Project Modules](#project-modules)
- [Quick Start](#quick-start)
- [Build Pipeline](#build-pipeline)
- [Run Demos](#run-demos)
- [API Design Notes](#api-design-notes)

## Demo

- Android app: [Google Play](https://play.google.com/store/apps/details?id=com.github.xpenatan.webgpu.demo)

## Platform Support

| Backend     | Emscripten | Windows | Linux  |       Mac       |                  Android                  | iOS  |
|:------------|:----------:|:-------:|:------:|:---------------:|:-----------------------------------------:|:----:|
| wgpu-native |     ❌      |    ✅    |   ✅    |        ✅        |                     ✅                     |  ❌   |
|             |     -      | x86_64  | x86_64 | x86_64<br>arm64 | x86<br>x86_64<br>arm64_v8a<br>armeabi_v7a | TODO |
| Dawn        |     ✅      |    ✅    |   ❌    |        ❌        |                     ❌                     |  ❌   |
|             |    wasm    | x86_64  |   -    |        -        |                     -                     |  -   |

## Installation

Use the group ID `com.github.xpenatan.jWebGPU`.

### Gradle (Kotlin DSL)

```kotlin
dependencies {
	// Base API
	implementation("com.github.xpenatan.jWebGPU:webgpu-core:<version>")

	// Choose one runtime target
	implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-jni:<version>")
	// implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-ffm:<version>")
	// implementation("com.github.xpenatan.jWebGPU:webgpu-teavm:<version>")
	// implementation("com.github.xpenatan.jWebGPU:webgpu-android:<version>")
}
```

Use the Maven Central badge for the latest release version. Snapshot builds are published with `-SNAPSHOT`.

## Requirements

- `Java 8+` for `webgpu-core`, `webgpu-desktop-jni`, and `webgpu-android` modules.
- `Java 11+` for `webgpu-teavm`.
- `Java 24+` for `webgpu-desktop-ffm`.
- Visual Studio 2022 C++ tools for Windows native builds.
- Emscripten for TeaVM/Dawn web output tasks.
- Android NDK for Android native build tasks.

## Project Modules

### Library modules

- `webgpu/webgpu-core`: generated Java WebGPU API core.
- `webgpu/webgpu-desktop-jni`: desktop JNI packaging with native classifiers.
- `webgpu/webgpu-desktop-ffm`: desktop FFM packaging with native classifiers.
- `webgpu/webgpu-teavm`: TeaVM/WebAssembly bridge artifacts.
- `webgpu/webgpu-android`: Android library packaging with JNI libs.

### Build and generation modules

- `webgpu/webgpu-base`: base Java templates and bridge directives.
- `webgpu/webgpu-build`: generation/build entry module and native build tasks.
- `webgpu/webgpu-download`: native dependency downloader tasks.
- `webgpu/webgpu-dawn`: experimental Dawn source build module.

### Demo modules

- `demos/app/*`: runnable demo applications per target.
- `demos/backend/*`: backend wiring modules used by demos.

## Quick Start

Before building locally, set `LibExt.exampleUseRepoLibs` to `false`.

Use `./gradlew` on macOS/Linux and `gradlew.bat` on Windows.

## Build Pipeline

### 1) Download Dependencies

Dependency artifacts are downloaded into `webgpu/webgpu-download/build`.

```bash
./gradlew :webgpu:webgpu-download:webgpu_download_glfw_windows
./gradlew :webgpu:webgpu-download:webgpu_download_emdawnwebgpu
./gradlew :webgpu:webgpu-download:webgpu_download_windows_x86_64_wgpu
./gradlew :webgpu:webgpu-download:webgpu_download_windows_x86_64_dawn
./gradlew :webgpu:webgpu-download:webgpu_download_windows_aarch64_wgpu
./gradlew :webgpu:webgpu-download:webgpu_download_macos_x86_64_wgpu
./gradlew :webgpu:webgpu-download:webgpu_download_macos_aarch64_wgpu
./gradlew :webgpu:webgpu-download:webgpu_download_linux_x86_64_wgpu
./gradlew :webgpu:webgpu-download:webgpu_download_linux_aarch64_wgpu
./gradlew :webgpu:webgpu-download:webgpu_download_android_x86_64_wgpu
./gradlew :webgpu:webgpu-download:webgpu_download_android_i686_wgpu
./gradlew :webgpu:webgpu-download:webgpu_download_android_armv7_wgpu
./gradlew :webgpu:webgpu-download:webgpu_download_android_aarch64_wgpu
```

### 2) Build Bindings

Generated native outputs are produced in `webgpu/webgpu-build/build/C++/libs`.

```bash
# Windows requires Visual Studio 2022 C++
./gradlew :webgpu:webgpu-build:webgpu_build_project_windows64_wgpu_jni
./gradlew :webgpu:webgpu-build:webgpu_build_project_windows64_wgpu_ffm
./gradlew :webgpu:webgpu-build:webgpu_build_project_windows64_dawn_jni
./gradlew :webgpu:webgpu-build:webgpu_build_project_windows64_dawn_ffm

# TeaVM requires Emscripten
./gradlew :webgpu:webgpu-build:webgpu_build_project_teavm_dawn

# Android requires NDK
./gradlew :webgpu:webgpu-build:webgpu_build_project_android_wgpu_jni
```


## Run Demos

```bash
./gradlew :demos:app:desktop-jni:webgpu_demo_app_run_desktop_jni
./gradlew :demos:app:desktop-ffm:webgpu_demo_app_run_desktop_ffm
./gradlew :demos:app:teavm:webgpu_demo_app_run_teavm
./gradlew :demos:app:android:installDebug
```

## Repository Layout

```text
webgpu/
  webgpu-base/          # base Java template layer
  webgpu-build/         # generation/native build driver
  webgpu-core/          # generated core Java API
  webgpu-desktop-jni/   # desktop JNI artifact + natives
  webgpu-desktop-ffm/   # desktop FFM artifact + natives
  webgpu-teavm/         # TeaVM JS/WASM bridge artifact
  webgpu-android/       # Android artifact + jniLibs packaging

demos/
  app/                  # runnable demos
  backend/              # backend abstraction modules for demos
```

## API Design Notes

### Naming conventions

- Custom classes start with `WGPU` (for example: `WGPU`, `WGPUAndroidWindow`).
- WebGPU classes start with `WGPU` (for example: `WGPUAdapter`, `WGPUDevice`).
- WebGPU enums start with `WGPU` (for example: `WGPUTextureFormat`, `WGPUPowerPreference`).
- Methods that require array/vector wrappers use `WGPUVector` prefixes.

### Memory management and `obtain`

Most non-opaque classes provide a static `obtain` method that reuses global instances to reduce Java/native allocations.

Use `obtain` when reuse is acceptable; create a new instance explicitly when you need a unique object and memory address.

## Development Notes

- Source-of-truth edits are typically in `webgpu/webgpu-base/src/main/java` and `webgpu/webgpu-build/src/main/cpp`.
- Generated targets live under `webgpu/webgpu-core`, `webgpu/webgpu-desktop-jni`, `webgpu/webgpu-desktop-ffm`, `webgpu/webgpu-android`, and `webgpu/webgpu-teavm`.

## Support

If you find this project valuable and want to fuel its continued growth, please consider [sponsoring](https://github.com/sponsors/xpenatan) it. Your support keeps the momentum going!

## License

jWebGPU is licensed under the [Apache License 2.0](LICENSE).
