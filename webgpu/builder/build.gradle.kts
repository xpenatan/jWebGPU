import com.github.xpenatan.jParser.builder.tool.JParserSymbolNameMode
import com.github.xpenatan.jParser.gradle.JParserTargetHooks
import com.github.xpenatan.jParser.gradle.JParserTargets
import com.github.xpenatan.jParser.idl.IDLRenaming
import java.io.File

plugins {
    id("java-library")
    id("com.github.xpenatan.jparser")
}

fun File.normalizedPath(): String {
    return absolutePath.replace('\\', '/')
}

val downloadBuildDir = file("../download/build")
val customSourceDir = file("src/main/cpp/custom")
val customSourceFile = File(customSourceDir, "jWebGPU.cpp")
val sourceIDLFile = file("src/main/cpp/jWebGPU.idl")

val wgpuWindowsDir = File(downloadBuildDir, "windows_x86_64")
val wgpuLinuxDir = File(downloadBuildDir, "linux_x86_64")
val wgpuMacDir = File(downloadBuildDir, "macos_x86_64")
val wgpuMacArmDir = File(downloadBuildDir, "macos_aarch64")
val dawnWindowsDir = File(downloadBuildDir, "dawn-x64")
val glfwIncludeDir = File(downloadBuildDir, "GLFW")
val emdawnDir = File(downloadBuildDir, "emdawnwebgpu_pkg")

val windowsSystemLibraries = listOf(
    "ws2_32.lib",
    "userenv.lib",
    "ntdll.lib",
    "opengl32.lib",
    "d3dcompiler.lib",
    "ole32.lib",
    "propsys.lib",
    "runtimeobject.lib",
    "user32.lib",
    "gdi32.lib",
    "oleaut32.lib"
)

fun JParserTargetHooks.configureWindowsWGPU() {
    includeDefaultSources.set(false)
    includeCustomSources.set(false)
    headerDir(File(wgpuWindowsDir, "include").normalizedPath())
    headerDir(glfwIncludeDir.normalizedPath())
    compileFlag("/MD")
    compileFlag("/EHsc")
    linkerFlag(File(wgpuWindowsDir, "lib/wgpu_native.lib").normalizedPath())
    windowsSystemLibraries.forEach(::linkerFlag)
}

fun JParserTargetHooks.configureWindowsDawn() {
    includeDefaultSources.set(false)
    includeCustomSources.set(false)
    headerDir(dawnWindowsDir.normalizedPath())
    headerDir(glfwIncludeDir.normalizedPath())
    compileFlag("/MD")
    compileFlag("/EHsc")
    linkerFlag(File(dawnWindowsDir, "webgpu_dawn.lib").normalizedPath())
    windowsSystemLibraries.forEach(::linkerFlag)
}

val androidArchives = mapOf(
    "x86" to "android_i686",
    "x86_64" to "android_x86_64",
    "armeabi_v7a" to "android_armv7",
    "arm64_v8a" to "android_aarch64"
)

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
}

