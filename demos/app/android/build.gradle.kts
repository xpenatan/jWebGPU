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
        versionCode = 4
        versionName = "0.0.4"
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
