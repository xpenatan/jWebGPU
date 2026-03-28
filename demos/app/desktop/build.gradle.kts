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

val jniToolchainVersion = providers.gradleProperty("jniToolchainVersion")
val requestedTaskNames = gradle.startParameter.taskNames
val requestedJniRunTask = requestedTaskNames.any { it.endsWith("webgpu_demo_app_run_desktop_jni") }
val requestedFfmRunTask = requestedTaskNames.any { it.endsWith("webgpu_demo_app_run_desktop_ffm") }
if(requestedJniRunTask && requestedFfmRunTask) {
    throw GradleException("Cannot infer bridge: both JNI and FFM run tasks were requested in the same command. Use one run task or -Pbridge.")
}
val inferredBridge = if(requestedFfmRunTask) "ffm" else "jni"
val selectedBridge = providers.gradleProperty("bridge").orElse(inferredBridge).get()
val backendDesktopConfiguration = if(selectedBridge.equals("ffm", ignoreCase = true)) "desktopFfm" else "desktopJni"
val backendDesktopArtifact = if(selectedBridge.equals("ffm", ignoreCase = true)) "backend-desktop-ffm" else "backend-desktop-jni"
val ffmRuntime by configurations.creating {
    isCanBeConsumed = false
    isCanBeResolved = true
    attributes {
        attribute(TargetJvmVersion.TARGET_JVM_VERSION_ATTRIBUTE, 24)
    }
}

dependencies {
    implementation(project(":demos:app:core"))
    if(LibExt.exampleUseRepoLibs) {
        implementation("${LibExt.groupId}:$backendDesktopArtifact:${LibExt.libVersion}")
        if(selectedBridge.equals("ffm", ignoreCase = true)) {
            ffmRuntime("${LibExt.groupId}:$backendDesktopArtifact:${LibExt.libVersion}")
        }
    }
    else {
        implementation(project(":demos:backend:desktop"))
        if(selectedBridge.equals("ffm", ignoreCase = true)) {
            ffmRuntime(project(mapOf("path" to ":demos:backend:desktop", "configuration" to backendDesktopConfiguration)))
        }
        else {
            runtimeOnly(project(mapOf("path" to ":demos:backend:desktop", "configuration" to backendDesktopConfiguration)))
        }
    }
}

fun JavaExec.configureDesktopJvm() {
    if(DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX) {
        jvmArgs("-XstartOnFirstThread")
    }
}

fun JavaExec.useJniLauncher() {
    classpath = sourceSets["main"].runtimeClasspath
    if(jniToolchainVersion.isPresent) {
        javaLauncher.set(javaToolchains.launcherFor {
            languageVersion.set(JavaLanguageVersion.of(jniToolchainVersion.get().toInt()))
        })
    }
    configureDesktopJvm()
}

fun JavaExec.useFfmLauncher() {
    classpath = sourceSets["main"].runtimeClasspath + ffmRuntime
    jvmArgs("--enable-native-access=ALL-UNNAMED")
    configureDesktopJvm()
}

val mainClassName = "com.github.xpenatan.webgpu.demo.app.Main"

tasks.register<JavaExec>("webgpu_demo_app_run_desktop_jni") {
    group = "demos"
    description = "Run desktop demo using JNI bridge"
    mainClass.set(mainClassName)
    useJniLauncher()
}

tasks.register<JavaExec>("webgpu_demo_app_run_desktop_ffm") {
    group = "demos"
    description = "Run desktop demo using FFM bridge"
    mainClass.set(mainClassName)
    useFfmLauncher()
}

tasks.register("webgpu_demo_app_run_desktop") {
    group = "demos"
    description = "Run desktop demo using -Pbridge=jni|ffm (default jni)"
    dependsOn(if(selectedBridge.equals("ffm", ignoreCase = true)) "webgpu_demo_app_run_desktop_ffm" else "webgpu_demo_app_run_desktop_jni")
}

