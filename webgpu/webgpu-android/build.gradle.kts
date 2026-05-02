plugins {
    id("com.android.library")
}

val moduleName = "webgpu-jni"

val isPublishingTask = gradle.startParameter.taskNames.any { it.contains("publish", ignoreCase = true) }

val androidLibDir = "$projectDir/../webgpu-build/build/c++/libs/android"

val androidAbiFiles = mapOf(
    "android_arm64_v8a" to "$androidLibDir/arm64-v8a/libjWebGPU.so",
    "android_armeabi_v7a" to "$androidLibDir/armeabi-v7a/libjWebGPU.so",
    "android_x86" to "$androidLibDir/x86/libjWebGPU.so",
    "android_x86_64" to "$androidLibDir/x86_64/libjWebGPU.so",
)

val androidAbiJars = androidAbiFiles.mapNotNull { (classifier, filePath) ->
    if(file(filePath).exists()) {
        tasks.register<Jar>("nativeJar${classifier}") {
            from(filePath)
            archiveClassifier.set(classifier)
        }
    }
    else {
        null
    }
}

val androidAllAbiJar = tasks.register<Jar>("nativeJarAndroid") {
    archiveClassifier.set("android")
    androidAbiFiles.values.forEach { filePath ->
        val nativeFile = file(filePath)
        if(nativeFile.exists()) {
            // Keep ABI folders so same lib filename from each ABI can coexist in the jar.
            from(nativeFile) {
                into(nativeFile.parentFile.name)
            }
        }
    }
}

val filterJniLibs by tasks.registering(Copy::class) {
    into(layout.buildDirectory.dir("tmp/jniLibs"))

    // Prevent stale copied natives from leaking between publish/non-publish builds.
    doFirst {
        delete(layout.buildDirectory.dir("tmp/jniLibs"))
    }

    if(!isPublishingTask) {
        from("$projectDir/../webgpu-build/build/c++/libs/android")
        include("**/*.so")
        exclude("**/*.a")
    }
}

android {
    namespace = "com.github.xpenatan.webgpu.android"
    compileSdk = 36

    defaultConfig {
        minSdk = 29
    }

    sourceSets {
        named("main") {
            jniLibs.srcDirs(layout.buildDirectory.dir("tmp/jniLibs"))
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
        targetCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
    }

    publishing {
        singleVariant("release") {
            withSourcesJar()
        }
    }
}

tasks.named("preBuild").configure {
    dependsOn(filterJniLibs)
}

dependencies {
    api(project(":webgpu:webgpu-jni"))
    api("com.github.xpenatan.jParser:runtime-jni:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:runtime-android:${LibExt.jParserVersion}:arm64_v8a")
    api("com.github.xpenatan.jParser:runtime-android:${LibExt.jParserVersion}:armeabi_v7a")
    api("com.github.xpenatan.jParser:runtime-android:${LibExt.jParserVersion}:x86")
    api("com.github.xpenatan.jParser:runtime-android:${LibExt.jParserVersion}:x86_64")
    api("com.github.xpenatan.jParser:api-core:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")
}

tasks.named("clean") {
    doFirst {
        val srcPath = "$projectDir/src/main/java"
        project.delete(files(srcPath))
    }
}

afterEvaluate {
    publishing {
        publications {
            create<MavenPublication>("maven") {
                artifactId = moduleName
                group = LibExt.groupId
                version = LibExt.libVersion
                artifact(androidAllAbiJar)
                androidAbiJars.forEach { artifact(it) }
            }
        }
    }
}
