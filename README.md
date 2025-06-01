# jWebGPU

A multi-platform WebGPU binding for Desktop, Mobile and Web using Java. 

## Build steps

1) Download all dependencies files. They will be located at webgpu/webgpu-wgpu/build
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

2) Build the bindings for all platforms. Your PC must be configured to be able to build it. <br>
The native files will be located at webgpu/webgpu-build/build/C++/libs
```
// Windows requires Visual studio 2022 C++
./gradlew :webgpu:webgpu-build:wgpu_build_project_windows64

// TeaVM requires Emscripten
./gradlew :webgpu:webgpu-build:wgpu_build_project_teavm

// Android requires NDK
./gradlew :webgpu:webgpu-build:wgpu_build_project_android
```

3) Run/Install the app demo like this:
```
./gradlew :demos:app:desktop:webgpu_demo_app_desktop
./gradlew :demos:app:teavm:webgpu_demo_app_run_teavm
./gradlew :demos:app:android:installDebug

```