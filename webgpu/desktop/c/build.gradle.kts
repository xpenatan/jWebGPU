import org.gradle.api.GradleException

plugins {
    id("base")
}

val moduleName = "webgpu-desktop-c"

base {
    archivesName.set(moduleName)
}

val jWebGPUNativeRoot = file("$projectDir/../../builder/build/c++/libs")
val nativeDependencyRoot = file("$projectDir/../../download/build")

enum class DesktopCBackend(
    val id: String,
    val requiredHeader: String,
) {
    WGPU("wgpu", "webgpu/wgpu.h"),
    DAWN("dawn", "dawn/webgpu.h"),
}

data class DesktopCPlatform(
    val name: String,
    val bridgeDirectory: String,
    val bridgeStaticFile: String,
    val bridgeSharedFile: String,
    val bridgeImportFile: String?,
    val wgpuDependencyDirectory: String,
    val wgpuDependencyFile: String,
    val dawnDependencyDirectory: String,
    val dawnDependencyFile: String,
) {
    val jParserRuntimeArtifactId = "runtime-desktop-c_$name"
}

val platforms = listOf(
    DesktopCPlatform(
        name = "windows_x64",
        bridgeDirectory = "windows/vc/teavm_c",
        bridgeStaticFile = "jWebGPU64_.lib",
        bridgeSharedFile = "jWebGPU64.dll",
        bridgeImportFile = "jWebGPU64.lib",
        wgpuDependencyDirectory = "windows_x86_64",
        wgpuDependencyFile = "lib/wgpu_native.lib",
        dawnDependencyDirectory = "dawn-natives-windows-x64-msvc",
        dawnDependencyFile = "lib/webgpu_dawn.lib",
    ),
    DesktopCPlatform(
        name = "linux_x64",
        bridgeDirectory = "linux/teavm_c",
        bridgeStaticFile = "libjWebGPU64_.a",
        bridgeSharedFile = "libjWebGPU64.so",
        bridgeImportFile = null,
        wgpuDependencyDirectory = "linux_x86_64",
        wgpuDependencyFile = "lib/libwgpu_native.a",
        dawnDependencyDirectory = "dawn-natives-linux-x64-gcc",
        dawnDependencyFile = "lib/libwebgpu_dawn.a",
    ),
    DesktopCPlatform(
        name = "mac_x64",
        bridgeDirectory = "mac/teavm_c",
        bridgeStaticFile = "libjWebGPU64_.a",
        bridgeSharedFile = "libjWebGPU64.dylib",
        bridgeImportFile = null,
        wgpuDependencyDirectory = "macos_x86_64",
        wgpuDependencyFile = "lib/libwgpu_native.a",
        dawnDependencyDirectory = "dawn-natives-macos-x64",
        dawnDependencyFile = "lib/libwebgpu_dawn.a",
    ),
    DesktopCPlatform(
        name = "mac_arm64",
        bridgeDirectory = "mac/arm/teavm_c",
        bridgeStaticFile = "libjWebGPU64_.a",
        bridgeSharedFile = "libjWebGPUarm64.dylib",
        bridgeImportFile = null,
        wgpuDependencyDirectory = "macos_aarch64",
        wgpuDependencyFile = "lib/libwgpu_native.a",
        dawnDependencyDirectory = "dawn-natives-macos-arm64",
        dawnDependencyFile = "lib/libwebgpu_dawn.a",
    ),
)

data class DesktopCJarSpec(
    val backend: DesktopCBackend,
    val platform: DesktopCPlatform,
    val bridgeRoot: File,
    val dependencyRoot: File,
    val dependencyFile: File,
) {
    val taskName = "nativeJar_${backend.id}_${platform.name}"
    val configName = "nativeRuntime_${backend.id}_${platform.name}"
    val artifactId = "$moduleName-${backend.id}_${platform.name}"
    val nativeResourceRoot = "external_cpp/jparser/jwebgpu/native/${platform.name}"

    fun bridgeStaticFile() = File(bridgeRoot, platform.bridgeStaticFile)
    fun bridgeSharedFile() = File(bridgeRoot, platform.bridgeSharedFile)
    fun bridgeImportFile() = platform.bridgeImportFile?.let { File(bridgeRoot, it) }
    fun includeRoot() = File(dependencyRoot, "include")
    fun bridgeStaticDestination() = if(platform.name == "windows_x64") {
        "$nativeResourceRoot/md/static"
    }
    else {
        nativeResourceRoot
    }
    fun dependencyDestination() = if(platform.name == "windows_x64") {
        "$nativeResourceRoot/md/deps"
    }
    else {
        "$nativeResourceRoot/deps"
    }

    fun requiredFiles(): List<File> = buildList {
        add(bridgeStaticFile())
        add(bridgeSharedFile())
        bridgeImportFile()?.let(::add)
        add(dependencyFile)
        add(File(includeRoot(), "webgpu/webgpu.h"))
        add(File(includeRoot(), backend.requiredHeader))
    }
}

