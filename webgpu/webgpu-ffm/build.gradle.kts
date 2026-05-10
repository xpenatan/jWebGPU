plugins {
    id("java")
    id("java-library")
}

val moduleName = "webgpu-ffm"

val libDir = "${rootProject.projectDir}/webgpu"
val windowsFile = "$libDir/webgpu-build/build/c++/libs/windows/vc/wgpu/ffm/jWebGPU64.dll"
val linuxFile = "$libDir/webgpu-build/build/c++/libs/linux/wgpu/ffm/libjWebGPU64.so"
val macArmFile = "$libDir/webgpu-build/build/c++/libs/mac/arm/wgpu/ffm/libjWebGPUarm64.dylib"
val macFile = "$libDir/webgpu-build/build/c++/libs/mac/wgpu/ffm/libjWebGPU64.dylib"
val windowsFileDawn1 = "$libDir/webgpu-build/build/c++/libs/windows/vc/dawn/ffm/jWebGPU64.dll"
val windowsFileDawn2 = "$libDir/webgpu-build/build/c++/libs/windows/vc/dawn/ffm/webgpu_dawn.dll"

dependencies {
    implementation("com.github.xpenatan.jParser:runtime-ffm:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-ffm_windows_x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-ffm_linux_x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-ffm_mac_x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-ffm_mac_arm64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:api-core:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")
}

val platforms: Map<String, Jar.() -> Unit> = mapOf(
    "windows_x64_dawn" to {
        from(windowsFileDawn1) { into("native/dawn") }
        from(windowsFileDawn2) { into("native/dawn") }
    },
    "windows_x64_wgpu" to {
        from(windowsFile) { into("native/wgpu") }
    },
    "linux_x64_wgpu" to {
        from(linuxFile) { into("native/wgpu") }
    },
    "mac_arm64_wgpu" to {
        from(macArmFile) { into("native/wgpu") }
    },
    "mac_x64_wgpu" to {
        from(macFile) { into("native/wgpu") }
    }
)

val taskNames = gradle.startParameter.taskNames
fun isTaskRequested(taskName: String): Boolean {
    return taskNames.any { it == taskName || it.endsWith(":$taskName") }
}
val isPrepareDeployTask = isTaskRequested("prepareReleaseDeploy") || isTaskRequested("prepareSnapshotDeploy")
val isPublishTask = taskNames.any { it.contains("publish", ignoreCase = true) }
val includeNativesInMainJar = !(isPrepareDeployTask || isPublishTask)

tasks.named<Jar>("jar") {
    if (includeNativesInMainJar) {
        platforms.forEach { (_, config) ->
            // `config` is a Jar.() -> Unit and will run with this Jar as the receiver
            config()
        }
    }
}

val nativeJars: Map<String, TaskProvider<Jar>> = platforms.mapValues { (platform, config) ->
    tasks.register<Jar>("nativeJar$platform") {
        config()
        archiveBaseName.set("${moduleName}_${platform}")
        archiveClassifier.set("")
    }
}

val nativeJarAll = tasks.register<Jar>("nativeJarAll") {
    platforms.forEach { (_, config) -> config() }
    archiveBaseName.set("${moduleName}_desktop")
    archiveClassifier.set("")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaFFMTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaFFMTarget)
}

java {
    withJavadocJar()
    withSourcesJar()
}

tasks.named("clean") {
    doFirst {
        val srcPath = "$projectDir/src/main/java"
        project.delete(files(srcPath))
    }
}

publishing {
    publications {
        create<MavenPublication>("maven") {
            artifactId = moduleName
            group = LibExt.groupId
            version = LibExt.libVersion
            from(components["java"])
        }

        nativeJars.forEach { (platform, nativeJar) ->
            create<MavenPublication>("maven${platform}") {
                artifactId = "${moduleName}_${platform}"
                group = LibExt.groupId
                version = LibExt.libVersion
                artifact(nativeJar)
            }
        }

        create<MavenPublication>("mavenDesktop") {
            artifactId = "${moduleName}_desktop"
            group = LibExt.groupId
            version = LibExt.libVersion
            artifact(nativeJarAll)
        }
    }
}
