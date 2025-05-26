plugins {
    id("java")
    id("java-library")
}

java.sourceCompatibility = JavaVersion.VERSION_11
java.targetCompatibility = JavaVersion.VERSION_11

val moduleName = "webgpu-desktop"

val libDir = "${rootProject.projectDir}/webgpu"
val windowsFile = "$libDir/webgpu-build/build/c++/libs/windows/vc/jWebGPU64.dll"
val windowsFile2 = "$libDir/webgpu-build/build/c++/libs/windows/vc/webgpu_dawn.dll"
val linuxFile = "$libDir/webgpu-build/build/c++/libs/linux/libjWebGPU64.so"
val macArmFile = "$libDir/webgpu-build/build/c++/libs/mac/arm/libjWebGPU64.dylib"
val macFile = "$libDir/webgpu-build/build/c++/libs/mac/libjWebGPU64.dylib"

tasks.jar {
    from(windowsFile)
    from(windowsFile2)
    from(linuxFile)
    from(macArmFile)
    from(macFile)
}

java {
    sourceCompatibility = JavaVersion.VERSION_11
    targetCompatibility = JavaVersion.VERSION_11
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
    }
}