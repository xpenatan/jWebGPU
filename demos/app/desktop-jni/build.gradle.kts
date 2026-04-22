import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform

plugins {
    id("java")
    id("java-library")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.java8Target)
    targetCompatibility = JavaVersion.toVersion(LibExt.java8Target)
}

dependencies {
    implementation(project(":demos:app:core"))
    implementation(project(":demos:backend:desktop"))
    api(project(":webgpu:webgpu-desktop-jni"))
}

val mainClassName = "com.github.xpenatan.webgpu.demo.app.Main"

tasks.register<JavaExec>("webgpu_demo_app_run_desktop_jni") {
    group = "demos"
    description = "Run desktop demo using JNI bridge"
    mainClass.set(mainClassName)
    classpath = sourceSets["main"].runtimeClasspath

    if(DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX) {
        jvmArgs("-XstartOnFirstThread")
    }
}

