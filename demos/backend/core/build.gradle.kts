plugins {
    id("java")
    id("java-library")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
}

dependencies {
    compileOnlyApi("com.github.xpenatan.jParser:runtime-core:${LibExt.jParserVersion}")
    if(LibExt.exampleUseRepoLibs) {
        compileOnlyApi("com.github.xpenatan.jWebGPU:webgpu-core:-SNAPSHOT")
    }
    else {
        compileOnlyApi(project(":webgpu:core"))
    }
}
