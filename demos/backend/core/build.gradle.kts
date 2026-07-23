plugins {
    id("java")
    id("java-library")
}

val exampleUseRepoLibs = providers.gradleProperty("exampleUseRepoLibs")
    .map(String::toBoolean)
    .getOrElse(false)

java {
    sourceCompatibility = JavaVersion.toVersion(libs.versions.javaMain.get())
    targetCompatibility = JavaVersion.toVersion(libs.versions.javaMain.get())
}

dependencies {
    compileOnlyApi(libs.jParserRuntimeCore)
    if(exampleUseRepoLibs) {
        compileOnlyApi(libs.jWebGPUCore)
    }
    else {
        compileOnlyApi(project(":webgpu:core"))
    }
}
