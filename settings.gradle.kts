pluginManagement {
    val jParserPluginVersion = "-SNAPSHOT"

    resolutionStrategy {
        eachPlugin {
            if(requested.id.id == "com.github.xpenatan.jparser") {
                useModule("com.github.xpenatan.jParser:jparser-gradle-plugin:$jParserPluginVersion")
            }
        }
    }

    plugins {
        id("com.github.xpenatan.jparser") version jParserPluginVersion
    }

    repositories {
        google()
        mavenCentral()
        maven { url = uri("https://central.sonatype.com/repository/maven-snapshots/") }
        gradlePluginPortal()
        maven {
            url = uri("http://teavm.org/maven/repository/")
            isAllowInsecureProtocol = true
        }
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

// Backend implementations used by the demo applications
include(":demos:backend:core")
include(":demos:backend:desktop")
include(":demos:backend:web")
include(":demos:backend:android")

include(":demos:app:core")
include(":demos:app:desktop-jni")
include(":demos:app:desktop-ffm")
include(":demos:app:web")
include(":demos:app:android")
