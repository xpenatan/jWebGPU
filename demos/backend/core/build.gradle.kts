plugins {
    id("java")
    id("java-library")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
}

dependencies {
    compileOnly("com.github.xpenatan.jParser:idl-helper-core:${LibExt.jParserVersion}")
    if(LibExt.exampleUseRepoLibs) {
        compileOnly("com.github.xpenatan.jWebGPU:webgpu-core:-SNAPSHOT")
    }
    else {
        compileOnly(project(":webgpu:webgpu-core"))
    }
}
