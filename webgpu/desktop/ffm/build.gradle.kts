import org.gradle.api.GradleException

plugins {
    id("java-library")
}

val moduleName = "webgpu-desktop-ffm"

base {
    archivesName.set(moduleName)
}

val nativeRoot = file("$projectDir/../../builder/build/c++/libs")

data class DesktopNativePlatform(
    val name: String,
    val wgpuNativeFile: String,
    val dawnNativeFile: String,
)

val platforms = listOf(
    DesktopNativePlatform(
        "windows_x64",
        "$nativeRoot/wgpu/windows/vc/ffm/jWebGPU64.dll",
        "$nativeRoot/dawn/windows/vc/ffm/jWebGPU64.dll"
    ),
    DesktopNativePlatform(
        "linux_x64",
        "$nativeRoot/wgpu/linux/ffm/libjWebGPU64.so",
        "$nativeRoot/dawn/linux/ffm/libjWebGPU64.so"
    ),
    DesktopNativePlatform(
        "mac_x64",
        "$nativeRoot/wgpu/mac/ffm/libjWebGPU64.dylib",
        "$nativeRoot/dawn/mac/ffm/libjWebGPU64.dylib"
    ),
    DesktopNativePlatform(
        "mac_arm64",
        "$nativeRoot/wgpu/mac/arm/ffm/libjWebGPUarm64.dylib",
        "$nativeRoot/dawn/mac/arm/ffm/libjWebGPUarm64.dylib"
    ),
)

enum class DesktopNativeBackend(val id: String, val resourceDir: String) {
    WGPU("wgpu", "native/wgpu"),
    DAWN("dawn", "native/dawn"),
}

data class DesktopNativeJarSpec(
    val backend: DesktopNativeBackend,
    val platformName: String,
    val nativeFile: String,
) {
    val taskName = "nativeJar_${backend.id}_$platformName"
    val configName = "nativeRuntime_${backend.id}_$platformName"
    val artifactId = "$moduleName-${backend.id}_$platformName"
    val jParserRuntimeArtifactId = "runtime-desktop-ffm_$platformName"
}

val nativeJarSpecs = platforms.flatMap { platform ->
    listOf(
        DesktopNativeJarSpec(DesktopNativeBackend.WGPU, platform.name, platform.wgpuNativeFile),
        DesktopNativeJarSpec(DesktopNativeBackend.DAWN, platform.name, platform.dawnNativeFile),
    )
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
        from(spec.nativeFile) {
            into(spec.backend.resourceDir)
        }
        archiveBaseName.set(spec.artifactId)
        archiveClassifier.set("")
        doFirst {
            val nativeFile = file(spec.nativeFile)
            if(!nativeFile.isFile) {
                throw GradleException("Missing desktop FFM ${spec.backend.id} native library for ${spec.platformName}: ${nativeFile.absolutePath}")
            }
        }
    }
}

val nativeJarSpecsToPublish = if(isLocalPrepareRelease) {
    nativeJarSpecs.filter { file(it.nativeFile).isFile }
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

dependencies {
    implementation(libs.jParserRuntimeDesktopFFM)
    implementation(libs.jParserApiCore)
    implementation(libs.jParserLoaderCore)

}

sourceSets {
    main {
        java.setSrcDirs(listOf("src/main/java"))
    }
}

tasks.named("clean") {
    doFirst {
        project.delete(files("$projectDir/src/main/java"))
    }
}

java {
    sourceCompatibility = JavaVersion.toVersion(libs.versions.javaFFM.get())
    targetCompatibility = JavaVersion.toVersion(libs.versions.javaFFM.get())
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

        nativeJarSpecsToPublish.forEach { spec ->
            val nativeJar = nativeJarsBySpec.getValue(spec)
            create<MavenPublication>("mavenNative_${spec.backend.id}_${spec.platformName}") {
                artifactId = spec.artifactId
                artifact(nativeJar)
                pom.withXml {
                    val dependenciesNode = asNode().appendNode("dependencies")
                    val dependencyNode = dependenciesNode.appendNode("dependency")
                    dependencyNode.appendNode("groupId", "com.github.xpenatan.jParser")
                    dependencyNode.appendNode("artifactId", spec.jParserRuntimeArtifactId)
                    dependencyNode.appendNode("version", libs.versions.jParser.get())
                    dependencyNode.appendNode("scope", "runtime")
                }
            }
        }
    }
}
