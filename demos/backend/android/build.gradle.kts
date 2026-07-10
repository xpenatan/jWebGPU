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

    flavorDimensions += "backend"
    productFlavors {
        create("wgpu") {
            dimension = "backend"
            buildConfigField("String", "JWEBGPU_BACKEND", "\"WGPU\"")
        }
        create("dawn") {
            dimension = "backend"
            buildConfigField("String", "JWEBGPU_BACKEND", "\"DAWN\"")
        }
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

    buildFeatures {
        buildConfig = true
    }
}

dependencies {
    api(project(":demos:backend:core"))

    if(LibExt.exampleUseRepoLibs) {
        add("wgpuApi", "com.github.xpenatan.jWebGPU:webgpu-android-wgpu:-SNAPSHOT")
        add("dawnApi", "com.github.xpenatan.jWebGPU:webgpu-android-dawn:-SNAPSHOT")
    }
    else {
        api(project(":webgpu:android:jni"))
    }
}
