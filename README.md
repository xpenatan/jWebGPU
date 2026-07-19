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
- TeaVM C desktop runtime bootstrap (Windows x64 WGPU)
- TeaVM/WebAssembly web runtime
- Android JNI runtime

## Project Structure

Canonical modules are defined in `settings.gradle.kts`.

Library modules:

- `:webgpu:download` - native dependency download tasks.
- `:webgpu:builder` - jParser plugin configuration and native build driver.
- `:webgpu:base` - hand-authored Java templates and directive blocks.
- `:webgpu:core` - generated Java API layer.
- `:webgpu:shared:jni` - generated JNI Java runtime.
- `:webgpu:shared:c` - generated TeaVM C implementations and portable native resources.
- `:webgpu:desktop:c` - desktop TeaVM C native-resource packaging.
- `:webgpu:desktop:jni` - desktop JNI native packaging.
- `:webgpu:desktop:ffm` - generated FFM Java runtime and desktop native packaging.
- `:webgpu:web:wasm` - TeaVM/WebAssembly runtime packaging.
- `:webgpu:android:jni` - WGPU/Dawn Android JNI runtime packaging.

Demo modules:

- `:demos:backend:core`
- `:demos:backend:desktop`
- `:demos:backend:web`
- `:demos:backend:android`
- `:demos:app:core`
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

Desktop TeaVM C publishes one artifact per native backend and host platform:

| Backend | Windows x64 | Linux x64 | macOS x64 | macOS arm64 |
| --- | --- | --- | --- | --- |
| WGPU | `webgpu-desktop-c-wgpu_windows_x64` | `webgpu-desktop-c-wgpu_linux_x64` | `webgpu-desktop-c-wgpu_mac_x64` | `webgpu-desktop-c-wgpu_mac_arm64` |
| Dawn | `webgpu-desktop-c-dawn_windows_x64` | `webgpu-desktop-c-dawn_linux_x64` | `webgpu-desktop-c-dawn_mac_x64` | `webgpu-desktop-c-dawn_mac_arm64` |

Add exactly one matching artifact to a TeaVM C application. Backend selection is a build-time choice. Linkage is a separate consumer choice: `JPARSER_JWEBGPU_TEAVMC_LINKAGE` accepts `STATIC`, `SHARED_LINKED`, or `RUNTIME_LOADED`, and jParser defaults it to `STATIC` when the application does not select a mode.

The WGPU/Dawn consumer requirements are declared in `webgpu/builder/build.gradle.kts`. jParser folds all platform/backend declarations into the existing generated `jparser_jwebgpu_teavm_c.cmake` inside `webgpu-c`; backend native artifacts contain only headers and native libraries, with no handwritten CMake hook. The packaged `include/webgpu/wgpu.h` or `include/dawn/webgpu.h` selector identifies the chosen backend. Supplying both backend artifacts, or neither matching artifact for the current platform, fails during CMake configuration instead of silently linking the wrong implementation.

The published Windows static payloads currently use the dynamic MSVC runtime and are stored as the `md` variant. A static consumer therefore selects `/MD`. The shared bridge remains usable by `/MD` and `/MT` applications because the CRT stays behind the DLL boundary. A static `/MT` consumer can rebuild the jWebGPU bridge and selected WebGPU dependency with `/MT`, package them under the matching `mt` payload directory, or provide them with `JPARSER_JWEBGPU_TEAVMC_LIBRARY` and `JPARSER_JWEBGPU_{WGPU|DAWN}_NATIVE_LIBRARY`. jWebGPU does not silently change the application's runtime or linkage choice.

Windows TeaVM C:

```powershell
.\gradlew.bat :webgpu:download:webgpu_download_glfw_windows
.\gradlew.bat :webgpu:download:webgpu_download_windows_x86_64_wgpu
.\gradlew.bat :webgpu:download:webgpu_download_windows_x86_64_dawn
.\gradlew.bat :webgpu:builder:jParser_build_windows64_teavm_c_wgpu :webgpu:builder:jParser_build_windows64_teavm_c_dawn
.\gradlew.bat :webgpu:shared:c:jar
.\gradlew.bat :webgpu:desktop:c:nativeJar_wgpu_windows_x64 :webgpu:desktop:c:nativeJar_dawn_windows_x64
```

Linux JNI/FFM:

```bash
./gradlew :webgpu:download:webgpu_download_linux_x86_64_wgpu
./gradlew :webgpu:download:webgpu_download_linux_x86_64_dawn
./gradlew :webgpu:download:webgpu_download_glfw_windows
./gradlew :webgpu:builder:jParser_build_linux64_jni_wgpu :webgpu:builder:jParser_build_linux64_jni_dawn
./gradlew :webgpu:builder:jParser_build_linux64_ffm_wgpu :webgpu:builder:jParser_build_linux64_ffm_dawn
```

Linux TeaVM C:

```bash
./gradlew :webgpu:builder:jParser_build_linux64_teavm_c_wgpu :webgpu:builder:jParser_build_linux64_teavm_c_dawn
./gradlew :webgpu:desktop:c:nativeJar_wgpu_linux_x64 :webgpu:desktop:c:nativeJar_dawn_linux_x64
```

macOS JNI/FFM:

