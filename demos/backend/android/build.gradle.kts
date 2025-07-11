plugins {
    id("com.android.library")
    id("kotlin-android")
}

android {
    namespace = "com.github.xpenatan.webgpu.backend.android"
    compileSdk = 35

    defaultConfig {
        minSdk = 26
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    kotlinOptions {
        jvmTarget = "11"
    }
}

dependencies {
    api(project(":demos:backend:core"))

    if(LibExt.exampleUseRepoLibs) {
        api("com.github.xpenatan.jWebGPU:webgpu-core:-SNAPSHOT")
        api("com.github.xpenatan.jWebGPU:webgpu-android:-SNAPSHOT")
    }
    else {
        api(project(":webgpu:webgpu-core"))
        api(project(":webgpu:webgpu-android"))
    }
}