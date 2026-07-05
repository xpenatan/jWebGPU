plugins {
    id("java-library")
}

val moduleName = "webgpu-jni-desktop"

base {
    archivesName.set(moduleName)
}

val nativeRoot = file("$projectDir/../../builder/build/c++/libs")
val wgpuNativePaths = listOf(
    "$nativeRoot/wgpu/windows/vc/jni/jWebGPU64.dll",
    "$nativeRoot/linux/jni/libjWebGPU64.so",
    "$nativeRoot/mac/jni/libjWebGPU64.dylib",
    "$nativeRoot/mac/arm/jni/libjWebGPUarm64.dylib"
)
val dawnNativePaths = listOf(
    "$nativeRoot/dawn/windows/vc/jni/jWebGPU64.dll",
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
    api(project(":webgpu:shared:jni"))

    implementation("com.github.xpenatan.jParser:runtime-jni_windows_x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-jni_linux_x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-jni_mac_x64:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:runtime-jni_mac_arm64:${LibExt.jParserVersion}")

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
    }
}
