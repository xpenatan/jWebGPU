plugins {
    id("com.android.application")
    id("kotlin-android")
}

android {
    namespace = "com.github.xpenatan.webgpu.demo"
    compileSdk = 35

    defaultConfig {
        applicationId = "com.github.xpenatan.webgpu.demo"
        minSdk = 26
        targetSdk = 35
        versionCode = 2
        versionName = "0.0.2"
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
    buildFeatures {
        viewBinding = true
    }
}

dependencies {
    implementation(project(":demos:triangle:core"))
    implementation(project(":demos:backend:android"))
}
