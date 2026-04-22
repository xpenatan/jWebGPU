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
    sourceCompatibility = JavaVersion.toVersion(LibExt.java11Target)
    targetCompatibility = JavaVersion.toVersion(LibExt.java11Target)
}

tasks.register<JavaExec>("webgpu_build_project") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("gen_ffm_desktop", "gen_jni_desktop", "gen_jni_android", "gen_jni_ios", "gen_teavm") // Just generate classes
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_windows64_dawn_jni") {
    group = "webgpu"
    description = "Generate dawn native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("jni_windows64", "dawn")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_windows64_dawn_ffm") {
    group = "webgpu"
    description = "Generate dawn native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("ffm_windows64", "dawn")
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
    args = mutableListOf("jni_windows64")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_linux64_wgpu_jni") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("jni_linux64")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_mac64_wgpu_jni") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("jni_mac64")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_macArm_wgpu_jni") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("jni_macArm")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_android_wgpu_jni") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("jni_android")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_windows64_wgpu_ffm") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("ffm_windows64")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_linux64_wgpu_ffm") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("ffm_linux64")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_mac64_wgpu_ffm") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("ffm_mac64")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("webgpu_build_project_macArm_wgpu_ffm") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("ffm_macArm")
    classpath = sourceSets["main"].runtimeClasspath
}