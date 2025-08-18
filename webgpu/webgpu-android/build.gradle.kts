plugins {
    id("com.android.library")
    kotlin("android")
}
val moduleName = "webgpu-android"

android {
    namespace = "com.github.xpenatan.webgpu.android"
    compileSdk = 35

    defaultConfig {
        minSdk = 26
    }

    sourceSets {
        named("main") {
            jniLibs.srcDirs("$projectDir/../webgpu-build/build/c++/libs/android")
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.toVersion(LibExt.java8Target)
        targetCompatibility = JavaVersion.toVersion(LibExt.java8Target)
    }
    
    kotlinOptions {
        jvmTarget = LibExt.java8Target
    }
}

dependencies {
}

publishing {
    publications {
        create<MavenPublication>("maven") {
            artifactId = moduleName
            group = LibExt.groupId
            version = LibExt.libVersion
            afterEvaluate {
                from(components["release"])
            }
        }
    }
}