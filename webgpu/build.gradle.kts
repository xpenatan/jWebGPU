import de.undercouch.gradle.tasks.download.Download
import org.gradle.api.tasks.Exec
import org.gradle.api.GradleException
import java.io.File

plugins {
    id("de.undercouch.download") version("5.4.0")
    id("java")
}

val buildDir = layout.buildDirectory.get().asFile
val zippedPath = "${buildDir}/dawn.tar.gz"
val sourcePath = "${buildDir}/dawn"
val sourceDestination = "${buildDir}/dawn/"
val libsDir = "${buildDir}/libs"

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

            // Fetch dependencies
            exec {
                commandLine("python", "tools/fetch_dawn_dependencies.py", "--use-test-deps")
                workingDir = file(sourceDestination)
            }
        } catch (e: Exception) {
            throw GradleException("Failed to process tar.gz: ${e.message}. Ensure 'tar' is installed and the file is valid.", e)
        }
    }
}

// Define platforms and their target architectures
val platforms = mapOf(
    "windows" to listOf("x64", "arm64"),
    "mac" to listOf("x64", "arm64"),
    "linux" to listOf("x64", "arm64"),
    "ios" to listOf("arm64", "x64"), // arm64 for devices, x64 for simulator
    "android" to listOf("arm64", "armv7", "x64", "x86"),
    "emscripten" to listOf("wasm")
)

// Function to get library file patterns based on platform and build type
fun getLibPatterns(platform: String, buildType: String): List<String> {
    return when (buildType) {
        "static" -> when (platform) {
            "windows" -> listOf("**/*.lib")
            else -> listOf("**/*.a")
        }
        "shared" -> when (platform) {
            "windows" -> listOf("**/*.dll")
            "mac" -> listOf("**/*.dylib")
            "linux", "android" -> listOf("**/*.so")
            "ios" -> listOf("**/*.dylib") // Possible for simulator or frameworks
            "emscripten" -> listOf("**/*.wasm") // WebAssembly as a shared-like output
            else -> throw GradleException("Unsupported platform for shared libraries: $platform")
        }
        else -> throw GradleException("Invalid build type: $buildType")
    }
}

// Function to create CMake generation tasks
fun createGenerateCMakeTask(platform: String, arch: String, buildType: String) {
    val taskName = "generateCMake_${buildType}_${platform}_$arch"
    tasks.register<Exec>(taskName) {
        description = "Generates CMake build files for $buildType libraries on $platform $arch."
        workingDir(sourcePath)
        val buildDirForArch = "${sourcePath}/build_${buildType}_${platform}_$arch"
        val cmakeArgs = mutableListOf(
            "-B", buildDirForArch,
            "-S", ".",
            "-DBUILD_SHARED_LIBS=${if (buildType == "shared") "ON" else "OFF"}"
        )
        when (platform) {
            "windows" -> {
                val cmakeArch = if (arch == "x64") "x64" else "ARM64"
                cmakeArgs.addAll(listOf("-A", cmakeArch))
            }
            "mac" -> {
                cmakeArgs.addAll(listOf("-DCMAKE_OSX_ARCHITECTURES=$arch"))
            }
            "linux" -> {
                val cmakeProcessor = if (arch == "x64") "x86_64" else "aarch64"
                cmakeArgs.addAll(listOf("-DCMAKE_SYSTEM_PROCESSOR=$cmakeProcessor"))
            }
            "ios" -> {
                val cmakeArch = if (arch == "x64") "x86_64" else "arm64"
                cmakeArgs.addAll(listOf(
                    "-DCMAKE_SYSTEM_NAME=iOS",
                    "-DCMAKE_OSX_ARCHITECTURES=$cmakeArch",
                    "-DCMAKE_OSX_SYSROOT=iphoneos"
                ))
            }
            "android" -> {
                val abi = when (arch) {
                    "arm64" -> "arm64-v8a"
                    "armv7" -> "armeabi-v7a"
                    "x64" -> "x86_64"
                    "x86" -> "x86"
                    else -> throw GradleException("Unsupported Android architecture: $arch")
                }
                val ndkHome = System.getenv("ANDROID_NDK_HOME") ?: throw GradleException("ANDROID_NDK_HOME not set")
                cmakeArgs.addAll(listOf(
                    "-DCMAKE_SYSTEM_NAME=Android",
                    "-CMAKE_ANDROID_NDK=$ndkHome",
                    "-DCMAKE_ANDROID_ARCH_ABI=$abi",
                    "-DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang",
                    "-DCMAKE_ANDROID_STL_TYPE=c++_static"
                ))
            }
            "emscripten" -> {
                val emsdkHome = System.getenv("EMSDK") ?: throw GradleException("EMSDK not set")
                cmakeArgs.addAll(listOf(
                    "-DCMAKE_TOOLCHAIN_FILE=$emsdkHome/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake",
                    "-DCMAKE_BUILD_TYPE=Release"
                ))
            }
        }
        commandLine("cmake", *cmakeArgs.toTypedArray())
    }
}

// Function to create build tasks
fun createBuildTask(platform: String, arch: String, buildType: String) {
    val taskName = "build_${buildType}_${platform}_$arch"
    tasks.register<Exec>(taskName) {
        description = "Builds the $buildType libraries for $platform on $arch."
        dependsOn("generateCMake_${buildType}_${platform}_$arch")
        workingDir("${sourcePath}/build_${buildType}_${platform}_$arch")
        commandLine("cmake", "--build", ".", "--config", "Release")
    }
}

// Function to create copy tasks for built libraries
fun createCopyLibsTask(platform: String, arch: String, buildType: String) {
    val taskName = "copyLibs_${buildType}_${platform}_$arch"
    tasks.register<Copy>(taskName) {
        description = "Copies $buildType libraries for $platform on $arch."
        dependsOn("build_${buildType}_${platform}_$arch")
        val patterns = getLibPatterns(platform, buildType)
        from(fileTree("${sourcePath}/build_${buildType}_${platform}_$arch").matching { include(*patterns.toTypedArray()) }.files)
        into("$libsDir/$platform/$arch")
    }
}

// Generate tasks for all platforms and architectures
platforms.forEach { (platform, archs) ->
    archs.forEach { arch ->
        // Create static library tasks for all platforms
        createGenerateCMakeTask(platform, arch, "static")
        createBuildTask(platform, arch, "static")
        createCopyLibsTask(platform, arch, "static")

        // Create shared library tasks for all platforms
        createGenerateCMakeTask(platform, arch, "shared")
        createBuildTask(platform, arch, "shared")
        createCopyLibsTask(platform, arch, "shared")
    }
}