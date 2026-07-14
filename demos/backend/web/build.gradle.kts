plugins {
    id("java")
    id("java-library")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaWebTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaWebTarget)
}

dependencies {
    api(project(":demos:backend:core"))

    if(LibExt.exampleUseRepoLibs) {
        api("com.github.xpenatan.jWebGPU:webgpu-core:-SNAPSHOT")
        api("com.github.xpenatan.jWebGPU:webgpu-web:-SNAPSHOT")
        api("com.github.xpenatan.jWebGPU:webgpu-web_wasm:-SNAPSHOT")
    }
    else {
        api(project(":webgpu:core"))
        api(project(":webgpu:web:wasm"))
    }
    implementation("com.github.xpenatan:jMultiplatform:${LibExt.jMultiplatform}")
}
