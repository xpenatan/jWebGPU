plugins {
    id("com.android.library")
}

val moduleName = "webgpu-android"

val filterJniLibs by tasks.registering(Copy::class) {
    from("$projectDir/../webgpu-build/build/c++/libs/android")
    into(layout.buildDirectory.dir("tmp/jniLibs"))
    include("**/*.so")
    exclude("**/*.a")
}

tasks.named("preBuild").configure {
    dependsOn(filterJniLibs)
}

android {
    namespace = "com.github.xpenatan.webgpu.android"
    compileSdk = 36

    defaultConfig {
        minSdk = 29
    }

    sourceSets {
        named("main") {
            jniLibs.srcDirs(layout.buildDirectory.dir("tmp/jniLibs"))
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
        targetCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
    }

    publishing {
        singleVariant("release") {
            withSourcesJar()
        }
    }
}

dependencies {
    implementation("com.github.xpenatan.jParser:idl-helper-android:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:idl-core:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")
}

tasks.named("clean") {
    doFirst {
        val srcPath = "$projectDir/src/main/java"
        project.delete(files(srcPath))
    }
}

afterEvaluate {
    publishing {
        publications {
            create<MavenPublication>("maven") {
                artifactId = moduleName
                group = LibExt.groupId
                version = LibExt.libVersion
                from(components["release"])
            }
        }
    }
}
