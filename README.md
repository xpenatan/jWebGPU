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
- TeaVM C desktop runtime
- TeaVM/WebAssembly web runtime
- Android JNI runtime

## Quickstart: desktop JNI (release 0.3.4)

Use published dependencies to add jWebGPU to an application. A native compiler or a checkout of this repository is not required. The [source build commands](#building-jwebgpu-from-source) are for developing the bindings themselves.

This example uses **JDK 25, Windows x64 and WGPU**. It creates and releases a native WebGPU instance; it does not open a window. For rendering examples, see [Run Demos](#run-demos).

In a Gradle project with a wrapper, create `settings.gradle.kts`:

```kotlin
rootProject.name = "webgpu-hello"
```

Create `build.gradle.kts`:

```kotlin
plugins {
    application
}

repositories {
    mavenCentral()
}

val webgpuVersion = "0.3.4"

dependencies {
    implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-jni:$webgpuVersion")
    runtimeOnly("com.github.xpenatan.jWebGPU:webgpu-desktop-jni-wgpu_windows_x64:$webgpuVersion")
}

application {
    mainClass.set("WebGPUHello")
    applicationDefaultJvmArgs = listOf("--enable-native-access=ALL-UNNAMED")
}
```

Create `src/main/java/WebGPUHello.java`:

```java
import com.github.xpenatan.webgpu.JWebGPULoader;
import com.github.xpenatan.webgpu.WGPU;
import com.github.xpenatan.webgpu.WGPUInstance;

public class WebGPUHello {
    public static void main(String[] args) {
        JWebGPULoader.init((success, error) -> {
            if (!success) {
                System.err.println("Could not load jWebGPU");
                if (error != null) error.printStackTrace();
                return;
            }

            WGPUInstance instance = WGPU.setupInstance();
            if (instance == null || !instance.isValid()) {
                System.err.println("Could not create a WebGPU instance");
                return;
            }
            try {
                System.out.println("WebGPU instance created");
            } finally {
                instance.release();
            }
        });
    }
}
```

Run from that application's root:

```powershell
.\gradlew.bat run
```

Expected output: `WebGPU instance created`. Initialize through `JWebGPULoader` and wait for the successful callback before creating WebGPU objects. Creating an instance alone does not verify adapter/device creation or rendering support.

### Why `webgpu-core` is not an application runtime

**Do not add `webgpu-core` as `implementation` or `runtimeOnly` to JNI, FFM or Android applications.** It contains API stubs with the same class names as the working implementations. If those stubs load first, calls such as `WGPU.setupInstance()` can return `null` even after native loading succeeds. Earlier README installation instructions incorrectly combined core and JNI this way; the quickstart above corrects that for 0.3.4.

A shared source module using the `java-library` plugin can compile against the portable API:

```kotlin
dependencies {
    compileOnlyApi("com.github.xpenatan.jWebGPU:webgpu-core:0.3.4")
}
```

Each executable then supplies its platform runtime. For a single module, use `compileOnly` if you need the core API on the compile classpath. JNI already exposes the API through its dependencies. TeaVM web and C intentionally bring core transitively: their compiler replaces the stubs with platform implementations.

## Other Platforms and Backends

Keep all jWebGPU dependencies on the same version. The following snippets use `val webgpuVersion = "0.3.4"` and replace the quickstart's dependency block; choose one runtime per executable.

### Desktop native artifact names

The Java runtime and the native payload are separate artifacts. Choose the suffix matching the **JVM architecture**:

| Platform | Suffix |
| --- | --- |
| Windows x64 | `windows_x64` |
| Linux x64 | `linux_x64` |
| macOS Intel | `mac_x64` |
| macOS Apple Silicon | `mac_arm64` |

For JNI use `webgpu-desktop-jni-{backend}_{suffix}`; for FFM use `webgpu-desktop-ffm-{backend}_{suffix}`. `{backend}` is `wgpu` or `dawn`. These are artifact IDs, not Maven classifiers. Add the selected payload with `runtimeOnly`.

To use Dawn, select its native artifact and initialize with `JWebGPULoader.init(JWebGPUBackend.DAWN, listener)` (import `com.github.xpenatan.webgpu.JWebGPUBackend`). The overload in the quickstart defaults to WGPU. This loader choice selects the native WebGPU implementation; Vulkan, D3D12 and Metal are graphics backends within it.

### Desktop FFM

FFM requires **JDK 25** for these artifacts. Keep `--enable-native-access=ALL-UNNAMED` in the application JVM arguments.

```kotlin
dependencies {
    compileOnly("com.github.xpenatan.jWebGPU:webgpu-core:$webgpuVersion")
    implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-ffm:$webgpuVersion")
    runtimeOnly("com.github.xpenatan.jWebGPU:webgpu-desktop-ffm-wgpu_windows_x64:$webgpuVersion")
}
```

The compile-only dependency exposes the API and its public dependency types while keeping the stubs out of the runtime. Use the same initialization example as JNI.

### Android JNI

In an Android application module, use Maven Central and Google repositories and set `minSdk` to **29 or later**:

```kotlin
dependencies {
    implementation("com.github.xpenatan.jWebGPU:webgpu-android-wgpu:$webgpuVersion")
}
```

For Dawn, replace the artifact with `webgpu-android-dawn` and select `JWebGPUBackend.DAWN` during loading. Choose exactly one backend AAR: both contain `libjWebGPU.so`. The AAR includes the native payloads and brings the JNI Java runtime transitively; desktop native JARs are not needed. See the standalone examples for Android surface and activity lifecycle handling.

### TeaVM / browser

Use a TeaVM application with **Java 17 or later** and both artifacts:

```kotlin
dependencies {
    implementation("com.github.xpenatan.jWebGPU:webgpu-web:$webgpuVersion")
    implementation("com.github.xpenatan.jWebGPU:webgpu-web_wasm:$webgpuVersion")
}
```

`webgpu-web` supplies the Java API and TeaVM replacements; `webgpu-web_wasm` supplies `jWebGPU.js` and `jWebGPU.wasm`. Adding dependencies alone does not assemble a browser application: configure TeaVM and package the loader scripts and WASM resources as shown in the standalone examples. Serve the complete output over localhost or HTTPS in a browser with WebGPU support. The browser selects its graphics backend.

### TeaVM C

For a TeaVM C application, select one native backend/platform artifact:

```kotlin
dependencies {
    implementation("com.github.xpenatan.jWebGPU:webgpu-desktop-c-wgpu_windows_x64:$webgpuVersion")
}
```

It includes `webgpu-c` and the jParser C runtime transitively. This target requires a TeaVM C build and native toolchain. See the [TeaVM C packaging details](#teavm-c-packaging) below for platforms and linkage choices.

## Versions and Migration

The quickstart is pinned to **0.3.4**. This branch can contain APIs and build changes that have not been released; consult the [0.3.4 source and README](https://github.com/xpenatan/jWebGPU/tree/0.3.4) when comparing release APIs, but use the corrected dependency scopes above.

For development snapshots, set `webgpuVersion` to the literal `"-SNAPSHOT"` and add the snapshot repository alongside Maven Central:

```kotlin
repositories {
    mavenCentral()
    maven("https://central.sonatype.com/repository/maven-snapshots/")
}
```

Use `--refresh-dependencies` when updating a cached snapshot. A checkout may be ahead of the published snapshot; changing the repository URL cannot make an unpublished API available.

When moving from 0.1.x:

- Replace the old `webgpu-desktop` dependency/classifiers with a JNI or FFM Java runtime and its matching native artifact listed above.
- Keep `webgpu-core` compile-only in shared modules. Recheck the runtime classpath instead of relying on dependency order to select the right classes.
- Use the initialization callback shown above and select the loader backend matching the packaged native payload.
- In the current source tree, generator tasks use `:webgpu:builder:jParser_*`; the old `:webgpu:webgpu-build:*` paths no longer apply. Demos have moved to the standalone project, so `:demos:*` tasks no longer run from this checkout.

### Troubleshooting installation

| Symptom | Check |
| --- | --- |
| Native loading succeeds but `WGPU.setupInstance()` returns `null` | Remove `webgpu-core` from the JNI/FFM runtime classpath. Inspect it with `./gradlew dependencies --configuration runtimeClasspath`. |
| Loader reports a missing native library | Include the native artifact for your OS/JVM architecture, align versions, and match WGPU/Dawn loading to the packaged backend. |
| FFM fails with an unsupported class version | Run with JDK 25; inspect `./gradlew --version` as well as your IDE's run configuration. |
| `setBackendType` is missing on `WGPUInstanceDescriptor` | This API was added after 0.3.4. Use a build containing that change or the release-compatible `WGPU.setupInstance()` quickstart. |
| Old Gradle demo/build task cannot be found | Use the commands for your checked-out version; current demos run from jWebGPU-Examples. |

If reporting another problem, include the jWebGPU version, dependency block, OS/architecture, JDK, and full error output.

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

The runnable examples are maintained separately in **jWebGPU-Examples**.

## Building jWebGPU from Source

Run these commands from the **jWebGPU library checkout**. Use JDK 25 for this build and install the target's native toolchain (MSVC on Windows, the appropriate compiler on Linux/macOS, Android SDK/NDK for Android, or Emscripten for web). The Gradle wrapper is included; use `./gradlew` on Linux/macOS.

Download tasks are manual prerequisites for native builds. `jParser_build_*` tasks do not run them automatically.

Despite its name, `webgpu_download_glfw_windows` supplies the shared GLFW headers used by the desktop builds, including Linux and macOS.

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

### TeaVM C packaging

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

Run the Linux download prerequisites above first.

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

Run the macOS download prerequisites above first.

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

The examples now live in the standalone **jWebGPU-Examples** project. Open that project's root and follow its README for desktop JNI/FFM, Android and browser commands. The library build no longer includes `:demos:*` modules.

For published dependencies, run from the examples root:

```powershell
.\gradlew.bat -PuseLocalJWebGPU=false :app:desktop-jni:webgpu_demo_app_desktop_jni_wgpu_run
```

The current examples use APIs added after 0.3.4, including `WGPUInstanceDescriptor.setBackendType`. They need a published snapshot containing those APIs. Until that snapshot is available, place this library beside the examples as `../jWebGPU`, generate the bindings and build the selected native bridge using the source commands above, then run:

```powershell
.\gradlew.bat -PuseLocalJWebGPU=true :app:desktop-jni:webgpu_demo_app_desktop_jni_wgpu_run
```

The composite substitutes the Java libraries and their matching native payloads. `-PjWebGPUPath=E:/path/to/jWebGPU` overrides the sibling location. These instructions refer to the current source layout; the 0.3.4 tag still contains the older in-repository demos.

## Native startup fallback

**Source API added after 0.3.4:** the following requires a matching newer library build. It is not part of the release quickstart above. With the WGPU loader, restrict each startup attempt using the instance descriptor:

```java
WGPUInstanceDescriptor descriptor = new WGPUInstanceDescriptor();
descriptor.setBackendType(WGPUBackendType.Vulkan);
WGPUInstance instance = WGPU.setupInstance(descriptor);
descriptor.dispose();
```

Check `instance.isValid()` before using it. Set the adapter request's backend type to the same value.
The instance setting controls which native backends may initialize surfaces; the adapter option selects
an adapter within that instance. `Undefined` keeps the loader defaults. Explicit instance backend selection
is supported by wgpu-native; Dawn and browser callers should keep `Undefined`.

The standalone demos own their ordered startup retries and cleanup; see their README for platform fallback behavior. Native process crashes cannot be recovered by an in-process retry.

## Development Notes

- Edit binding templates in `webgpu/base/src/main/java/**`.
- Edit IDL and native glue in `webgpu/builder/src/main/cpp/**`.
- Do not hand-edit generated Java under `webgpu/core`, `webgpu/shared/jni`, `webgpu/shared/c`, `webgpu/desktop/ffm`, or `webgpu/web/wasm`.

## Ecosystem

For LibGDX projects, [gdx-webgpu](https://github.com/MonstrousSoftware/gdx-webgpu) is a higher-level backend extension built on top of jWebGPU.

## Support

If you find this project valuable and want to fuel its continued growth, please consider [sponsoring](https://github.com/sponsors/xpenatan).

## License

jWebGPU is licensed under the [Apache License 2.0](LICENSE).
