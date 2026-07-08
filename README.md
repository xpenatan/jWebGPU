# jWebGPU

![Build](https://github.com/xpenatan/jWebGPU/actions/workflows/snapshot.yml/badge.svg)
[![Maven Central Version](https://img.shields.io/maven-central/v/com.github.xpenatan.jWebGPU/webgpu-core)](https://central.sonatype.com/namespace/com.github.xpenatan.jWebGPU)
[![Snapshot](https://img.shields.io/badge/snapshot---SNAPSHOT-red)](https://central.sonatype.com/service/rest/repository/browse/maven-snapshots/com/github/xpenatan/jWebGPU/)

jWebGPU is a Java binding stack for WebGPU APIs across desktop, Android, and web targets.

<img width="790" height="592" alt="image" src="https://github.com/user-attachments/assets/21317836-0c92-4457-9354-22b494cfc438" />
<br>

- Browser example: [Studio demo](https://xpenatan.github.io/jWebGPU/index.html?demo=studio)
- Android app: [Google Play](https://play.google.com/store/apps/details?id=com.github.xpenatan.webgpu.demo)

## What This Project Is For

- Build and ship WebGPU-capable Java apps across multiple runtimes.
- Reuse the same Java-facing API while swapping backend/runtime packaging per platform.
- Generate bindings from `webgpu/base` templates plus `webgpu/builder` IDL/native sources using the jParser Gradle plugin.

Targets:

- JNI desktop runtime
- FFM desktop runtime
- TeaVM/WebAssembly web runtime
- Android JNI runtime

## Project Structure

Canonical modules are defined in `settings.gradle.kts`.

Library modules:

- `:webgpu:download` - native dependency download tasks.
- `:webgpu:dawn` - experimental Dawn source build helpers.
- `:webgpu:builder` - jParser plugin configuration and native build driver.
- `:webgpu:base` - hand-authored Java templates and directive blocks.
- `:webgpu:core` - generated Java API layer.
- `:webgpu:shared:jni` - generated JNI Java runtime.
- `:webgpu:desktop:jni` - desktop JNI native packaging.
- `:webgpu:desktop:ffm` - generated FFM Java runtime and desktop native packaging.
- `:webgpu:web:wasm` - TeaVM/WebAssembly runtime packaging.
- `:webgpu:android:jni` - Android JNI runtime packaging.

Demo modules:

- `:demos:app:desktop-jni`
- `:demos:app:desktop-ffm`
- `:demos:app:web`
- `:demos:app:android`

## Build Commands

Download tasks are manual prerequisites for native builds. `jParser_build_*` tasks do not run them automatically.

Generate Java bindings:

```powershell
.\gradlew.bat :webgpu:builder:jParser_generate
```

Windows JNI/FFM:

```powershell
.\gradlew.bat :webgpu:download:webgpu_download_glfw_windows
.\gradlew.bat :webgpu:download:webgpu_download_windows_x86_64_wgpu
.\gradlew.bat :webgpu:download:webgpu_download_windows_x86_64_dawn
.\gradlew.bat :webgpu:builder:jParser_build_windows64_jni_wgpu :webgpu:builder:jParser_build_windows64_jni_dawn
.\gradlew.bat :webgpu:builder:jParser_build_windows64_ffm_wgpu :webgpu:builder:jParser_build_windows64_ffm_dawn
```

Linux JNI/FFM:

```bash
./gradlew :webgpu:download:webgpu_download_linux_x86_64_wgpu
./gradlew :webgpu:download:webgpu_download_glfw_windows
./gradlew :webgpu:builder:jParser_build_linux64_jni_wgpu
./gradlew :webgpu:builder:jParser_build_linux64_ffm_wgpu
```

macOS JNI/FFM:

```bash
./gradlew :webgpu:download:webgpu_download_macos_x86_64_wgpu
./gradlew :webgpu:download:webgpu_download_macos_aarch64_wgpu
./gradlew :webgpu:download:webgpu_download_glfw_windows
./gradlew :webgpu:builder:jParser_build_mac64_jni_wgpu :webgpu:builder:jParser_build_macArm_jni_wgpu
./gradlew :webgpu:builder:jParser_build_mac64_ffm_wgpu :webgpu:builder:jParser_build_macArm_ffm_wgpu
```

TeaVM/WebAssembly:

```powershell
.\gradlew.bat :webgpu:download:webgpu_download_emdawnwebgpu
.\gradlew.bat :webgpu:builder:jParser_build_web_wasm
```

Android JNI:

```powershell
.\gradlew.bat :webgpu:download:webgpu_download_android_x86_64_wgpu
.\gradlew.bat :webgpu:download:webgpu_download_android_i686_wgpu
.\gradlew.bat :webgpu:download:webgpu_download_android_armv7_wgpu
.\gradlew.bat :webgpu:download:webgpu_download_android_aarch64_wgpu
.\gradlew.bat :webgpu:builder:jParser_build_android_jni
```

Generated native outputs are placed under `webgpu/builder/build/c++/libs/`.

## Run Demos

```powershell
.\gradlew.bat :demos:app:desktop-jni:webgpu_demo_app_run_desktop_jni
.\gradlew.bat :demos:app:desktop-ffm:webgpu_demo_app_run_desktop_ffm
.\gradlew.bat :demos:app:web:webgpu_demo_app_run_web
.\gradlew.bat :demos:app:android:installDebug
```

## Development Notes

- Edit binding templates in `webgpu/base/src/main/java/**`.
- Edit IDL and native glue in `webgpu/builder/src/main/cpp/**`.
- Do not hand-edit generated Java under `webgpu/core`, `webgpu/shared/jni`, `webgpu/desktop/ffm`, or `webgpu/web/wasm`.

## Installation

Use group ID `com.github.xpenatan.jWebGPU`.

```kotlin
dependencies {
    implementation("com.github.xpenatan.jWebGPU:webgpu-core:<version>")

    // Choose one runtime target
    implementation("com.github.xpenatan.jWebGPU:webgpu-jni:<version>")
    implementation("com.github.xpenatan.jWebGPU:webgpu-jni-desktop:<version>")

    // implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-ffm:<version>")
    // implementation("com.github.xpenatan.jWebGPU:webgpu-web:<version>")
    // implementation("com.github.xpenatan.jWebGPU:webgpu-android:<version>")
}
```

## Ecosystem

For LibGDX projects, [gdx-webgpu](https://github.com/MonstrousSoftware/gdx-webgpu) is a higher-level backend extension built on top of jWebGPU.

## Support

If you find this project valuable and want to fuel its continued growth, please consider [sponsoring](https://github.com/sponsors/xpenatan).

## License

jWebGPU is licensed under the [Apache License 2.0](LICENSE).
