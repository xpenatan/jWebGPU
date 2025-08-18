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
        versionCode = 5
        versionName = "0.0.5"
    }

    signingConfigs {
        create("release") {
            storeFile = file(System.getenv("KEYSTORE_FILE") ?: "keystore.jks")
            storePassword = System.getenv("KEYSTORE_PASSWORD")
            keyAlias = System.getenv("KEY_ALIAS")
            keyPassword = System.getenv("KEY_PASSWORD")
        }
    }

    buildTypes {
        release {
            signingConfig = signingConfigs.getByName("release")
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.toVersion(LibExt.java8Target)
        targetCompatibility = JavaVersion.toVersion(LibExt.java8Target)
    }
    kotlinOptions {
        jvmTarget = LibExt.java8Target
    }
    buildFeatures {
        viewBinding = true
    }
}

dependencies {
    implementation(project(":demos:app:core"))
    implementation(project(":demos:backend:android"))
}
