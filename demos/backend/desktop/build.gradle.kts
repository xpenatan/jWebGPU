import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform

plugins {
    id("java")
    id("java-library")
}

java.sourceCompatibility = JavaVersion.VERSION_11
java.targetCompatibility = JavaVersion.VERSION_11

val lwjglVersion = "3.3.4"
val lwjglWindowsNatives = "natives-windows"
val lwjglLinuxNatives = "natives-linux"
val lwjglMacNatives = "natives-macos"

dependencies {
    api(project(":demos:backend:core"))

    if(LibExt.exampleUseRepoLibs) {
        api("com.github.xpenatan.jWebGPU:webgpu-core:-SNAPSHOT")
        api("com.github.xpenatan.jWebGPU:webgpu-desktop:-SNAPSHOT")
    }
    else {
        api(project(":webgpu:webgpu-core"))
        api(project(":webgpu:webgpu-desktop"))
    }

    implementation(platform("org.lwjgl:lwjgl-bom:$lwjglVersion"))
    implementation("org.lwjgl:lwjgl")
    implementation("org.lwjgl:lwjgl-glfw")
    implementation("org.lwjgl:lwjgl-opengl")
    runtimeOnly("org.lwjgl:lwjgl::$lwjglWindowsNatives")
    runtimeOnly("org.lwjgl:lwjgl-glfw::$lwjglWindowsNatives")
    runtimeOnly("org.lwjgl:lwjgl-opengl::$lwjglWindowsNatives")
    runtimeOnly("org.lwjgl:lwjgl::$lwjglLinuxNatives")
    runtimeOnly("org.lwjgl:lwjgl-glfw::$lwjglLinuxNatives")
    runtimeOnly("org.lwjgl:lwjgl-opengl::$lwjglLinuxNatives")
    runtimeOnly("org.lwjgl:lwjgl::$lwjglMacNatives")
    runtimeOnly("org.lwjgl:lwjgl-glfw::$lwjglMacNatives")
    runtimeOnly("org.lwjgl:lwjgl-opengl::$lwjglMacNatives")
}

val mainClassName = "com.github.xpenatan.webgpu.demo.triangle.GLFWApp"

tasks.register<JavaExec>("webgpu_demo_triangle_desktop") {
    group = "demos"
    mainClass.set(mainClassName)
    classpath = sourceSets["main"].runtimeClasspath

    if(DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX) {
        jvmArgs("-XstartOnFirstThread")
    }
}