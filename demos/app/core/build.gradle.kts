plugins {
    id("java")
    id("java-library")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.java8Target)
    targetCompatibility = JavaVersion.toVersion(LibExt.java8Target)
}

dependencies {
    implementation(project(":demos:backend:core"))

    if(LibExt.exampleUseRepoLibs) {
        compileOnly("com.github.xpenatan.jWebGPU:webgpu-core:-SNAPSHOT")
    }
    else {
        compileOnly(project(":webgpu:webgpu-core"))
    }
}