```bash
./gradlew :webgpu:download:webgpu_download_macos_x86_64_wgpu
./gradlew :webgpu:download:webgpu_download_macos_aarch64_wgpu
./gradlew :webgpu:download:webgpu_download_macos_x86_64_dawn
./gradlew :webgpu:download:webgpu_download_macos_aarch64_dawn
./gradlew :webgpu:download:webgpu_download_glfw_windows
./gradlew :webgpu:builder:jParser_build_mac64_jni_wgpu :webgpu:builder:jParser_build_mac64_jni_dawn :webgpu:builder:jParser_build_macArm_jni_wgpu :webgpu:builder:jParser_build_macArm_jni_dawn
./gradlew :webgpu:builder:jParser_build_mac64_ffm_wgpu :webgpu:builder:jParser_build_mac64_ffm_dawn :webgpu:builder:jParser_build_macArm_ffm_wgpu :webgpu:builder:jParser_build_macArm_ffm_dawn
```

macOS TeaVM C:

```bash
./gradlew :webgpu:builder:jParser_build_mac64_teavm_c_wgpu :webgpu:builder:jParser_build_mac64_teavm_c_dawn :webgpu:builder:jParser_build_macArm_teavm_c_wgpu :webgpu:builder:jParser_build_macArm_teavm_c_dawn
./gradlew :webgpu:desktop:c:nativeJar_wgpu_mac_x64 :webgpu:desktop:c:nativeJar_dawn_mac_x64 :webgpu:desktop:c:nativeJar_wgpu_mac_arm64 :webgpu:desktop:c:nativeJar_dawn_mac_arm64
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
.\gradlew.bat :webgpu:download:webgpu_download_android_x86_64_dawn
.\gradlew.bat :webgpu:download:webgpu_download_android_i686_dawn
.\gradlew.bat :webgpu:download:webgpu_download_android_armv7_dawn
.\gradlew.bat :webgpu:download:webgpu_download_android_aarch64_dawn
.\gradlew.bat :webgpu:builder:jParser_build_android_jni_wgpu :webgpu:builder:jParser_build_android_jni_dawn
```

Generated native outputs are placed under `webgpu/builder/build/c++/libs/`.

Native compiler policy is configured through jParser's generic target hooks. The current Windows bridge payloads, and the downloaded Dawn archive they can statically link, use the dynamic CRT, so the producer target declarations pass `compileFlag("/MD")` explicitly; there is no jWebGPU or jParser `msvcRuntime` type. Rebuilding the bridge and dependencies with another compatible ABI only requires changing the ordinary compiler flag and packaging the result in the matching runtime directory. Linux, macOS, Android, and iOS use their normal toolchain options; MSVC `/MT` and `/MD` flags are Windows-only.

## Run Demos

```powershell
.\gradlew.bat :demos:app:desktop-jni:webgpu_demo_app_desktop_jni_wgpu_run
.\gradlew.bat :demos:app:desktop-jni:webgpu_demo_app_desktop_jni_dawn_run
.\gradlew.bat :demos:app:desktop-ffm:webgpu_demo_app_desktop_ffm_wgpu_run
.\gradlew.bat :demos:app:desktop-ffm:webgpu_demo_app_desktop_ffm_dawn_run
.\gradlew.bat :demos:app:web:webgpu_demo_app_web_run
.\gradlew.bat :demos:app:android:installWgpuDebug
.\gradlew.bat :demos:app:android:installDawnDebug
```

## Development Notes

- Edit binding templates in `webgpu/base/src/main/java/**`.
- Edit IDL and native glue in `webgpu/builder/src/main/cpp/**`.
- Do not hand-edit generated Java under `webgpu/core`, `webgpu/shared/jni`, `webgpu/shared/c`, `webgpu/desktop/ffm`, or `webgpu/web/wasm`.

## Installation

Use group ID `com.github.xpenatan.jWebGPU`.

```kotlin
dependencies {
    implementation("com.github.xpenatan.jWebGPU:webgpu-core:<version>")

    // Desktop JNI: choose one backend native artifact for your platform.
    implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-jni:<version>")
    runtimeOnly("com.github.xpenatan.jWebGPU:webgpu-desktop-jni-wgpu_windows_x64:<version>")
    // runtimeOnly("com.github.xpenatan.jWebGPU:webgpu-desktop-jni-dawn_windows_x64:<version>")

    // Desktop FFM: choose one backend native artifact for your platform.
    // implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-ffm:<version>")
    // runtimeOnly("com.github.xpenatan.jWebGPU:webgpu-desktop-ffm-wgpu_windows_x64:<version>")
    // runtimeOnly("com.github.xpenatan.jWebGPU:webgpu-desktop-ffm-dawn_windows_x64:<version>")

    // TeaVM C, Windows x64 WGPU: includes webgpu-c and the jParser C runtime transitively.
    // implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-c-wgpu_windows_x64:<version>")

    // TeaVM/WebAssembly requires both the Java runtime and WebAssembly payload.
    // implementation("com.github.xpenatan.jWebGPU:webgpu-web:<version>")
    // implementation("com.github.xpenatan.jWebGPU:webgpu-web_wasm:<version>")

    // Android: choose exactly one backend AAR.
    // implementation("com.github.xpenatan.jWebGPU:webgpu-android-wgpu:<version>")
    // implementation("com.github.xpenatan.jWebGPU:webgpu-android-dawn:<version>")
}
```

Android backend AARs are mutually exclusive because both package `libjWebGPU.so`. Applications using `webgpu-android-dawn` must initialize with `JWebGPULoader.init(JWebGPUBackend.DAWN, listener)`; the overload without a backend keeps WGPU as its default.

## Ecosystem

For LibGDX projects, [gdx-webgpu](https://github.com/MonstrousSoftware/gdx-webgpu) is a higher-level backend extension built on top of jWebGPU.

## Support

If you find this project valuable and want to fuel its continued growth, please consider [sponsoring](https://github.com/sponsors/xpenatan).

## License

jWebGPU is licensed under the [Apache License 2.0](LICENSE).
