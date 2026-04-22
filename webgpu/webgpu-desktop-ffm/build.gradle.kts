plugins {
    id("java")
    id("java-library")
}

val moduleName = "webgpu-desktop-ffm"

val libDir = "${rootProject.projectDir}/webgpu"
val windowsFile = "$libDir/webgpu-build/build/c++/libs/windows/vc/wgpu/ffm/jWebGPU64.dll"
val linuxFile = "$libDir/webgpu-build/build/c++/libs/linux/wgpu/ffm/libjWebGPU64.so"
val macArmFile = "$libDir/webgpu-build/build/c++/libs/mac/arm/wgpu/ffm/libjWebGPUarm64.dylib"
val macFile = "$libDir/webgpu-build/build/c++/libs/mac/wgpu/ffm/libjWebGPU64.dylib"
val windowsFileDawn1 = "$libDir/webgpu-build/build/c++/libs/windows/vc/dawn/ffm/jWebGPU64.dll"
val windowsFileDawn2 = "$libDir/webgpu-build/build/c++/libs/windows/vc/dawn/ffm/webgpu_dawn.dll"

dependencies {
    implementation("com.github.xpenatan.jParser:idl-helper-desktop-ffm:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:idl-core:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")
}

tasks.jar {
    from(windowsFileDawn1) { into("native/dawn") }
    from(windowsFileDawn2) { into("native/dawn") }
    from(windowsFile) { into("native/wgpu") }
    from(linuxFile) { into("native/wgpu") }
    from(macArmFile) { into("native/wgpu") }
    from(macFile) { into("native/wgpu") }
}

val platforms: Map<String, Jar.() -> Unit> = mapOf(
    "windows_64_dawn" to {
        from(windowsFileDawn1) { into("native/dawn") }
        from(windowsFileDawn2) { into("native/dawn") }
    },
    "windows_64_wgpu" to {
        from(windowsFile) { into("native/wgpu") }
    },
    "linux_64_wgpu" to {
        from(linuxFile) { into("native/wgpu") }
    },
    "mac_arm64_wgpu" to {
        from(macArmFile) { into("native/wgpu") }
    },
    "mac_64_wgpu" to {
        from(macFile) { into("native/wgpu") }
    }
)

val nativeJars = platforms.map { (classifier, config) ->
    tasks.register<Jar>("nativeJar$classifier") {
        config()
        archiveClassifier.set(classifier)
    }
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.java24Target)
    targetCompatibility = JavaVersion.toVersion(LibExt.java24Target)
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
            nativeJars.forEach { artifact(it) }
        }
    }
}