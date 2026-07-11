plugins {
    id("com.android.library")
}

val wgpuModuleName = "webgpu-android-wgpu"
val dawnModuleName = "webgpu-android-dawn"

android {
    namespace = "com.github.xpenatan.webgpu.android.jni"
    compileSdk = 36

    defaultConfig {
        minSdk = 29
    }

    flavorDimensions += "backend"
    productFlavors {
        create("wgpu") {
            dimension = "backend"
        }
        create("dawn") {
            dimension = "backend"
        }
    }

    sourceSets {
        named("wgpu") {
            jniLibs.srcDirs("$projectDir/../../builder/build/c++/libs/wgpu/android")
        }
        named("dawn") {
            jniLibs.srcDirs("$projectDir/../../builder/build/c++/libs/dawn/android")
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
        targetCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }

    publishing {
        singleVariant("wgpuRelease")
        singleVariant("dawnRelease")
    }
}

dependencies {
    api(project(":webgpu:shared:jni"))
    api("com.github.xpenatan.jParser:runtime-jni:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:runtime-android:${LibExt.jParserVersion}")
    listOf("x86", "x86_64", "armeabi_v7a", "arm64_v8a").forEach { abi ->
        api("com.github.xpenatan.jParser:runtime-android_$abi:${LibExt.jParserVersion}")
    }
    api("com.github.xpenatan.jParser:api-core:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")
}

publishing {
    publications {
        create<MavenPublication>("mavenWgpu") {
            artifactId = wgpuModuleName
            groupId = LibExt.groupId
            version = LibExt.libVersion
        }
        create<MavenPublication>("mavenDawn") {
            artifactId = dawnModuleName
            groupId = LibExt.groupId
            version = LibExt.libVersion
        }
    }
}

afterEvaluate {
    publishing {
        publications.named<MavenPublication>("mavenWgpu") {
            from(components["wgpuRelease"])
        }
        publications.named<MavenPublication>("mavenDawn") {
            from(components["dawnRelease"])
        }
    }
}
