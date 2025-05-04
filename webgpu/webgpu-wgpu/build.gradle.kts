import java.net.URL
import java.nio.file.Files
import java.nio.file.StandardCopyOption

val WGPU_VERSION = "24.0.3.1"

fun registerDownloadTask(platform: String, os: String, arch: String) {
    val taskName = "download_$platform"
    tasks.register(taskName) {
        group = "wgpu"
        description = "Download wgpu-native binaries for $platform"
        doLast {
            val zipName = "wgpu-$os-$arch-release.zip"
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

val platforms = mapOf(
    "emscripten" to Pair("windows", "x86_64-msvc"), // Used to obtain headers only
    "windows_x86_64" to Pair("windows", "x86_64-msvc"),
    "windows_aarch64" to Pair("windows", "aarch64-msvc"),
    "linux_x86_64" to Pair("linux", "x86_64"),
    "linux_aarch64" to Pair("linux", "aarch64"),
    "macos_x86_64" to Pair("macos", "x86_64"),
    "macos_aarch64" to Pair("macos", "aarch64"),
    "android_x86_64" to Pair("android", "x86_64"),
    "android_aarch64" to Pair("android", "aarch64"),
    "android_armv7" to Pair("android", "armv7")
)

platforms.forEach { (platform, pair) ->
    val (os, arch) = pair
    registerDownloadTask(platform, os, arch)
}