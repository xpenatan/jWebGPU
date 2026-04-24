plugins {
    id("java")
    id("java-library")
}

val mainWGPUClassName = "WGPUBuild"

dependencies {
    implementation(project(":webgpu:webgpu-base"))
    implementation("com.github.xpenatan.jParser:jParser-core:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-build:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-build-tool:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-teavm:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-jni:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-ffm:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:jParser-idl:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:idl-helper-core:${LibExt.jParserVersion}")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaWebTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaWebTarget)
}

tasks.register<JavaExec>("webgpu_build_project") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("gen_desktop_ffm", "gen_desktop_jni", "gen_android_jni", "gen_ios_jni", "gen_teavm") // Just generate classes
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_windows64_dawn_jni") {
    group = "webgpu"
    description = "Generate dawn native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("windows64_jni", "dawn")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_windows64_dawn_ffm") {
    group = "webgpu"
    description = "Generate dawn native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("windows64_ffm", "dawn")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_teavm_dawn") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("teavm")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_windows64_wgpu_jni") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("windows64_jni")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_linux64_wgpu_jni") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("linux64_jni")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_mac64_wgpu_jni") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("mac64_jni")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_macArm_wgpu_jni") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("macArm_jni")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_android_wgpu_jni") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("android_jni")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_windows64_wgpu_ffm") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("windows64_ffm")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_linux64_wgpu_ffm") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("linux64_ffm")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_mac64_wgpu_ffm") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("mac64_ffm")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_macArm_wgpu_ffm") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("macArm_ffm")
    classpath = sourceSets["main"].runtimeClasspath
}