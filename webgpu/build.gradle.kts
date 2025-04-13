import de.undercouch.gradle.tasks.download.Download
import org.gradle.api.tasks.Exec
import org.gradle.api.GradleException
import java.io.File

plugins {
    id("de.undercouch.download") version("5.4.0")
}

val buildDir = layout.buildDirectory.get().asFile
val zippedPath = "${buildDir}/dawn.tar.gz"
val sourcePath = "${buildDir}/dawn"
val sourceDestination = "${buildDir}/dawn/"

tasks.register<Download>("download_source") {
    group = "webgpu"
    description = "Download WebGPU Dawn source"
    src("https://dawn.googlesource.com/dawn/+archive/refs/heads/chromium/6613.tar.gz")
    dest(File(zippedPath))
    onlyIfModified(true)
    doLast {
        try {
            // Ensure destination directory exists
            File(sourceDestination).mkdirs()

            // Extract the tar.gz file using system tar command
            exec {
                commandLine = listOf("tar", "-xzf", zippedPath, "-C", sourceDestination)
                workingDir = buildDir
            }

            // Check for nested .tar files
            File(sourceDestination).walk().forEach { file ->
                if (file.extension == "tar") {
                    println("Found nested tar file: ${file.name}")
                    exec {
                        commandLine = listOf("tar", "-xf", file.absolutePath, "-C", sourceDestination)
                        workingDir = buildDir
                    }
                    file.delete()
                }
            }

            // Clean up
            delete(zippedPath)
        } catch (e: Exception) {
            throw GradleException("Failed to process tar.gz: ${e.message}. Ensure 'tar' is installed and the file is valid.", e)
        }
    }
}

// Helper function to create configure and build tasks for a platform and architecture
fun createBuildTasks(platform: String, arch: String, cmakeArgs: List<String>, buildDir: String) {
    // Ensure the build directory exists
    val buildDirFile = File(buildDir)
    buildDirFile.mkdirs()

    // Task to configure the build with CMake
    val configureTaskName = "configure${platform.capitalize()}${arch.replace("-", "_").capitalize()}"
    tasks.register<Exec>(configureTaskName) {
        workingDir = buildDirFile
        // Explicitly point to the project root where CMakeLists.txt resides
        commandLine("cmake", *cmakeArgs.toTypedArray(), "-DBUILD_STATIC_LIB=ON", project.projectDir.absolutePath)
    }

    // Task to build the static library
    val buildTaskName = "build${platform.capitalize()}${arch.replace("-", "_").capitalize()}"
    tasks.register<Exec>(buildTaskName) {
        dependsOn(configureTaskName)
        workingDir = buildDirFile
        commandLine("cmake", "--build", ".", "--config", "Release")
    }
}

// Windows: x86_64 and ARM64
createBuildTasks(
    "windows", "x64",
    listOf("-G", "Visual Studio 17 2022", "-A", "x64", "-DCMAKE_BUILD_TYPE=Release"),
    "build/windows_x64"
)
createBuildTasks(
    "windows", "arm64",
    listOf("-G", "Visual Studio 17 2022", "-A", "ARM64", "-DCMAKE_BUILD_TYPE=Release"),
    "build/windows_arm64"
)

// Mac: x86_64, ARM64, and universal binary
val macArchs = listOf("x86_64", "arm64")
macArchs.forEach { arch ->
    createBuildTasks(
        "mac", arch,
        listOf("-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_OSX_ARCHITECTURES=$arch"),
        "build/mac_$arch"
    )
}

// Task to create a universal binary for Mac
tasks.register<Exec>("buildMacUniversal") {
    dependsOn("buildMacX86_64", "buildMacArm64")
    val libX64 = File("build/mac_x86_64/libwebgpu_static.a")
    val libArm64 = File("build/mac_arm64/libwebgpu_static.a")
    val universalLib = File("build/mac_universal/libwebgpu_static.a")
    universalLib.parentFile.mkdirs()
    workingDir = universalLib.parentFile
    commandLine("lipo", "-create", "-output", universalLib.absolutePath, libX64.absolutePath, libArm64.absolutePath)
}

// Linux: x86_64 and ARM64
createBuildTasks(
    "linux", "x64",
    listOf("-DCMAKE_BUILD_TYPE=Release"),
    "build/linux_x64"
)
createBuildTasks(
    "linux", "arm64",
    listOf("-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc", "-DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++"),
    "build/linux_arm64"
)

// Android: armeabi-v7a, arm64-v8a, x86, x86_64
val androidAbis = listOf("armeabi-v7a", "arm64-v8a", "x86", "x86_64")
androidAbis.forEach { abi ->
    val ndkHome = System.getenv("ANDROID_NDK_HOME") ?: throw GradleException("ANDROID_NDK_HOME not set")
    createBuildTasks(
        "android", abi,
        listOf(
            "-DCMAKE_TOOLCHAIN_FILE=${ndkHome}/build/cmake/android.toolchain.cmake",
            "-DANDROID_ABI=$abi",
            "-DANDROID_PLATFORM=android-29",
            "-DCMAKE_BUILD_TYPE=Release"
        ),
        "build/android_$abi"
    )
}

// iOS: ARM64 (devices) and x86_64 (simulator)
createBuildTasks(
    "ios", "arm64",
    listOf("-DCMAKE_SYSTEM_NAME=iOS", "-DCMAKE_OSX_ARCHITECTURES=arm64", "-DCMAKE_BUILD_TYPE=Release"),
    "build/ios_arm64"
)
createBuildTasks(
    "ios", "x86_64",
    listOf("-DCMAKE_SYSTEM_NAME=iOS", "-CMAKE_OSX_ARCHITECTURES=x86_64", "-CMAKE_IOS_SIMULATOR=YES", "-DCMAKE_BUILD_TYPE=Release"),
    "build/ios_x86_64"
)

// Emscripten: WebAssembly
createBuildTasks(
    "emscripten", "wasm",
    listOf("-DCMAKE_BUILD_TYPE=Release"),
    "build/emscripten"
)