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

val libraryModes = listOf("shared", "static")

// Task to download and extract Dawn source
tasks.register<Download>("download_source") {
    group = "dawn"
    description = "Download WebGPU Dawn source"
    src("https://dawn.googlesource.com/dawn/+archive/refs/heads/chromium/7458.tar.gz")
    dest(File(zippedPath))
    onlyIfModified(true)
    doLast {
        try {
            File(sourcePath).mkdirs()
            exec {
                commandLine = listOf("tar", "-xzf", zippedPath, "-C", sourcePath)
                workingDir = buildDir
            }
            File(sourcePath).walk().forEach { file ->
                if (file.extension == "tar") {
                    println("Found nested tar file: ${file.name}")
                    exec {
                        commandLine = listOf("tar", "-xf", file.absolutePath, "-C", sourcePath)
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

tasks.register("patch_dawn") {
    group = "dawn"
    doLast {

        val bundleLibrariesFile = File(sourcePath, "/src/cmake/BundleLibraries.cmake")
        val cmakeListsFile = File(sourcePath, "/src/dawn/native/CMakeLists.txt")

        if (!bundleLibrariesFile.exists()) {
            throw IllegalStateException("BundleLibraries.cmake not found at ${bundleLibrariesFile.absolutePath}")
        }
        if (!cmakeListsFile.exists()) {
            throw IllegalStateException("CMakeLists.txt not found at ${cmakeListsFile.absolutePath}")
        }

        val bundleContent = bundleLibrariesFile.readText()
        val patchedBundleContent = bundleContent.replace(
            "add_library(\${output_target} SHARED",
            "add_library(\${output_target} STATIC"
        )

        if (patchedBundleContent != bundleContent) {
            bundleLibrariesFile.writeText(patchedBundleContent)
            println("Patched ${bundleLibrariesFile.absolutePath}: Changed SHARED to STATIC")
        }
        else {
            println("${bundleLibrariesFile.absolutePath} already patched or no change needed")
        }

        val cmakeContent = cmakeListsFile.readText()
        var patchedCmakeContent = cmakeContent
        patchedCmakeContent = patchedCmakeContent.replace(
            Regex("PUBLIC\\s+\"WGPU_SHARED_LIBRARY\"\\s+\"DAWN_NATIVE_SHARED_LIBRARY\""),
            ""
        )

//        install(TARGETS webgpu_dawn ARCHIVE DESTINATION lib )
//        install(DIRECTORY "${CMAKE_BINARY_DIR}/gen/include/dawn/" DESTINATION include/dawn FILES_MATCHING PATTERN "*.h")
//        install(DIRECTORY "${CMAKE_BINARY_DIR}/gen/include/webgpu/" DESTINATION include/webgpu FILES_MATCHING PATTERN "*.h")
//        install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/webgpu/" DESTINATION include/webgpu FILES_MATCHING PATTERN "*.h")

        val oldText = "dawn_install_target\\(webgpu_dawn\\s+HEADERS\\s+\\\$\\{webgpu_dawn_public_headers}\\)"
        val newText = "install(TARGETS webgpu_dawn ARCHIVE DESTINATION lib )\n" +
                "\t\tinstall(DIRECTORY \"\\$\\{CMAKE_BINARY_DIR\\}/gen/include/dawn/\" DESTINATION include/dawn FILES_MATCHING PATTERN \"*.h\")\n" +
                "\t\tinstall(DIRECTORY \"\\$\\{CMAKE_BINARY_DIR\\}/gen/include/webgpu/\" DESTINATION include/webgpu FILES_MATCHING PATTERN \"*.h\")\n" +
                "\t\tinstall(DIRECTORY \"\\$\\{CMAKE_SOURCE_DIR\\}/include/webgpu/\" DESTINATION include/webgpu FILES_MATCHING PATTERN \"*.h\")"
        patchedCmakeContent = patchedCmakeContent.replace(Regex(oldText), newText)

        println(patchedCmakeContent)
        if (patchedCmakeContent != cmakeContent) {
            cmakeListsFile.writeText(patchedCmakeContent)
            println("Patched ${cmakeListsFile.absolutePath}: Commented out shared library definitions")
        } else {
            println("${cmakeListsFile.absolutePath} already patched or no change needed")
        }
    }
}

//tasks.named("download_source") {
//    finalizedBy("patch_dawn")
//}

// Generate CMake build files for a platform and architecture
fun createGenerateCMakeTask(platform: String, arch: String, mode: String) {
    val modeUpper = mode.uppercase()
    val taskName = "generateCMake_${platform}_${arch}_${mode}"
    tasks.register<Exec>(taskName) {
        description = "Generates CMake build files for $platform $arch."
        group = "dawn"
        workingDir(sourcePath)
        val buildDir = "${sourcePath}/build_${platform}_$arch"
        val cmakeArgs = mutableListOf(
            "-B", buildDir,
            "-S", ".",
            "-DDAWN_FETCH_DEPENDENCIES=ON",
            "-DDAWN_ENABLE_INSTALL=ON",
            "-DCMAKE_BUILD_TYPE=Release",
            "-DCMAKE_CXX_STANDARD=17",
            "-DDAWN_BUILD_MONOLITHIC_LIBRARY=$modeUpper",
            "-DDAWN_ENABLE_D3D11=OFF",
            "-DBUILD_SHARED_LIBS=OFF",
            "-DDAWN_BUILD_SAMPLES=OFF",
            "-DDAWN_BUILD_TESTS=OFF",
            "-DTINT_BUILD_TESTS=OFF",
            "-DTINT_BUILD_SPV_READER=OFF",
            "-DTINT_BUILD_SPV_WRITER=OFF",
            "-DTINT_BUILD_CMD_TOOLS=OFF",
            "-DTINT_BUILD_IR_BINARY=OFF",
            "-DDAWN_ENABLE_SPIRV_VALIDATION=OFF",
            "-DDAWN_DXC_ENABLE_ASSERTS_IN_NDEBUG=OFF",
            "-DDAWN_USE_GLFW=OFF"
        )
        when (platform) {
            "windows" -> cmakeArgs.addAll(listOf("-A", if (arch == "x64") "x64" else "ARM64"))
            "mac" -> {
                val macArch = when(arch) {
                    "x64" -> "x86_64"
                    "arm64" -> "arm64"
                    else -> throw GradleException("Unsupported Mac arch: $arch")
                }
                cmakeArgs.addAll(listOf("-DCMAKE_OSX_ARCHITECTURES=$macArch"))
            }
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
                    "-DCMAKE_TOOLCHAIN_FILE=$ndkHome/build/cmake/android.toolchain.cmake",
                    "-DANDROID_PLATFORM=android-26",
                    "-DANDROID_ABI=$abi",
                    "-DDAWN_ENABLE_DESKTOP_GL=OFF",
                    "-DDAWN_ENABLE_OPENGLES=ON",
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
        group = "dawn"
        workingDir("${sourcePath}/build_${platform}_$arch")
        commandLine("cmake", "--build", ".", "--config", "Release", "--target",  "webgpu_dawn", "--parallel 2")
    }
    val taskNameInstall = taskName + "_install"
    tasks.register<Exec>(taskNameInstall) {
        group = "dawn"
        workingDir(sourcePath)
        val primaryTaskState = project.tasks.getByName(taskName).state
        val runCommand = if (primaryTaskState.executed) {
            primaryTaskState.failure == null
        }
        else {
            true
        }
        if(runCommand) {
            commandLine("cmake", "--install", taskName, "--prefix", "install/${taskName}")
        }
    }
    tasks.named(taskName) {
        finalizedBy(taskNameInstall)
    }
}

// Register tasks for all platforms and architectures
libraryModes.forEach { mode ->
    platforms.forEach { (platform, archs) ->
        archs.forEach { arch ->
            createGenerateCMakeTask(platform, arch, mode)
        }
    }
}

platforms.forEach { (platform, archs) ->
    archs.forEach { arch ->
        createBuildTask(platform, arch)
    }
}