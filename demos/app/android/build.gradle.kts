plugins {
    id("com.android.application")
}

val releaseKeystoreFile = file(System.getenv("KEYSTORE_FILE") ?: "keystore.jks")
val releaseStorePassword = System.getenv("KEYSTORE_PASSWORD")
val releaseKeyAlias = System.getenv("KEY_ALIAS")
val releaseKeyPassword = System.getenv("KEY_PASSWORD")
val hasReleaseSigning = releaseKeystoreFile.exists()
        && !releaseStorePassword.isNullOrBlank()
        && !releaseKeyAlias.isNullOrBlank()
        && !releaseKeyPassword.isNullOrBlank()

android {
    namespace = "com.github.xpenatan.webgpu.demo"
    compileSdk = 36

    defaultConfig {
        applicationId = "com.github.xpenatan.webgpu.demo"
        minSdk = 29
        targetSdk = 36
        versionCode = 6
        versionName = "0.0.6"
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

    signingConfigs {
        if(hasReleaseSigning) {
            create("release") {
                storeFile = releaseKeystoreFile
                storePassword = releaseStorePassword
                keyAlias = releaseKeyAlias
                keyPassword = releaseKeyPassword
            }
        }
    }

    buildTypes {
        release {
            if(hasReleaseSigning) {
                signingConfig = signingConfigs.getByName("release")
            }
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
        targetCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
    }
    buildFeatures {
        viewBinding = true
    }
}

dependencies {
    implementation(project(":demos:app:core"))
    implementation(project(":demos:backend:android"))
}
