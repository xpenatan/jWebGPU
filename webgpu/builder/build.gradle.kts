import com.github.xpenatan.jParser.builder.tool.JParserSymbolNameMode
import com.github.xpenatan.jParser.gradle.JParserNativeTargetVariantHooks
import com.github.xpenatan.jParser.gradle.JParserTargetHooks
import com.github.xpenatan.jParser.gradle.JParserTeaVMCConsumerHooks
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
val dawnWindowsDir = File(downloadBuildDir, "dawn-natives-windows-x64-msvc")
val dawnLinuxDir = File(downloadBuildDir, "dawn-natives-linux-x64-gcc")
val dawnMacDir = File(downloadBuildDir, "dawn-natives-macos-x64")
val dawnMacArmDir = File(downloadBuildDir, "dawn-natives-macos-arm64")
val glfwIncludeDir = File(downloadBuildDir, "GLFW")
val emdawnDir = File(downloadBuildDir, "dawn-natives-web-emdawnwebgpu")

val windowsSystemLibraries = listOf(
    "onecore_apiset.lib",
    "dxguid.lib",
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

fun JParserTeaVMCConsumerHooks.configureWGPUConsumer(staticLibraryFile: String) {
    selectorResource("include/webgpu/wgpu.h")
    headerDir("include")
    staticLibrary("deps/$staticLibraryFile", "JPARSER_JWEBGPU_WGPU_NATIVE_LIBRARY")
}

fun JParserTeaVMCConsumerHooks.configureDawnConsumer(staticLibraryFile: String) {
    selectorResource("include/dawn/webgpu.h")
    headerDir("include")
    compileDefinition("JWEBGPU_DAWN=1")
    staticLibrary("deps/$staticLibraryFile", "JPARSER_JWEBGPU_DAWN_NATIVE_LIBRARY")
}

fun JParserTeaVMCConsumerHooks.linkWindowsBackendRequirements() {
    windowsSystemLibraries.forEach(::staticLinkLibrary)
}

fun JParserTeaVMCConsumerHooks.linkLinuxBackendRequirements() {
    staticLinkLibrary("pthread")
    staticLinkLibrary("\${CMAKE_DL_LIBS}")
}

fun JParserTeaVMCConsumerHooks.linkMacBackendRequirements(frameworks: List<String>) {
    frameworks.forEach { framework ->
        staticLinkerFlag("SHELL:-framework $framework")
    }
}

fun JParserTargetHooks.configureWindowsWGPU() {
    includeDefaultSources.set(false)
    includeCustomSources.set(false)
    headerDir(File(wgpuWindowsDir, "include").normalizedPath())
    headerDir(glfwIncludeDir.normalizedPath())
    compileFlag("/EHsc")
    linkerFlag(File(wgpuWindowsDir, "lib/wgpu_native.lib").normalizedPath())
    windowsSystemLibraries.forEach(::linkerFlag)
}

fun JParserTargetHooks.configureWindowsDawn() {
    includeDefaultSources.set(false)
    includeCustomSources.set(false)
    headerDir(File(dawnWindowsDir, "include").normalizedPath())
    headerDir(glfwIncludeDir.normalizedPath())
    compileFlag("/EHsc")
    compileFlag("/Zc:preprocessor")
    compileFlag("/DJWEBGPU_DAWN")
    linkerFlag(File(dawnWindowsDir, "lib/webgpu_dawn.lib").normalizedPath())
    windowsSystemLibraries.forEach(::linkerFlag)
}

fun JParserTargetHooks.configureLinuxWGPU() {
    includeDefaultSources.set(false)
    includeCustomSources.set(false)
    headerDir(File(wgpuLinuxDir, "include").normalizedPath())
    headerDir(glfwIncludeDir.normalizedPath())
    linkerFlag(File(wgpuLinuxDir, "lib/libwgpu_native.a").normalizedPath())
}

fun JParserTargetHooks.configureLinuxDawn() {
    includeDefaultSources.set(false)
    includeCustomSources.set(false)
    headerDir(File(dawnLinuxDir, "include").normalizedPath())
    headerDir(glfwIncludeDir.normalizedPath())
    compileFlag("-DJWEBGPU_DAWN")
    linkerFlag(File(dawnLinuxDir, "lib/libwebgpu_dawn.a").normalizedPath())
    linkerFlag("-pthread")
    linkerFlag("-ldl")
}

fun JParserTargetHooks.configureMacWGPU(wgpuDir: File) {
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

fun JParserTargetHooks.configureMacDawn(dawnDir: File) {
    includeDefaultSources.set(false)
    includeCustomSources.set(false)
    headerDir(File(dawnDir, "include").normalizedPath())
    headerDir(glfwIncludeDir.normalizedPath())
    compileFlag("-x")
    compileFlag("objective-c++")
    compileFlag("-DJWEBGPU_DAWN")
    linkerFlag(File(dawnDir, "lib/libwebgpu_dawn.a").normalizedPath())
    listOf("Cocoa", "IOKit", "Foundation", "IOSurface", "QuartzCore", "Metal").forEach { framework ->
        linkerFlag("-framework")
        linkerFlag(framework)
    }
}

val androidWGPUArchives = mapOf(
    "x86" to "android_i686",
    "x86_64" to "android_x86_64",
    "armeabi_v7a" to "android_armv7",
    "arm64_v8a" to "android_aarch64"
)

val androidDawnArchives = mapOf(
    "x86" to "dawn-natives-android-x86",
    "x86_64" to "dawn-natives-android-x86_64",
    "armeabi_v7a" to "dawn-natives-android-armeabi-v7a",
    "arm64_v8a" to "dawn-natives-android-arm64-v8a"
)

fun JParserNativeTargetVariantHooks.configureAndroidWGPU() {
    includeDefaultSources.set(false)
    includeCustomSources.set(false)
    androidWGPUArchives.forEach { (abi, archiveDirName) ->
        val archiveDir = File(downloadBuildDir, archiveDirName)
        androidTarget(abi) {
            headerDir(File(archiveDir, "include").normalizedPath())
            linkerFlag(File(archiveDir, "lib/libwgpu_native.a").normalizedPath())
            linkerFlag("-landroid")
            linkerFlag("-llog")
        }
    }
}

fun JParserNativeTargetVariantHooks.configureAndroidDawn() {
    includeDefaultSources.set(false)
    includeCustomSources.set(false)
    compileFlag("-DJWEBGPU_DAWN")
    androidDawnArchives.forEach { (abi, archiveDirName) ->
        val archiveDir = File(downloadBuildDir, archiveDirName)
        androidTarget(abi) {
            headerDir(File(archiveDir, "include").normalizedPath())
            linkerFlag(File(archiveDir, "lib/libwebgpu_dawn.a").normalizedPath())
            linkerFlag("-pthread")
            linkerFlag("-ldl")
            linkerFlag("-landroid")
            linkerFlag("-llog")
        }
    }
}

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
    moduleCSuffix.set("shared/c")
    moduleFFMSuffix.set("desktop/ffm")
    moduleWebSuffix.set("web/wasm")
    packageName.set("com.github.xpenatan.webgpu")
    cppSourcePath.set(customSourceDir.normalizedPath())
    jniSymbolNameMode.set(JParserSymbolNameMode.OBFUSCATED)
    ffmSymbolNameMode.set(JParserSymbolNameMode.OBFUSCATED)
    teaVMCSymbolNameMode.set(JParserSymbolNameMode.OBFUSCATED)
    ffmLogMethod.set(true)
    jniCppStandard.set("c++17")
    ffmCppStandard.set("c++17")
    teaVMCCppStandard.set("c++17")
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
                compileFlag("/MD")
            }
            targetVariant(targetName, "wgpu") {
                configureWindowsWGPU()
                compileFlag("/MD")
            }
            targetVariant(targetName, "dawn") {
                configureWindowsDawn()
                compileFlag("/MD")
            }
        }

        targetVariant(JParserTargets.WINDOWS64_TEAVM_C, "wgpu") {
            configureWindowsWGPU()
            // Raw compiler policy stays at the target declaration. The
            // published Windows bridge payload is currently the MD variant.
            compileFlag("/MD")
            forcedInclude(File(customSourceDir, "jWebGPU.h"))
            consumer {
                configureWGPUConsumer("wgpu_native.lib")
                linkWindowsBackendRequirements()
            }
        }
        targetVariant(JParserTargets.WINDOWS64_TEAVM_C, "dawn") {
            configureWindowsDawn()
            // The downloaded Dawn archive and published bridge use the dynamic CRT.
            compileFlag("/MD")
            forcedInclude(File(customSourceDir, "jWebGPU.h"))
            consumer {
                configureDawnConsumer("webgpu_dawn.lib")
                linkWindowsBackendRequirements()
            }
        }

        listOf(JParserTargets.LINUX64_JNI, JParserTargets.LINUX64_FFM).forEach { targetName ->
            targetVariant(targetName, "wgpu") {
                configureLinuxWGPU()
            }
            targetVariant(targetName, "dawn") {
                configureLinuxDawn()
            }
        }
        targetVariant(JParserTargets.LINUX64_TEAVM_C, "wgpu") {
            configureLinuxWGPU()
            forcedInclude(File(customSourceDir, "jWebGPU.h"))
            consumer {
                configureWGPUConsumer("libwgpu_native.a")
                linkLinuxBackendRequirements()
            }
        }
        targetVariant(JParserTargets.LINUX64_TEAVM_C, "dawn") {
            configureLinuxDawn()
            forcedInclude(File(customSourceDir, "jWebGPU.h"))
            consumer {
                configureDawnConsumer("libwebgpu_dawn.a")
                linkLinuxBackendRequirements()
            }
        }

        mapOf(
            JParserTargets.MAC64_JNI to wgpuMacDir,
            JParserTargets.MAC64_FFM to wgpuMacDir,
            JParserTargets.MAC64_TEAVM_C to wgpuMacDir,
            JParserTargets.MAC_ARM_JNI to wgpuMacArmDir,
            JParserTargets.MAC_ARM_FFM to wgpuMacArmDir,
            JParserTargets.MAC_ARM_TEAVM_C to wgpuMacArmDir
        ).forEach { (targetName, wgpuDir) ->
            targetVariant(targetName, "wgpu") {
                configureMacWGPU(wgpuDir)
                if(targetName == JParserTargets.MAC64_TEAVM_C || targetName == JParserTargets.MAC_ARM_TEAVM_C) {
                    forcedInclude(File(customSourceDir, "jWebGPU.h"))
                    consumer {
                        configureWGPUConsumer("libwgpu_native.a")
                        linkMacBackendRequirements(listOf(
                            "Metal",
                            "Foundation",
                            "Cocoa",
                            "QuartzCore",
                            "CoreFoundation",
                            "IOKit"
                        ))
                    }
                }
            }
        }

        mapOf(
            JParserTargets.MAC64_JNI to dawnMacDir,
            JParserTargets.MAC64_FFM to dawnMacDir,
            JParserTargets.MAC64_TEAVM_C to dawnMacDir,
            JParserTargets.MAC_ARM_JNI to dawnMacArmDir,
            JParserTargets.MAC_ARM_FFM to dawnMacArmDir,
            JParserTargets.MAC_ARM_TEAVM_C to dawnMacArmDir
        ).forEach { (targetName, dawnDir) ->
            targetVariant(targetName, "dawn") {
                configureMacDawn(dawnDir)
                if(targetName == JParserTargets.MAC64_TEAVM_C || targetName == JParserTargets.MAC_ARM_TEAVM_C) {
                    forcedInclude(File(customSourceDir, "jWebGPU.h"))
                    consumer {
                        configureDawnConsumer("libwebgpu_dawn.a")
                        linkMacBackendRequirements(listOf(
                            "Cocoa",
                            "IOKit",
                            "Foundation",
                            "IOSurface",
                            "QuartzCore",
                            "Metal"
                        ))
                    }
                }
            }
        }

        targetVariant(JParserTargets.ANDROID_JNI, "wgpu") {
            configureAndroidWGPU()
        }

        targetVariant(JParserTargets.ANDROID_JNI, "dawn") {
            configureAndroidDawn()
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
