import org.gradle.api.GradleException

plugins {
    id("base")
}

val moduleName = "webgpu-desktop-c"
val backendName = "wgpu"
val platformName = "windows_x64"
val nativeArtifactId = "$moduleName-${backendName}_$platformName"

base {
    archivesName.set(moduleName)
}

val jWebGPUNativeRoot = file("$projectDir/../../builder/build/c++/libs/wgpu/windows/vc/teavm_c")
val wgpuNativeRoot = file("$projectDir/../../download/build/windows_x86_64/lib")
val wgpuNativeIncludeRoot = file("$projectDir/../../download/build/windows_x86_64/include")
val nativeResourceRoot = "external_cpp/jparser/jwebgpu/native/$platformName"

data class NativeResource(
    val source: File,
    val destination: String,
)

val nativeResources = listOf(
    NativeResource(File(jWebGPUNativeRoot, "jWebGPU64_.lib"), nativeResourceRoot),
    NativeResource(File(jWebGPUNativeRoot, "jWebGPU64.lib"), "$nativeResourceRoot/shared"),
    NativeResource(File(jWebGPUNativeRoot, "jWebGPU64.dll"), "$nativeResourceRoot/shared"),
    NativeResource(File(wgpuNativeRoot, "wgpu_native.lib"), "$nativeResourceRoot/deps"),
)

val nativeJar = tasks.register<Jar>("nativeJar_${backendName}_$platformName") {
    from("src/main/resources")
    from(wgpuNativeIncludeRoot) {
        into("$nativeResourceRoot/include")
    }
    nativeResources.forEach { resource ->
        from(resource.source) {
            into(resource.destination)
        }
    }
    archiveBaseName.set(nativeArtifactId)
    archiveClassifier.set("")
    doFirst {
        val requiredResources = nativeResources.map { it.source } + listOf(
            File(wgpuNativeIncludeRoot, "webgpu/webgpu.h"),
            File(wgpuNativeIncludeRoot, "webgpu/wgpu.h"),
        )
        val missingResources = requiredResources.filterNot(File::isFile)
        if(missingResources.isNotEmpty()) {
            throw GradleException(
                "Missing desktop TeaVM C WGPU native payloads for $platformName:\n" +
                    missingResources.joinToString("\n") { it.absolutePath }
            )
        }
    }
}

val nativeRuntime by configurations.creating {
    isCanBeConsumed = true
    isCanBeResolved = false
}

artifacts {
    add(nativeRuntime.name, nativeJar)
}

publishing {
    publications {
        create<MavenPublication>("mavenNative_${backendName}_$platformName") {
            artifactId = nativeArtifactId
            groupId = LibExt.groupId
            version = LibExt.libVersion
            artifact(nativeJar)
            pom.withXml {
                val dependenciesNode = asNode().appendNode("dependencies")

                val webgpuCDependency = dependenciesNode.appendNode("dependency")
                webgpuCDependency.appendNode("groupId", LibExt.groupId)
                webgpuCDependency.appendNode("artifactId", "webgpu-c")
                webgpuCDependency.appendNode("version", LibExt.libVersion)
                webgpuCDependency.appendNode("scope", "compile")

                val jParserRuntimeDependency = dependenciesNode.appendNode("dependency")
                jParserRuntimeDependency.appendNode("groupId", "com.github.xpenatan.jParser")
                jParserRuntimeDependency.appendNode("artifactId", "runtime-desktop-c_$platformName")
                jParserRuntimeDependency.appendNode("version", LibExt.jParserVersion)
                jParserRuntimeDependency.appendNode("scope", "compile")
            }
        }
    }
}
