import java.net.URL
import java.nio.file.Files
import java.nio.file.StandardCopyOption

plugins {
    id("java")
}

val WGPU_VERSION = "25.0.2.1"
val emdawnVersion = "v20250629.124903"
val buildDawnVersion = "2025-07-27"

fun registerDownloadTask(platform: String, os: String, arch: String) {
    val taskName = "download_$platform"
    tasks.register(taskName) {
        group = "wgpu"
        description = "Download wgpu-native binaries for $platform"
        doLast {
            val zipName = "wgpu-$os-$arch.zip"
            val url = "https://github.com/gfx-rs/wgpu-native/releases/download/v$WGPU_VERSION/$zipName"
            println("URL: $url")
            val tmpDir = project.buildDir.resolve("tmp")
            tmpDir.mkdirs()
            val zipFile = tmpDir.resolve("wgpu-$platform.zip")
            // Download the file
            URL(url).openStream().use { input ->
                Files.copy(input, zipFile.toPath(), StandardCopyOption.REPLACE_EXISTING)
            }
            // Extract to build/bin_release64
            val nativesDir = project.buildDir.resolve(platform)
            nativesDir.mkdirs()
            project.copy {
                from(project.zipTree(zipFile))
                into(nativesDir)
            }
        }
    }
}

tasks.register("download_emdawnwebgpu") {
    group = "wgpu"
    description = "Download emdawnwebgpu headers"
    doLast {
        var url = "https://github.com/google/dawn/releases/download/${emdawnVersion}/emdawnwebgpu_pkg-${emdawnVersion}.zip"
        println("URL: $url")
        val tmpDir = project.buildDir.resolve("tmp")
        tmpDir.mkdirs()
        val zipFile = tmpDir.resolve("emdawnwebgpu.zip")
        // Download the file
        URL(url).openStream().use { input ->
            Files.copy(input, zipFile.toPath(), StandardCopyOption.REPLACE_EXISTING)
        }
        // Extract to build/bin_release64
        val nativesDir = project.buildDir.resolve("")
        nativesDir.mkdirs()
        project.copy {
            from(project.zipTree(zipFile))
            into(nativesDir)
        }
    }
}

tasks.register("download_windows_x86_64_dawn") {
    group = "wgpu"
    description = "Download windows64 dawn headers"
    doLast {
        var url = "https://github.com/mmozeiko/build-dawn/releases/download/${buildDawnVersion}/dawn-x64-${buildDawnVersion}.zip"
        println("URL: $url")
        val tmpDir = project.buildDir.resolve("tmp")
        tmpDir.mkdirs()
        val zipFile = tmpDir.resolve("win64dawn.zip")
        // Download the file
        URL(url).openStream().use { input ->
            Files.copy(input, zipFile.toPath(), StandardCopyOption.REPLACE_EXISTING)
        }
        // Extract to build/bin_release64
        val nativesDir = project.buildDir.resolve("")
        nativesDir.mkdirs()
        project.copy {
            from(project.zipTree(zipFile))
            into(nativesDir)
        }
    }
}

tasks.register("download_glfw_windows") {
    group = "wgpu"
    description = "Download GLFW 3.4 binaries for Windows and extract only the include folder contents to glfw-windows"
    doLast {
        val glfwVersion = "3.4"
        val zipName = "glfw-$glfwVersion.bin.WIN64.zip"
        val url = "https://github.com/glfw/glfw/releases/download/$glfwVersion/$zipName"
        println("URL: $url")
        val tmpDir = project.buildDir.resolve("tmp")
        tmpDir.mkdirs()
        val zipFile = tmpDir.resolve("glfw-windows.zip")
        // Download the file
        URL(url).openStream().use { input ->
            Files.copy(input, zipFile.toPath(), StandardCopyOption.REPLACE_EXISTING)
        }
        val nativesDir = project.buildDir.resolve("")
        nativesDir.mkdirs()
        project.copy {
            from(project.zipTree(zipFile)) {
                include("*/include/GLFW/**")
                // Remove the leading directory (e.g., glfw-3.4.bin.WIN64/include/GLFW/...)
                eachFile {
                    val segments = relativePath.segments
                    // segments[0] = glfw-3.4.bin.WIN64, segments[1] = include, segments[2] = GLFW, ...
                    if (segments.size >= 3 && segments[1] == "include" && segments[2] == "GLFW") {
                        // Remove the first two segments (the zip's root folder and "include")
                        relativePath = org.gradle.api.file.RelativePath(true, *segments.copyOfRange(2, segments.size))
                    }
                }
                includeEmptyDirs = false
            }
            into(nativesDir)
        }
    }
}

val platforms = mapOf(
    "windows_x86_64_debug" to Pair("windows", "x86_64-msvc-debug"),
    "windows_x86_64" to Pair("windows", "x86_64-msvc-release"),
    "windows_aarch64" to Pair("windows", "aarch64-msvc-release"),
    "linux_x86_64" to Pair("linux", "x86_64-release"),
    "linux_aarch64" to Pair("linux", "aarch64-release"),
    "macos_x86_64" to Pair("macos", "x86_64-release"),
    "macos_aarch64" to Pair("macos", "aarch64-release"),
    "android_x86_64" to Pair("android", "x86_64-release"),
    "android_aarch64" to Pair("android", "aarch64-release"),
    "android_i686" to Pair("android", "i686-release"),
    "android_armv7" to Pair("android", "armv7-release")
)

platforms.forEach { (platform, pair) ->
    val (os, arch) = pair
    registerDownloadTask(platform, os, arch)
}