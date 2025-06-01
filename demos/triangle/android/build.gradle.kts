plugins {
    id("com.android.application")
    id("kotlin-android")
}

android {
    namespace = "com.github.xpenatan.webgpu.demo"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.escplay.webgpu.demo"
        minSdk = 26
        targetSdk = 34
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
val natives: Configuration by configurations.creating

dependencies {
    implementation(project(":demos:triangle:core"))
    implementation(project(":demos:backend:android"))
}

tasks.register("copyAndroidNatives") {
    group = "basic-android"
    doFirst {
        natives.files.forEach { jar ->
            val outputDir = file("libs/" + jar.nameWithoutExtension.substringAfterLast("natives-"))
            outputDir.mkdirs()
            copy {
                from(zipTree(jar))
                into(outputDir)
                include("*.so")
            }
        }
    }
}

tasks.whenTaskAdded {
    if ("package" in name) {
        dependsOn("copyAndroidNatives")
    }
}