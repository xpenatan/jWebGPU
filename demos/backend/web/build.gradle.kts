plugins {
    id("java")
    id("java-library")
}

val exampleUseRepoLibs = providers.gradleProperty("exampleUseRepoLibs")
    .map(String::toBoolean)
    .getOrElse(false)

java {
    sourceCompatibility = JavaVersion.toVersion(libs.versions.javaWeb.get())
    targetCompatibility = JavaVersion.toVersion(libs.versions.javaWeb.get())
}

dependencies {
    api(project(":demos:backend:core"))

    if(exampleUseRepoLibs) {
        api(libs.jWebGPUCore)
        api(libs.jWebGPUWeb)
        api(libs.jWebGPUWebWasm)
    }
    else {
        api(project(":webgpu:core"))
        api(project(":webgpu:web:wasm"))
    }
    implementation(libs.jMultiplatform)
}
