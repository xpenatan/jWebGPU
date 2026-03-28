import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform

plugins {
    id("java")
    id("java-library")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.java8Target)
    targetCompatibility = JavaVersion.toVersion(LibExt.java8Target)
}

val lwjglVersion = "3.3.4"
val lwjglWindowsNatives = "natives-windows"
val lwjglLinuxNatives = "natives-linux"
val lwjglMacNatives = "natives-macos"
val lwjglMacArmNatives = "natives-macos-arm64"
val selectedBridge = providers.gradleProperty("bridge").orElse("jni")
val jniArtifactId = "backend-desktop-jni"
val ffmArtifactId = "backend-desktop-ffm"

val jniBridgeRuntime by configurations.creating
val ffmBridgeRuntime by configurations.creating
val bridgeAttribute = Attribute.of("com.github.xpenatan.webgpu.bridge", String::class.java)

val desktopJni by configurations.creating {
    isCanBeConsumed = true
    isCanBeResolved = false
    extendsFrom(configurations["api"], configurations["implementation"], configurations["runtimeOnly"], jniBridgeRuntime)
    attributes {
        attribute(Usage.USAGE_ATTRIBUTE, objects.named(Usage.JAVA_RUNTIME))
        attribute(Category.CATEGORY_ATTRIBUTE, objects.named(Category.LIBRARY))
        attribute(Bundling.BUNDLING_ATTRIBUTE, objects.named(Bundling.EXTERNAL))
        attribute(LibraryElements.LIBRARY_ELEMENTS_ATTRIBUTE, objects.named(LibraryElements.JAR))
        attribute(bridgeAttribute, "jni")
    }
    outgoing.artifact(tasks.named("jar"))
}

val desktopFfm by configurations.creating {
    isCanBeConsumed = true
    isCanBeResolved = false
    extendsFrom(configurations["api"], configurations["implementation"], configurations["runtimeOnly"], ffmBridgeRuntime)
    attributes {
        attribute(Usage.USAGE_ATTRIBUTE, objects.named(Usage.JAVA_RUNTIME))
        attribute(Category.CATEGORY_ATTRIBUTE, objects.named(Category.LIBRARY))
        attribute(Bundling.BUNDLING_ATTRIBUTE, objects.named(Bundling.EXTERNAL))
        attribute(LibraryElements.LIBRARY_ELEMENTS_ATTRIBUTE, objects.named(LibraryElements.JAR))
        attribute(bridgeAttribute, "ffm")
    }
    outgoing.artifact(tasks.named("jar"))
}

dependencies {
    api(project(":demos:backend:core"))
    compileOnly("com.github.xpenatan.jParser:idl-helper-core:${LibExt.jParserVersion}")

    if(LibExt.exampleUseRepoLibs) {
        compileOnly("com.github.xpenatan.jWebGPU:webgpu-core:-SNAPSHOT")
//        api("com.github.xpenatan.jWebGPU:webgpu-desktop:-SNAPSHOT:windows_64_dawn")
//        api("com.github.xpenatan.jWebGPU:webgpu-desktop:-SNAPSHOT:windows_64_wgpu")
//        api("com.github.xpenatan.jWebGPU:webgpu-desktop:-SNAPSHOT:linux_64_wgpu")
//        api("com.github.xpenatan.jWebGPU:webgpu-desktop:-SNAPSHOT:mac_arm64_wgpu")
//        api("com.github.xpenatan.jWebGPU:webgpu-desktop:-SNAPSHOT:mac_64_wgpu")
    }
    else {
        compileOnly(project(":webgpu:webgpu-core"))
//        api(project(":webgpu:webgpu-desktop"))
    }

    jniBridgeRuntime(project(":webgpu:webgpu-core"))
    jniBridgeRuntime(project(":webgpu:webgpu-desktop-jni"))
    jniBridgeRuntime("com.github.xpenatan.jParser:idl-helper-core:${LibExt.jParserVersion}")

    // Use published FFM artifact to avoid compiling local Java 24 module on Java 8/JNI workflows.
    ffmBridgeRuntime("${LibExt.groupId}:webgpu-desktop-ffm:${LibExt.libVersion}")

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
    runtimeOnly("org.lwjgl:lwjgl::$lwjglMacArmNatives")
    runtimeOnly("org.lwjgl:lwjgl-glfw::$lwjglMacArmNatives")
    runtimeOnly("org.lwjgl:lwjgl-opengl::$lwjglMacArmNatives")
}

val mainClassName = "com.github.xpenatan.webgpu.demo.triangle.GLFWApp"

tasks.register<JavaExec>("webgpu_demo_triangle_desktop") {
    group = "demos"
    mainClass.set(mainClassName)
    classpath = sourceSets["main"].runtimeClasspath + if(selectedBridge.get().equals("ffm", ignoreCase = true)) ffmBridgeRuntime else jniBridgeRuntime

    if(DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX) {
        jvmArgs("-XstartOnFirstThread")
    }
}