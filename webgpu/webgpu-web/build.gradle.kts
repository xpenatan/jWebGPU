plugins {
    id("java")
    id("java-library")
}

val moduleName = "webgpu-web"

val emscriptenJS = "$projectDir/../webgpu-build/build/c++/libs/emscripten/jWebGPU.js"
val emscriptenWASM = "$projectDir/../webgpu-build/build/c++/libs/emscripten/jWebGPU.wasm"

val wasmJar = tasks.register<Jar>("wasmJar") {
    // Publish web runtime payload as a dedicated artifactId publication.
    from(emscriptenJS, emscriptenWASM)
    archiveBaseName.set("${moduleName}-wasm")
    archiveClassifier.set("")
}

val taskNames = gradle.startParameter.taskNames
fun isTaskRequested(taskName: String): Boolean {
    return taskNames.any { it == taskName || it.endsWith(":$taskName") }
}
val isPrepareDeployTask = isTaskRequested("prepareReleaseDeploy") || isTaskRequested("prepareSnapshotDeploy")
val isPublishTask = taskNames.any { it.contains("publish", ignoreCase = true) }
val includeNativesInMainJar = !(isPrepareDeployTask || isPublishTask)

tasks.named<Jar>("jar") {
    // For in-repo project dependencies, keep classes and web payload in the same jar.
    // During publishing, keep main runtime-web artifact classes-only.
    if(includeNativesInMainJar) {
        from(emscriptenJS, emscriptenWASM)
    }
}

dependencies {
    api("com.github.xpenatan.jParser:runtime-core:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:runtime-web:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:runtime-web_wasm:${LibExt.jParserVersion}")

    implementation("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:loader-web:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:api-core:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:api-web:${LibExt.jParserVersion}")

    api("org.teavm:teavm-jso:${LibExt.teaVMVersion}")
    api("org.teavm:teavm-core:${LibExt.teaVMVersion}")
    api("org.teavm:teavm-classlib:${LibExt.teaVMVersion}")
    api("org.teavm:teavm-jso-apis:${LibExt.teaVMVersion}")
}

tasks.named("clean") {
    doFirst {
        val srcPath = "$projectDir/src/main/java"
        val jsPath = "$projectDir/src/main/resources/webgpu.wasm.js"
        project.delete(files(srcPath, jsPath))
    }
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaWebTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaWebTarget)
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

        create<MavenPublication>("mavenWasm") {
            artifactId = "${moduleName}_wasm"
            group = LibExt.groupId
            version = LibExt.libVersion
            artifact(wasmJar)
        }
    }
}
