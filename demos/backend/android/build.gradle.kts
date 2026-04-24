plugins {
    id("com.android.library")
    id("kotlin-android")
}

android {
    namespace = "com.github.xpenatan.webgpu.backend.android"
    compileSdk = 36

    defaultConfig {
        minSdk = 29
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }

    kotlin {
        compileOptions {
            sourceCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
            targetCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
        }
    }
}

dependencies {
    api(project(":demos:backend:core"))

    if(LibExt.exampleUseRepoLibs) {
        api("com.github.xpenatan.jWebGPU:webgpu-android:-SNAPSHOT")
    }
    else {
        api(project(":webgpu:webgpu-android"))
    }
}