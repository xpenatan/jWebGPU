plugins {
    id("java")
    id("java-library")
}

val moduleName = "webgpu-jni"

val libDir = "${rootProject.projectDir}/webgpu"
val windowsFile = "$libDir/webgpu-build/build/c++/libs/windows/vc/wgpu/jni/jWebGPU64.dll"
val linuxFile = "$libDir/webgpu-build/build/c++/libs/linux/wgpu/jni/libjWebGPU64.so"
val macArmFile = "$libDir/webgpu-build/build/c++/libs/mac/arm/wgpu/jni/libjWebGPUarm64.dylib"
val macFile = "$libDir/webgpu-build/build/c++/libs/mac/wgpu/jni/libjWebGPU64.dylib"
val windowsFileDawn1 = "$libDir/webgpu-build/build/c++/libs/windows/vc/dawn/jni/jWebGPU64.dll"
val windowsFileDawn2 = "$libDir/webgpu-build/build/c++/libs/windows/vc/dawn/jni/webgpu_dawn.dll"

dependencies {
    implementation("com.github.xpenatan.jParser:runtime-jni:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-jni-windows-64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-jni-linux-x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-jni-mac-x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-jni-mac-arm64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:api-core:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")
}


val platforms: Map<String, Jar.() -> Unit> = mapOf(
    "windows-64-dawn" to {
        from(windowsFileDawn1) { into("native/dawn") }
        from(windowsFileDawn2) { into("native/dawn") }
    },
    "windows-64-wgpu" to {
        from(windowsFile) { into("native/wgpu") }
    },
    "linux-64-wgpu" to {
        from(linuxFile) { into("native/wgpu") }
    },
    "mac-arm64-wgpu" to {
        from(macArmFile) { into("native/wgpu") }
    },
    "mac-64-wgpu" to {
        from(macFile) { into("native/wgpu") }
    }
)

val isPublishing: Boolean = gradle.startParameter.taskNames.any { name ->
    name.contains("publish", ignoreCase = true) ||
    name.contains("publishToMavenLocal", ignoreCase = true) ||
    name.contains("upload", ignoreCase = true)
}
val includeNativesInMainJar = !isPublishing

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
        archiveBaseName.set("${moduleName}-${platform}")
        archiveClassifier.set("")
    }
}

val nativeJarAll = tasks.register<Jar>("nativeJarAll") {
    platforms.forEach { (_, config) -> config() }
    archiveBaseName.set("${moduleName}-desktop")
    archiveClassifier.set("")
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
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
            create<MavenPublication>("maven${platform.replace("_", "")}") {
                artifactId = "${moduleName}-${platform}"
                group = LibExt.groupId
                version = LibExt.libVersion
                artifact(nativeJar)
            }
        }

        create<MavenPublication>("mavenDesktop") {
            artifactId = "${moduleName}-desktop"
            group = LibExt.groupId
            version = LibExt.libVersion
            artifact(nativeJarAll)
        }
    }
}