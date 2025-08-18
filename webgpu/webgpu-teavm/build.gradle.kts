plugins {
    id("java")
    id("java-library")
}

java.sourceCompatibility = JavaVersion.VERSION_11
java.targetCompatibility = JavaVersion.VERSION_11

val moduleName = "webgpu-teavm"

val emscriptenFile = "$projectDir/../webgpu-build/build/c++/libs/emscripten/jWebGPU.wasm.js"

tasks.jar {
    from(emscriptenFile)
}

dependencies {
    implementation("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:loader-teavm:${LibExt.jParserVersion}")
    implementation("org.teavm:teavm-jso:${LibExt.teaVMVersion}")
    implementation("org.teavm:teavm-core:${LibExt.teaVMVersion}")
    implementation("org.teavm:teavm-classlib:${LibExt.teaVMVersion}")
    implementation("org.teavm:teavm-jso-apis:${LibExt.teaVMVersion}")
}

tasks.named("clean") {
    doFirst {
        val srcPath = "$projectDir/src/main/java"
        val jsPath = "$projectDir/src/main/resources/webgpu.wasm.js"
        project.delete(files(srcPath, jsPath))
    }
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.java11Target)
    targetCompatibility = JavaVersion.toVersion(LibExt.java11Target)
}

java {
    withJavadocJar()
    withSourcesJar()
}

publishing {
    publications {
        create<MavenPublication>("maven") {
            artifactId = moduleName
            group = LibExt.groupId
            version = LibExt.libVersion
            from(components["java"])
        }
    }
}