import org.gradle.jvm.toolchain.JavaLanguageVersion
import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform

plugins {
    id("java")
    id("java-library")
}

java {
    // Keep launcher Java 8-compatible; bridge/runtime is selected by task.
    sourceCompatibility = JavaVersion.toVersion(LibExt.java8Target)
    targetCompatibility = JavaVersion.toVersion(LibExt.java8Target)
}

val jniRuntime by configurations.creating
val ffmRuntime by configurations.creating
val jniToolchainVersion = providers.gradleProperty("jniToolchainVersion")

dependencies {
    implementation(project(":demos:app:core"))
    implementation(project(":demos:backend:desktop"))

    jniRuntime(project(":webgpu:webgpu-core"))
    jniRuntime(project(":webgpu:webgpu-desktop-jni"))
    jniRuntime("com.github.xpenatan.jParser:idl-helper-core:${LibExt.jParserVersion}")
    ffmRuntime(project(":webgpu:webgpu-desktop-ffm"))
}

fun JavaExec.configureDesktopJvm() {
    if(DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX) {
        jvmArgs("-XstartOnFirstThread")
    }
}

fun JavaExec.useJniRuntime() {
    classpath = sourceSets["main"].runtimeClasspath + jniRuntime
    if(jniToolchainVersion.isPresent) {
        javaLauncher.set(javaToolchains.launcherFor {
            languageVersion.set(JavaLanguageVersion.of(jniToolchainVersion.get().toInt()))
        })
    }
    configureDesktopJvm()
}

fun JavaExec.useFfmRuntime() {
    classpath = sourceSets["main"].runtimeClasspath + ffmRuntime
    javaLauncher.set(javaToolchains.launcherFor {
        languageVersion.set(JavaLanguageVersion.of(24))
    })
    jvmArgs("--enable-native-access=ALL-UNNAMED")
    configureDesktopJvm()
}

val mainClassName = "com.github.xpenatan.webgpu.demo.app.Main"

tasks.register<JavaExec>("webgpu_demo_app_run_desktop_jni") {
    group = "demos"
    description = "Run desktop demo using JNI bridge"
    mainClass.set(mainClassName)
    useJniRuntime()
}

tasks.register<JavaExec>("webgpu_demo_app_run_desktop_ffm") {
    group = "demos"
    description = "Run desktop demo using FFM bridge"
    mainClass.set(mainClassName)
    useFfmRuntime()
}

val selectedBridge = providers.gradleProperty("bridge").orElse("jni")

tasks.register("webgpu_demo_app_run_desktop") {
    group = "demos"
    description = "Run desktop demo using -Pbridge=jni|ffm (default jni)"
    dependsOn(selectedBridge.map {
        if(it.equals("ffm", ignoreCase = true)) {
            "webgpu_demo_app_run_desktop_ffm"
        }
        else {
            "webgpu_demo_app_run_desktop_jni"
        }
    })
}

