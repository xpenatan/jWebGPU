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
        versionCode = 3
        versionName = "0.0.3"
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
    implementation(project(":demos:app:core"))
    implementation(project(":demos:backend:android"))
}
