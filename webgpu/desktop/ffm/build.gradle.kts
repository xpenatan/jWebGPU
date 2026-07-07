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
    val dawnNativeFiles: List<String> = emptyList(),
)

val platforms = listOf(
    DesktopNativePlatform(
        "windows_x64",
        "$nativeRoot/wgpu/windows/vc/ffm/jWebGPU64.dll",
        listOf(
            "$nativeRoot/dawn/windows/vc/ffm/jWebGPU64.dll",
            "$projectDir/../../download/build/dawn-x64/webgpu_dawn.dll"
        )
    ),
    DesktopNativePlatform("linux_x64", "$nativeRoot/linux/ffm/libjWebGPU64.so"),
    DesktopNativePlatform("mac_x64", "$nativeRoot/mac/ffm/libjWebGPU64.dylib"),
    DesktopNativePlatform("mac_arm64", "$nativeRoot/mac/arm/ffm/libjWebGPUarm64.dylib"),
)

val taskNames = gradle.startParameter.taskNames
fun isTaskRequested(taskName: String): Boolean {
    return taskNames.any { it == taskName || it.endsWith(":$taskName") }
}
val isPrepareDeployTask = isTaskRequested("prepareReleaseDeploy") || isTaskRequested("prepareSnapshotDeploy")
val isPublishTask = taskNames.any { it.contains("publish", ignoreCase = true) }
val isPublicationMetadataTask = taskNames.any {
    it.contains("Publication", ignoreCase = true)
            && (it.contains("generatePom", ignoreCase = true) || it.contains("generateMetadata", ignoreCase = true))
}
val includeNativesInMainJar = !(isPrepareDeployTask || isPublishTask)
val includeNativeArtifactDependencies = isPrepareDeployTask || isPublishTask || isPublicationMetadataTask

val nativeJars = platforms.map { platform ->
    platform.name to tasks.register<Jar>("nativeJar_${platform.name}") {
        from(platform.wgpuNativeFile) {
            into("native/wgpu")
        }
        from(platform.dawnNativeFiles) {
            into("native/dawn")
        }
        archiveBaseName.set("$moduleName-${platform.name}")
        archiveClassifier.set("")
        doFirst {
            val missingFiles = (listOf(platform.wgpuNativeFile) + platform.dawnNativeFiles)
                .map(::file)
                .filterNot { it.isFile }
            if(missingFiles.isNotEmpty()) {
                logger.warn("Missing desktop FFM native libraries for ${platform.name}: ${missingFiles.joinToString { it.absolutePath }}")
            }
        }
    }
}

tasks.named<Jar>("jar") {
    inputs.property("includeNativesInMainJar", includeNativesInMainJar)
    if(includeNativesInMainJar) {
        platforms.forEach { platform ->
            from(platform.wgpuNativeFile) {
                into("native/wgpu")
            }
            from(platform.dawnNativeFiles) {
                into("native/dawn")
            }
        }
    }
}

val nativeRuntime by configurations.creating {
    isCanBeConsumed = true
    isCanBeResolved = false
}

artifacts {
    nativeJars.forEach { add(nativeRuntime.name, it.second) }
}

dependencies {
    implementation("com.github.xpenatan.jParser:runtime-ffm:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-ffm_windows_x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-ffm_linux_x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-ffm_mac_x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-ffm_mac_arm64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:api-core:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")

    if(includeNativeArtifactDependencies) {
        platforms.forEach { platform ->
            runtimeOnly("${LibExt.groupId}:${moduleName}_${platform.name}:${LibExt.libVersion}")
        }
    }
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
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaFFMTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaFFMTarget)
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

        nativeJars.forEach { (platform, nativeJar) ->
            create<MavenPublication>("mavenNative_${platform}") {
                artifactId = "${moduleName}_$platform"
                groupId = LibExt.groupId
                version = LibExt.libVersion
                artifact(nativeJar)
            }
        }
    }
}
