plugins {
    id("java")
    id("java-library")
}

java.sourceCompatibility = JavaVersion.VERSION_11
java.targetCompatibility = JavaVersion.VERSION_11

val mainWGPUClassName = "WGPUBuild"

dependencies {
    implementation(project(":webgpu:webgpu-base"))
    implementation("com.github.xpenatan.jParser:jParser-core:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-build:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-build-tool:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-teavm:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-cpp:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-idl:${LibExt.jParserVersion}")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.java8Target)
    targetCompatibility = JavaVersion.toVersion(LibExt.java8Target)
}

tasks.register<JavaExec>("webgpu_build_project") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf() // Just generate classes
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_windows64_dawn") {
    group = "webgpu"
    description = "Generate dawn native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("windows64_dawn")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_teavm_dawn") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("teavm")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_windows64_wgpu") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("windows64")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_linux64_wgpu") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("linux64")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_mac64_wgpu") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("mac64")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_macArm_wgpu") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("macArm")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_android_wgpu") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("android")
    classpath = sourceSets["main"].runtimeClasspath
}