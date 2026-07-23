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
val isPrepareTask = isTaskRequested("prepareRelease") || isTaskRequested("prepareSnapshot")
val isPublishTask = taskNames.any { it.contains("publish", ignoreCase = true) }
val includeNativesInMainJar = !(isPrepareTask || isPublishTask)

tasks.named<Jar>("jar") {
    if(includeNativesInMainJar) {
        from(provider {
            listOf(emscriptenJS, emscriptenWASM).map(::file).filter { it.exists() }
        })
    }
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
}

java {
    withJavadocJar()
    withSourcesJar()
}

publishing {
    publications {
        create<MavenPublication>("maven") {
            artifactId = moduleName
            from(components["java"])
        }

        create<MavenPublication>("mavenWasm") {
            artifactId = "${moduleName}_wasm"
            artifact(wasmJar)
        }
    }
}
