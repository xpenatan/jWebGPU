val moduleName = "webgpu-desktop"

val libDir = "${rootProject.projectDir}/webgpu"
val windowsFile = "$libDir/webgpu-build/build/c++/libs/windows/vc/webgpu64.dll"
val linuxFile = "$libDir/webgpu-build/build/c++/libs/linux/libwebgpu64.so"
val macArmFile = "$libDir/webgpu-build/build/c++/libs/mac/arm/libwebgpu64.dylib"
val macFile = "$libDir/webgpu-build/build/c++/libs/mac/libwebgpu64.dylib"

tasks.jar {
    from(windowsFile)
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