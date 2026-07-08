import org.gradle.api.GradleException

plugins {
    id("java-library")
}

val moduleName = "webgpu-desktop-jni"

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
        "$nativeRoot/wgpu/windows/vc/jni/jWebGPU64.dll",
        listOf(
            "$nativeRoot/dawn/windows/vc/jni/jWebGPU64.dll",
            "$projectDir/../../download/build/dawn-x64/webgpu_dawn.dll"
        )
    ),
    DesktopNativePlatform("linux_x64", "$nativeRoot/wgpu/linux/jni/libjWebGPU64.so"),
    DesktopNativePlatform("mac_x64", "$nativeRoot/wgpu/mac/jni/libjWebGPU64.dylib"),
    DesktopNativePlatform("mac_arm64", "$nativeRoot/wgpu/mac/arm/jni/libjWebGPUarm64.dylib"),
)

val taskNames = gradle.startParameter.taskNames
fun isTaskRequested(taskName: String): Boolean {
    return taskNames.any { it == taskName || it.endsWith(":$taskName") }
}
val isPrepareDeployTask = isTaskRequested("prepareReleaseDeploy") || isTaskRequested("prepareSnapshotDeploy")
val isPublishTask = taskNames.any { it.contains("publish", ignoreCase = true) }
val includeNativesInMainJar = !(isPrepareDeployTask || isPublishTask)

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
                throw GradleException("Missing desktop JNI native libraries for ${platform.name}: ${missingFiles.joinToString { it.absolutePath }}")
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
    api(project(":webgpu:shared:jni"))

    testImplementation("junit:junit:${LibExt.jUnitVersion}")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
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