val nativeJarSpecs = platforms.flatMap { platform ->
    DesktopCBackend.values().map { backend ->
        val dependencyDirectory = when(backend) {
            DesktopCBackend.WGPU -> platform.wgpuDependencyDirectory
            DesktopCBackend.DAWN -> platform.dawnDependencyDirectory
        }
        val dependencyFile = when(backend) {
            DesktopCBackend.WGPU -> platform.wgpuDependencyFile
            DesktopCBackend.DAWN -> platform.dawnDependencyFile
        }
        DesktopCJarSpec(
            backend = backend,
            platform = platform,
            bridgeRoot = File(jWebGPUNativeRoot, "${backend.id}/${platform.bridgeDirectory}"),
            dependencyRoot = File(nativeDependencyRoot, dependencyDirectory),
            dependencyFile = File(nativeDependencyRoot, "$dependencyDirectory/$dependencyFile"),
        )
    }
}

val taskNames = gradle.startParameter.taskNames
fun isTaskRequested(taskName: String): Boolean {
    return taskNames.any { it == taskName || it.endsWith(":$taskName") }
}

val isStrictPublishTaskRequested = isTaskRequested("publish")
        || isTaskRequested("publishRelease")
val isLocalPrepareRelease = isTaskRequested("prepareRelease") && !isStrictPublishTaskRequested

val nativeJars = nativeJarSpecs.map { spec ->
    spec to tasks.register<Jar>(spec.taskName) {
        from("src/main/resources")
        from(spec.includeRoot()) {
            into("${spec.nativeResourceRoot}/include")
        }
        from(spec.bridgeStaticFile()) {
            into(spec.bridgeStaticDestination())
        }
        from(spec.bridgeSharedFile()) {
            into("${spec.nativeResourceRoot}/shared")
        }
        spec.bridgeImportFile()?.let { importFile ->
            from(importFile) {
                into("${spec.nativeResourceRoot}/shared")
            }
        }
        from(spec.dependencyFile) {
            into(spec.dependencyDestination())
        }
        archiveBaseName.set(spec.artifactId)
        archiveClassifier.set("")
        doFirst {
            val missingResources = spec.requiredFiles().filterNot(File::isFile)
            if(missingResources.isNotEmpty()) {
                throw GradleException(
                    "Missing desktop TeaVM C ${spec.backend.id} native payloads for ${spec.platform.name}:\n" +
                            missingResources.joinToString("\n") { it.absolutePath }
                )
            }
        }
    }
}

val nativeJarSpecsToPublish = if(isLocalPrepareRelease) {
    nativeJarSpecs.filter { spec -> spec.requiredFiles().all(File::isFile) }
}
else {
    nativeJarSpecs
}
val nativeJarsBySpec = nativeJars.toMap()

nativeJarSpecs.forEach { spec ->
    configurations.create(spec.configName) {
        isCanBeConsumed = true
        isCanBeResolved = false
    }
}

artifacts {
    nativeJars.forEach { (spec, nativeJar) ->
        add(spec.configName, nativeJar)
    }
}

publishing {
    publications {
        nativeJarSpecsToPublish.forEach { spec ->
            val nativeJar = nativeJarsBySpec.getValue(spec)
            create<MavenPublication>("mavenNative_${spec.backend.id}_${spec.platform.name}") {
                artifactId = spec.artifactId
                artifact(nativeJar)
                pom.withXml {
                    val dependenciesNode = asNode().appendNode("dependencies")

                    val webgpuCDependency = dependenciesNode.appendNode("dependency")
                    webgpuCDependency.appendNode("groupId", rootProject.group.toString())
                    webgpuCDependency.appendNode("artifactId", "webgpu-c")
                    webgpuCDependency.appendNode("version", project.version.toString())
                    webgpuCDependency.appendNode("scope", "compile")

                    val jParserRuntimeDependency = dependenciesNode.appendNode("dependency")
                    jParserRuntimeDependency.appendNode("groupId", "com.github.xpenatan.jParser")
                    jParserRuntimeDependency.appendNode("artifactId", spec.platform.jParserRuntimeArtifactId)
                    jParserRuntimeDependency.appendNode("version", libs.versions.jParser.get())
                    jParserRuntimeDependency.appendNode("scope", "compile")
                }
            }
        }
    }
}
