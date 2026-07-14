plugins {
    id("java-library")
}

val moduleName = "webgpu-web"

base {
    archivesName.set(moduleName)
}

val emscriptenJS = "$projectDir/../../builder/build/c++/libs/emscripten/jWebGPU.js"
val emscriptenWASM = "$projectDir/../../builder/build/c++/libs/emscripten/jWebGPU.wasm"

val wasmJar = tasks.register<Jar>("wasmJar") {
    from(provider {
        listOf(emscriptenJS, emscriptenWASM).map(::file).filter { it.exists() }
    })
    archiveBaseName.set("${moduleName}_wasm")
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
    if(includeNativesInMainJar) {
        from(provider {
            listOf(emscriptenJS, emscriptenWASM).map(::file).filter { it.exists() }
        })
    }
}

dependencies {
    api(project(":webgpu:core"))
    api("com.github.xpenatan.jParser:runtime-core:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:runtime-web:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:runtime-web_wasm:${LibExt.jParserVersion}")
}

tasks.named("clean") {
    doFirst {
        project.delete(files("$projectDir/src/main/java", "$projectDir/src/main/resources/webgpu.wasm.js"))
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
            groupId = LibExt.groupId
            version = LibExt.libVersion
            from(components["java"])
        }

        create<MavenPublication>("mavenWasm") {
            artifactId = "${moduleName}_wasm"
            groupId = LibExt.groupId
            version = LibExt.libVersion
            artifact(wasmJar)
        }
    }
}
