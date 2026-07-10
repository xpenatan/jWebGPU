import java.net.URL
import java.nio.file.Files
import java.nio.file.StandardCopyOption

plugins {
    id("java")
}

val WGPU_VERSION = "29.0.1.1"
val dawnNativesVersion = providers.gradleProperty("dawnNativesVersion").orElse("7458").get()
val dawnNativesTag = "dawn-$dawnNativesVersion"
val dawnNativesReleaseUrl = "https://github.com/xpenatan/dawn-natives/releases/download/$dawnNativesTag"

val buildDir = layout.buildDirectory.get().asFile

fun downloadZip(url: String, zipFile: File) {
    println("URL: $url")
    zipFile.parentFile.mkdirs()
    URL(url).openStream().use { input ->
        Files.copy(input, zipFile.toPath(), StandardCopyOption.REPLACE_EXISTING)
    }
}

fun extractZip(zipFile: File, outputDir: File) {
    project.delete(outputDir)
    outputDir.mkdirs()
    project.copy {
        from(project.zipTree(zipFile))
        into(outputDir)
    }
}

fun registerDownloadTask(platform: String, os: String, arch: String) {
    val taskName = "webgpu_download_${platform}_wgpu"
    tasks.register(taskName) {
        group = "webgpu"
        description = "Download wgpu-native binaries for $platform"
        doLast {
            val zipName = "wgpu-$os-$arch.zip"
            val url = "https://github.com/gfx-rs/wgpu-native/releases/download/v$WGPU_VERSION/$zipName"
            val tmpDir = buildDir.resolve("tmp")
            val zipFile = tmpDir.resolve("wgpu-$platform.zip")
            val nativesDir = buildDir.resolve(platform)
            downloadZip(url, zipFile)
            extractZip(zipFile, nativesDir)
        }
    }
}

fun registerDawnNativesDownloadTask(platform: String, packageName: String) {
    tasks.register("webgpu_download_${platform}_dawn") {
        group = "webgpu"
        description = "Download $packageName from xpenatan/dawn-natives $dawnNativesTag"
        doLast {
            val url = "$dawnNativesReleaseUrl/$packageName.zip"
            val zipFile = buildDir.resolve("tmp/$packageName.zip")
            val nativesDir = buildDir.resolve(packageName)
            downloadZip(url, zipFile)
            extractZip(zipFile, nativesDir)
        }
    }
}

tasks.register("webgpu_download_emdawnwebgpu") {
    group = "webgpu"
    description = "Download Emdawnwebgpu from xpenatan/dawn-natives $dawnNativesTag"
    doLast {
        val packageName = "dawn-natives-web-emdawnwebgpu"
        val url = "$dawnNativesReleaseUrl/$packageName.zip"
        val zipFile = buildDir.resolve("tmp/$packageName.zip")
        val nativesDir = buildDir.resolve(packageName)
        downloadZip(url, zipFile)
        extractZip(zipFile, nativesDir)
    }
}

registerDawnNativesDownloadTask("windows_x86_64", "dawn-natives-windows-x64-msvc")
registerDawnNativesDownloadTask("linux_x86_64", "dawn-natives-linux-x64-gcc")
registerDawnNativesDownloadTask("macos_x86_64", "dawn-natives-macos-x64")
registerDawnNativesDownloadTask("macos_aarch64", "dawn-natives-macos-arm64")
registerDawnNativesDownloadTask("android_i686", "dawn-natives-android-x86")
registerDawnNativesDownloadTask("android_x86_64", "dawn-natives-android-x86_64")
registerDawnNativesDownloadTask("android_armv7", "dawn-natives-android-armeabi-v7a")
registerDawnNativesDownloadTask("android_aarch64", "dawn-natives-android-arm64-v8a")

tasks.register("webgpu_download_glfw_windows") {
    group = "webgpu"
    description = "Download GLFW 3.4 binaries for Windows and extract only the include folder contents to glfw-windows"
    doLast {
        val glfwVersion = "3.4"
        val zipName = "glfw-$glfwVersion.bin.WIN64.zip"
        val url = "https://github.com/glfw/glfw/releases/download/$glfwVersion/$zipName"
        println("URL: $url")
        val tmpDir = buildDir.resolve("tmp")
        tmpDir.mkdirs()
        val zipFile = tmpDir.resolve("glfw-windows.zip")
        // Download the file
        URL(url).openStream().use { input ->
            Files.copy(input, zipFile.toPath(), StandardCopyOption.REPLACE_EXISTING)
        }
        val nativesDir = buildDir.resolve("")
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
