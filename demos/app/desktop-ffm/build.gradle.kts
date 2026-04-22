import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform

plugins {
    id("java")
    id("java-library")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.java24Target)
    targetCompatibility = JavaVersion.toVersion(LibExt.java24Target)
}

dependencies {
    implementation(project(":demos:app:core"))
    implementation(project(":demos:backend:desktop"))
    api(project(":webgpu:webgpu-desktop-ffm"))
}

val mainClassName = "com.github.xpenatan.webgpu.demo.app.Main"

tasks.register<JavaExec>("webgpu_demo_app_run_desktop_ffm") {
    group = "demos"
    description = "Run desktop demo using FFM bridge"
    mainClass.set(mainClassName)
    classpath = sourceSets["main"].runtimeClasspath
    jvmArgs("--enable-native-access=ALL-UNNAMED")

    if(DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX) {
        jvmArgs("-XstartOnFirstThread")
    }
}

