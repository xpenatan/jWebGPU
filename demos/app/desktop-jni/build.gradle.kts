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
val jwebgpuBackend = providers.gradleProperty("jwebgpuBackend").orElse("wgpu")

tasks.register<JavaExec>("webgpu_demo_app_run_desktop_jni") {
    group = "demos"
    description = "Run desktop demo using JNI bridge"
    mainClass.set(mainClassName)
    classpath = sourceSets["main"].runtimeClasspath
    systemProperty("jwebgpu.backend", jwebgpuBackend.get())
    systemProperty("studio.capture", "false")
    systemProperty("studio.capture.shadow", "false")
    systemProperty("studio.capture.shadow.diagnostics", "false")
    systemProperty("studio.capture.max", "1")
    systemProperty("studio.capture.path", "local-logs/screenshots")

    if(DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX) {
        jvmArgs("-XstartOnFirstThread")
    }
}
