import de.undercouch.gradle.tasks.download.Download
import org.gradle.api.tasks.Exec
import org.gradle.api.GradleException
import java.io.File

plugins {
    id("de.undercouch.download") version "5.4.0"
}

val buildDir = layout.buildDirectory.get().asFile
val zippedPath = "${buildDir}/dawn.tar.gz"
val sourcePath = "${buildDir}/dawn"
val sourceDestination = "${buildDir}/dawn/"
val libsDir = "${buildDir}/libs"

// Define platforms and their architectures
val platforms = mapOf(
    "windows" to listOf("x64", "arm64"),
    "mac" to listOf("x64", "arm64"),
    "linux" to listOf("x64", "arm64"),
    "ios" to listOf("arm64", "x64"),
    "android" to listOf("arm64", "armv7", "x64", "x86"),
    "emscripten" to listOf("wasm")
)

// Define libraries to combine for each platform (customize this list as needed)
val libsToCombine = mapOf(
    "windows" to listOf("dawn.lib", "dxgi.lib", "d3d11.lib", "d3d12.lib"),
    "mac" to listOf("libdawn.a", "libmetal.a"),
    "linux" to listOf("libdawn.a", "libvulkan.a"),
    "ios" to listOf("libdawn.a", "libmetal.a"),
    "android" to listOf("libdawn.a", "libvulkan.a"),
    "emscripten" to listOf() // No combination for Emscripten
)

// Build types per platform
val buildTypes = mapOf(
    "windows" to "shared",
    "mac" to "static",
    "linux" to "static",
    "ios" to "static",
    "android" to "static",
    "emscripten" to "shared"
)

// Library file patterns by platform and build type
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
            "ios" -> listOf("**/*.dylib")
            "emscripten" -> listOf("**/*.wasm")
            else -> throw GradleException("Unsupported platform: $platform")
        }
        else -> throw GradleException("Invalid build type: $buildType")
    }
}

// Task to download and extract Dawn source
tasks.register<Download>("download_source") {
    group = "webgpu"
    description = "Download WebGPU Dawn source"
    src("https://dawn.googlesource.com/dawn/+archive/refs/heads/chromium/6613.tar.gz")
    dest(File(zippedPath))
    onlyIfModified(true)
    doLast {
        try {
            File(sourceDestination).mkdirs()
            exec {
                commandLine = listOf("tar", "-xzf", zippedPath, "-C", sourceDestination)
                workingDir = buildDir
            }
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
            delete(zippedPath)
        } catch (e: Exception) {
            throw GradleException("Failed to process tar.gz: ${e.message}. Ensure 'tar' is installed.", e)
        }
    }
}

// Generate CMake build files for a platform and architecture
fun createGenerateCMakeTask(platform: String, arch: String) {
    val taskName = "generateCMake_${platform}_$arch"
    tasks.register<Exec>(taskName) {
        description = "Generates CMake build files for $platform $arch."
        group = "webgpu"
        workingDir(sourcePath)
        val buildDir = "${sourcePath}/build_${platform}_$arch"
        val buildType = buildTypes[platform] ?: throw GradleException("Unknown platform: $platform")
        val cmakeArgs = mutableListOf(
            "-B", buildDir,
            "-S", ".",
            "-DDAWN_FETCH_DEPENDENCIES=ON",
            "-DCMAKE_BUILD_TYPE=Release",
//            "-DDAWN_BUILD_SAMPLES=OFF",
//            "-DDAWN_BUILD_TESTS=OFF",
//            "-DDAWN_ENABLE_D3D12=ON",
//            "-DDAWN_ENABLE_D3D11=OFF",
//            "-DDAWN_ENABLE_DESKTOP_GL=OFF",
//            "-DDAWN_ENABLE_OPENGLES=OFF",
//            "-DDAWN_ENABLE_VULKAN=OFF",
//            "-DDAWN_USE_GLFW=OFF",
//            "-DDAWN_ENABLE_SPIRV_VALIDATION=OFF",
//            "-DDAWN_DXC_ENABLE_ASSERTS_IN_NDEBUG=OFF",
            "-DDAWN_BUILD_MONOLITHIC_LIBRARY=ON",
//            "-DTINT_BUILD_TESTS=OFF"
//            "-DBUILD_SHARED_LIBS=${if (buildType == "static") "OFF" else "ON"}"
            "-DBUILD_SHARED_LIBS=OFF"
        )
        when (platform) {
            "windows" -> cmakeArgs.addAll(listOf("-A", if (arch == "x64") "x64" else "ARM64"))
            "mac" -> cmakeArgs.addAll(listOf("-DCMAKE_OSX_ARCHITECTURES=$arch"))
            "linux" -> cmakeArgs.addAll(listOf("-DCMAKE_SYSTEM_PROCESSOR=$arch"))
            "ios" -> cmakeArgs.addAll(listOf("-CMAKE_SYSTEM_NAME=iOS", "-CMAKE_OSX_ARCHITECTURES=$arch"))
            "android" -> {
                val ndkHome = System.getenv("ANDROID_NDK_HOME") ?: throw GradleException("ANDROID_NDK_HOME not set")
                val abi = when (arch) {
                    "arm64" -> "arm64-v8a"
                    "armv7" -> "armeabi-v7a"
                    "x64" -> "x86_64"
                    "x86" -> "x86"
                    else -> throw GradleException("Unsupported Android arch: $arch")
                }
                cmakeArgs.addAll(listOf(
                    "-DCMAKE_SYSTEM_NAME=Android",
                    "-DCMAKE_ANDROID_NDK=$ndkHome",
                    "-CMAKE_ANDROID_ARCH_ABI=$abi"
                ))
            }
            "emscripten" -> {
                val emsdkHome = System.getenv("EMSDK") ?: throw GradleException("EMSDK not set")
                cmakeArgs.addAll(listOf("-DCMAKE_TOOLCHAIN_FILE=$emsdkHome/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"))
            }
        }
        commandLine("cmake", *cmakeArgs.toTypedArray())
    }
}