jParser {
    libName.set("jWebGPU")
    idlName.set("jWebGPU")
    modulePrefix.set("")
    modulePath.set(file("..").absolutePath)
    moduleBuildSuffix.set("builder")
    moduleBaseSuffix.set("base")
    moduleCoreSuffix.set("core")
    moduleJNISuffix.set("shared/jni")
    moduleFFMSuffix.set("desktop/ffm")
    moduleWebSuffix.set("web/wasm")
    packageName.set("com.github.xpenatan.webgpu")
    cppSourcePath.set(customSourceDir.normalizedPath())
    jniSymbolNameMode.set(JParserSymbolNameMode.OBFUSCATED)
    ffmSymbolNameMode.set(JParserSymbolNameMode.OBFUSCATED)
    ffmLogMethod.set(true)
    jniCppStandard.set("c++17")
    ffmCppStandard.set("c++17")
    webCppStandard.set("c++17")
    idlRenaming(object : IDLRenaming {
        private val constructorNames: Set<String> by lazy {
            sourceIDLFile.readLines()
                .mapNotNull { line -> Regex("^\\s*interface\\s+(\\w+)").find(line)?.groupValues?.get(1) }
                .toSet()
        }

        override fun getIDLMethodName(methodName: String): String {
            if(methodName.isEmpty() || methodName in constructorNames || !methodName[0].isUpperCase()) {
                return methodName
            }
            return methodName.replaceFirstChar { it.lowercaseChar() }
        }
    })

    native {
        headerDir(customSourceDir.normalizedPath())
        cppInclude(customSourceFile.normalizedPath())
        includeDefaultSources.set(false)
        includeCustomSources.set(false)

        listOf(JParserTargets.WINDOWS64_JNI, JParserTargets.WINDOWS64_FFM).forEach { targetName ->
            target(targetName) {
                configureWindowsWGPU()
            }
            targetVariant(targetName, "wgpu") {
                configureWindowsWGPU()
            }
            targetVariant(targetName, "dawn") {
                configureWindowsDawn()
            }
        }

        listOf(JParserTargets.LINUX64_JNI, JParserTargets.LINUX64_FFM).forEach { targetName ->
            target(targetName) {
                includeDefaultSources.set(false)
                includeCustomSources.set(false)
                headerDir(File(wgpuLinuxDir, "include").normalizedPath())
                headerDir(glfwIncludeDir.normalizedPath())
                linkerFlag(File(wgpuLinuxDir, "lib/libwgpu_native.a").normalizedPath())
            }
        }

        mapOf(
            JParserTargets.MAC64_JNI to wgpuMacDir,
            JParserTargets.MAC64_FFM to wgpuMacDir,
            JParserTargets.MAC_ARM_JNI to wgpuMacArmDir,
            JParserTargets.MAC_ARM_FFM to wgpuMacArmDir
        ).forEach { (targetName, wgpuDir) ->
            target(targetName) {
                includeDefaultSources.set(false)
                includeCustomSources.set(false)
                headerDir(File(wgpuDir, "include").normalizedPath())
                headerDir(glfwIncludeDir.normalizedPath())
                compileFlag("-x")
                compileFlag("objective-c++")
                linkerFlag("-framework")
                linkerFlag("Metal")
                linkerFlag("-framework")
                linkerFlag("Foundation")
                linkerFlag("-framework")
                linkerFlag("Cocoa")
                linkerFlag("-framework")
                linkerFlag("QuartzCore")
                linkerFlag("-framework")
                linkerFlag("CoreFoundation")
                linkerFlag("-framework")
                linkerFlag("IOKit")
                linkerFlag(File(wgpuDir, "lib/libwgpu_native.a").normalizedPath())
            }
        }

        target(JParserTargets.ANDROID_JNI) {
            includeDefaultSources.set(false)
            includeCustomSources.set(false)
            linkerFlag("-landroid")
            linkerFlag("-llog")
            androidArchives.forEach { (abi, archiveDirName) ->
                val archiveDir = File(downloadBuildDir, archiveDirName)
                androidTarget(abi) {
                    headerDir(File(archiveDir, "include").normalizedPath())
                    linkerFlag(File(archiveDir, "lib/libwgpu_native.a").normalizedPath())
                }
            }
        }

        target(JParserTargets.WEB_WASM) {
            includeDefaultSources.set(false)
            includeCustomSources.set(false)
            headerDir(File(emdawnDir, "webgpu/include").normalizedPath())
            headerDir(File(emdawnDir, "webgpu_cpp/include").normalizedPath())
            forcedInclude(File(customSourceDir, "jWebGPU.h").normalizedPath())
            linkerFlag("--use-port=${File(emdawnDir, "emdawnwebgpu.port.py").normalizedPath()}")
            linkerFlag("--closure-args=--externs=${File(emdawnDir, "webgpu/src/webgpu-externs.js").normalizedPath()}")
            linkerFlag("-lc++abi")
            linkerFlag("-lc++")
            linkerFlag("-lc")
        }
    }
}
