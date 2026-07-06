plugins {
    id("java-library")
}

val moduleName = "webgpu-desktop-ffm"

base {
    archivesName.set(moduleName)
}

val nativeRoot = file("$projectDir/../../builder/build/c++/libs")
val wgpuNativePaths = listOf(
    "$nativeRoot/wgpu/windows/vc/ffm/jWebGPU64.dll",
    "$nativeRoot/linux/ffm/libjWebGPU64.so",
    "$nativeRoot/mac/ffm/libjWebGPU64.dylib",
    "$nativeRoot/mac/arm/ffm/libjWebGPUarm64.dylib"
)
val dawnNativePaths = listOf(
    "$nativeRoot/dawn/windows/vc/ffm/jWebGPU64.dll",
    "$projectDir/../../download/build/dawn-x64/webgpu_dawn.dll"
)

tasks.named<Jar>("jar") {
    from(provider {
        wgpuNativePaths.map(::file).filter { it.exists() }
    }) {
        into("native/wgpu")
    }
    from(provider {
        dawnNativePaths.map(::file).filter { it.exists() }
    }) {
        into("native/dawn")
    }
}

dependencies {
    implementation("com.github.xpenatan.jParser:runtime-ffm:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-ffm_windows_x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-ffm_linux_x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-ffm_mac_x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-ffm_mac_arm64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:api-core:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")
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
    }
}
