import buildlogic.configureWasmJar

plugins {
    id("java-library")
}

val moduleName = "webgpu-web"

base {
    archivesName.set(moduleName)
}

dependencies {
    api(project(":webgpu:core"))
    api(libs.jParserRuntimeCore)
    api(libs.jParserRuntimeWeb)
    api(libs.jParserRuntimeWebWasm)
}

tasks.named("clean") {
    doFirst {
        project.delete(files("$projectDir/src/main/java", "$projectDir/src/main/resources/webgpu.wasm.js"))
    }
}

java {
    sourceCompatibility = JavaVersion.toVersion(libs.versions.javaWeb.get())
    targetCompatibility = JavaVersion.toVersion(libs.versions.javaWeb.get())
    withJavadocJar()
    withSourcesJar()
}

publishing {
    publications {
        create<MavenPublication>("maven") {
            artifactId = moduleName
            from(components["java"])
        }
    }
}

val nativeRoot = rootProject.file("webgpu/builder/build/c++/libs/emscripten")
configureWasmJar(
    artifactName = "${moduleName}_wasm",
    files = listOf(nativeRoot.resolve("jWebGPU.js"), nativeRoot.resolve("jWebGPU.wasm"))
)
