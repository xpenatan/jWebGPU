plugins {
    id("java")
    id("java-library")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.java8Target)
    targetCompatibility = JavaVersion.toVersion(LibExt.java8Target)
}

dependencies {
    if(LibExt.exampleUseRepoLibs) {
        api("com.github.xpenatan.jWebGPU:webgpu-core:-SNAPSHOT")
    }
    else {
        api(project(":webgpu:webgpu-core"))
    }
}
