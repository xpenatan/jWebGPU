plugins {
    alias(libs.plugins.androidLibrary)
}

val wgpuModuleName = "webgpu-android-wgpu"
val dawnModuleName = "webgpu-android-dawn"

android {
    namespace = "com.github.xpenatan.webgpu.android.jni"
    compileSdk = 36

    defaultConfig {
        minSdk = 29
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

    sourceSets {
        named("wgpu") {
            jniLibs.srcDirs("$projectDir/../../builder/build/c++/libs/wgpu/android")
        }
        named("dawn") {
            jniLibs.srcDirs("$projectDir/../../builder/build/c++/libs/dawn/android")
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.toVersion(libs.versions.javaMain.get())
        targetCompatibility = JavaVersion.toVersion(libs.versions.javaMain.get())
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }

    publishing {
        singleVariant("wgpuRelease")
        singleVariant("dawnRelease")
    }
}

dependencies {
    api(project(":webgpu:shared:jni"))
    api(libs.jParserRuntimeJni)
    api(libs.jParserRuntimeAndroid)
    api(libs.jParserRuntimeAndroidX86)
    api(libs.jParserRuntimeAndroidX8664)
    api(libs.jParserRuntimeAndroidArmeabiV7a)
    api(libs.jParserRuntimeAndroidArm64V8a)
    api(libs.jParserApiCore)
    api(libs.jParserLoaderCore)
}

publishing {
    publications {
        create<MavenPublication>("mavenWgpu") {
            artifactId = wgpuModuleName
        }
        create<MavenPublication>("mavenDawn") {
            artifactId = dawnModuleName
        }
    }
}

afterEvaluate {
    publishing {
        publications.named<MavenPublication>("mavenWgpu") {
            from(components["wgpuRelease"])
        }
        publications.named<MavenPublication>("mavenDawn") {
            from(components["dawnRelease"])
        }
    }
}
