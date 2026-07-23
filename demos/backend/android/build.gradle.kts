plugins {
    alias(libs.plugins.androidLibrary)
    alias(libs.plugins.kotlinAndroid)
}

val exampleUseRepoLibs = providers.gradleProperty("exampleUseRepoLibs")
    .map(String::toBoolean)
    .getOrElse(false)

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
            sourceCompatibility = JavaVersion.toVersion(libs.versions.javaMain.get())
            targetCompatibility = JavaVersion.toVersion(libs.versions.javaMain.get())
        }
    }

    buildFeatures {
        buildConfig = true
    }
}

dependencies {
    api(project(":demos:backend:core"))

    if(exampleUseRepoLibs) {
        add("wgpuApi", libs.jWebGPUAndroidWgpu)
        add("dawnApi", libs.jWebGPUAndroidDawn)
    }
    else {
        api(project(":webgpu:android:jni"))
    }
}
