pluginManagement {
    repositories {
        google()
        maven { url = uri("https://central.sonatype.com/repository/maven-snapshots/") }
        mavenLocal()
        mavenCentral()
        gradlePluginPortal()
    }
}

rootProject.name = "jWebGPU"

include(":webgpu:download") // Module to download native dependencies

include(":webgpu:builder")
include(":webgpu:base")
include(":webgpu:core")
include(":webgpu:shared:jni")
include(":webgpu:shared:c")
include(":webgpu:desktop:c")
include(":webgpu:desktop:jni")
include(":webgpu:desktop:ffm")
include(":webgpu:web:wasm")
include(":webgpu:android:jni")