// Build the libraries for a platform and architecture
fun createBuildTask(platform: String, arch: String) {
    val taskName = "build_${platform}_$arch"
    tasks.register<Exec>(taskName) {
        description = "Builds the library for $platform on $arch."
        group = "webgpu"
        dependsOn("generateCMake_${platform}_$arch")
        workingDir("${sourcePath}/build_${platform}_$arch")
        commandLine("cmake", "--build", ".", "--config", "Release", "--target",  "webgpu_dawn", "--parallel")
    }
}

// Copy the built libraries to the output directory
fun createCopyLibsTask(platform: String, arch: String) {
    val taskName = "copyLibs_${platform}_$arch"
    tasks.register<Copy>(taskName) {
        description = "Copies libraries for $platform $arch."
        group = "webgpu"
        dependsOn("build_${platform}_$arch")
        val buildType = buildTypes[platform] ?: throw GradleException("Unknown platform: $platform")
        val patterns = getLibPatterns(platform, buildType)
        from(fileTree("${sourcePath}/build_${platform}_$arch").matching { include(*patterns.toTypedArray()) }.files)
        into("$libsDir/${platform}_$arch")
        onlyIf {
            val files = fileTree("${sourcePath}/build_${platform}_$arch").matching { include(*patterns.toTypedArray()) }.files
            !files.isEmpty()
        }
    }
}

// Combine selected libraries for a platform and architecture
fun createCombineLibsTask(platform: String, arch: String) {
    val taskName = "combineLibs_${platform}_$arch"
    val libs = libsToCombine[platform] ?: listOf()
    if (platform == "emscripten" || libs.isEmpty()) {
        return // Skip combination for Emscripten or if no libs are specified
    }
    tasks.register<Exec>(taskName) {
        description = "Combines selected static libraries for $platform $arch into a single library."
        group = "webgpu"
        dependsOn("copyLibs_${platform}_$arch")
        workingDir("$libsDir/${platform}_$arch")
        val combinedLibName = if (platform == "windows") "combined.lib" else "combined.a"
        when (platform) {
            "windows" -> {
                commandLine("cmd", "/c", "lib /OUT:$combinedLibName ${libs.joinToString(" ")}")
            }
            "mac", "ios" -> {
                commandLine("libtool", "-static", "-o", combinedLibName, *libs.toTypedArray())
            }
            "linux", "android" -> {
                commandLine("sh", "-c", "mkdir temp && cd temp && for lib in ${libs.joinToString(" ")}; do ar x \$lib; done && ar rcs ../$combinedLibName *.o && cd .. && rm -rf temp")
            }
        }
    }
}

// Register tasks for all platforms and architectures
platforms.forEach { (platform, archs) ->
    archs.forEach { arch ->
        createGenerateCMakeTask(platform, arch)
        createBuildTask(platform, arch)
        createCopyLibsTask(platform, arch)
        createCombineLibsTask(platform, arch)
    }
}

// Main task to build, copy, and combine libraries for all platforms
tasks.register("buildAllLibs") {
    description = "Builds, copies, and combines libraries for all platforms and architectures."
    group = "webgpu"
    dependsOn(platforms.flatMap { (platform, archs) ->
        archs.mapNotNull { arch ->
            if (libsToCombine[platform]?.isNotEmpty() == true) "combineLibs_${platform}_$arch" else "copyLibs_${platform}_$arch"
        }
    })
}

val mainClassName = "Build"

dependencies {
    implementation(project(":webgpu:webgpu-base"))
    implementation("com.github.xpenatan.jParser:jParser-core:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-build:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-build-tool:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-teavm:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-cpp:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-idl:${LibExt.jParserVersion}")
}

tasks.register<JavaExec>("build_project_windows64") {
    group = "webgpu-build"
    description = "Generate native project"
    mainClass.set(mainClassName)
    args = mutableListOf("windows64")
    classpath = sourceSets["main"].runtimeClasspath
}