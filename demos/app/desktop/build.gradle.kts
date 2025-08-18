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
}

val mainClassName = "com.github.xpenatan.webgpu.demo.app.MainDesktop"

tasks.register<JavaExec>("webgpu_demo_app_run_desktop") {
    group = "demos"
    mainClass.set(mainClassName)
    classpath = sourceSets["main"].runtimeClasspath

    if(DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX) {
        jvmArgs("-XstartOnFirstThread")
    }
}