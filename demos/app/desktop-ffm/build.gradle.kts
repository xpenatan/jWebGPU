import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform

plugins {
    id("java")
    id("java-library")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaFFMTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaFFMTarget)
}

dependencies {
    implementation(project(":demos:app:core"))
    implementation(project(":demos:backend:desktop"))

    if(LibExt.exampleUseRepoLibs) {
        api("com.github.xpenatan.jWebGPU:webgpu-ffm:-SNAPSHOT")
    }
    else {
        api(project(":webgpu:desktop:ffm"))
    }
}

val mainClassName = "com.github.xpenatan.webgpu.demo.app.Main"
val jwebgpuBackend = providers.gradleProperty("jwebgpuBackend").orElse("wgpu")

tasks.register<JavaExec>("webgpu_demo_app_run_desktop_ffm") {
    group = "demos"
    description = "Run desktop demo using FFM bridge"
    mainClass.set(mainClassName)
    classpath = sourceSets["main"].runtimeClasspath
    systemProperty("jwebgpu.backend", jwebgpuBackend.get())
    jvmArgs("--enable-native-access=ALL-UNNAMED")
    systemProperty("studio.capture", "false")
    systemProperty("studio.capture.shadow", "false")
    systemProperty("studio.capture.shadow.diagnostics", "false")
    systemProperty("studio.capture.max", "1")
    systemProperty("studio.capture.path", "local-logs/screenshots")

    if(DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX) {
        jvmArgs("-XstartOnFirstThread")
    }
}
