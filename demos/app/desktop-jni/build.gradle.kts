import org.gradle.api.GradleException
import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform

plugins {
    id("java")
    id("java-library")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
}

dependencies {
    implementation(project(":demos:app:core"))
    implementation(project(":demos:backend:desktop"))

    if(LibExt.exampleUseRepoLibs) {
        api("com.github.xpenatan.jWebGPU:webgpu-jni:-SNAPSHOT")
        api("com.github.xpenatan.jWebGPU:webgpu-desktop-jni:-SNAPSHOT")
    }
    else {
        api(project(":webgpu:shared:jni"))
        api(project(":webgpu:desktop:jni"))
    }
}

val mainClassName = "com.github.xpenatan.webgpu.demo.app.Main"

enum class DemoBackend(val id: String, val systemValue: String) {
    WGPU("wgpu", "WGPU"),
    DAWN("dawn", "DAWN"),
}

fun currentDesktopPlatformName(): String {
    val os = DefaultNativePlatform.getCurrentOperatingSystem()
    val archName = DefaultNativePlatform.getCurrentArchitecture().name.lowercase()
    return when {
        os.isWindows -> "windows_x64"
        os.isLinux -> "linux_x64"
        os.isMacOsX && (archName.contains("aarch64") || archName.contains("arm64")) -> "mac_arm64"
        os.isMacOsX -> "mac_x64"
        else -> throw GradleException("Unsupported desktop platform: ${os.name} $archName")
    }
}

val currentDesktopPlatform = currentDesktopPlatformName()

fun registerDesktopRunTask(taskName: String, backend: DemoBackend, descriptionSuffix: String) {
    tasks.register<JavaExec>(taskName) {
        group = "demos"
        description = "Run desktop demo using JNI bridge with $descriptionSuffix backend"
        mainClass.set(mainClassName)
        classpath = sourceSets["main"].runtimeClasspath
        systemProperty("jwebgpu.bridge", "JNI")
        systemProperty("jwebgpu.backend", backend.systemValue)
        systemProperty("studio.capture", "false")
        systemProperty("studio.capture.shadow", "false")
        systemProperty("studio.capture.shadow.diagnostics", "false")
        systemProperty("studio.capture.max", "1")
        systemProperty("studio.capture.path", "local-logs/screenshots")

        if(LibExt.exampleUseRepoLibs) {
            val artifactId = "webgpu-desktop-jni-${backend.id}_$currentDesktopPlatform"
            classpath(configurations.detachedConfiguration(dependencies.create("${LibExt.groupId}:$artifactId:${LibExt.libVersion}")))
        }
        else {
            val nativeJarTask = project(":webgpu:desktop:jni").tasks.named("nativeJar_${backend.id}_$currentDesktopPlatform")
            dependsOn(nativeJarTask)
            classpath(files(nativeJarTask))
        }

        if(DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX) {
            jvmArgs("-XstartOnFirstThread")
        }
    }
}

registerDesktopRunTask("webgpu_demo_app_desktop_jni_wgpu_run", DemoBackend.WGPU, "WGPU")
registerDesktopRunTask("webgpu_demo_app_desktop_jni_dawn_run", DemoBackend.DAWN, "Dawn")
