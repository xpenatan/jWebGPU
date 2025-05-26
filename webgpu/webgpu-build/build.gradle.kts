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

tasks.register<JavaExec>("wgpu_build_project_windows64") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("windows64")
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register<JavaExec>("wgpu_build_project_teavm") {
    group = "webgpu"
    description = "Generate native project"
    mainClass.set(mainWGPUClassName)
    args = mutableListOf("teavm")
    classpath = sourceSets["main"].runtimeClasspath
}