plugins {
    id("com.android.application")
    id("kotlin-android")
}

android {
    namespace = "com.github.xpenatan.webgpu.demo.triangle.android"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.github.xpenatan.webgpu.demo.triangle.android"
        minSdk = 24
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"
    }

//    sourceSets {
//        named("main") {
////            assets.srcDirs(project.file("../assets"))
//            jniLibs.srcDirs("libs")
//        }
//    }

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
val natives: Configuration by configurations.creating

dependencies {

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