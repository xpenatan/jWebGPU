plugins {
    id("java")
    id("java-library")
}

java.sourceCompatibility = JavaVersion.VERSION_11
java.targetCompatibility = JavaVersion.VERSION_11

val moduleName = "webgpu-desktop"

val libDir = "${rootProject.projectDir}/webgpu"
val windowsFile = "$libDir/webgpu-build/build/c++/libs/windows/vc/wgpu/jWebGPU64.dll"
val linuxFile = "$libDir/webgpu-build/build/c++/libs/linux/libjWebGPU64.so"
val macArmFile = "$libDir/webgpu-build/build/c++/libs/mac/arm/libjWebGPUarm64.dylib"
val macFile = "$libDir/webgpu-build/build/c++/libs/mac/libjWebGPU64.dylib"
val windowsFileDawn1 = "$libDir/webgpu-build/build/c++/libs/windows/vc/dawn/jWebGPU64.dll"
val windowsFileDawn2 = "$libDir/webgpu-build/build/c++/libs/windows/vc/dawn/webgpu_dawn.dll"

tasks.jar {
    from(windowsFileDawn1)
    from(windowsFileDawn2)
    from(windowsFile)
    from(linuxFile)
    from(macArmFile)
    from(macFile)
}

val platforms: Map<String, Jar.() -> Unit> = mapOf(
    "windows_64_dawn" to {
        from(windowsFileDawn1)
        from(windowsFileDawn2)
    },
    "windows_64_wgpu" to {
        from(windowsFile)
    },
    "linux_64_wgpu" to {
        from(linuxFile)
    },
    "mac_arm64_wgpu" to {
        from(macArmFile)
    },
    "mac_64_wgpu" to {
        from(macFile)
    }
)

val nativeJars = platforms.map { (classifier, config) ->
    tasks.register<Jar>("nativeJar$classifier") {
        config()
        archiveClassifier.set(classifier)
    }
}

//java {
//    withJavadocJar()
//    withSourcesJar()
//}

publishing {
    publications {
        create<MavenPublication>("maven") {
            artifactId = moduleName
            group = LibExt.groupId
            version = LibExt.libVersion
//            from(components["java"])
            nativeJars.forEach { artifact(it) }
        }
    }
}