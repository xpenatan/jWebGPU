plugins {
    id("java")
    id("com.github.xpenatan.easy-publishing") version "0.1.0"
}

LibExt.isRelease = rootProject.extra["easyPublishing.releaseRequested"] as Boolean

buildscript {
    repositories {
        mavenCentral()
        google()
    }

    val kotlinVersion = "2.3.20"

    dependencies {
        classpath("com.android.tools.build:gradle:8.12.3")
        classpath("org.jetbrains.kotlin:kotlin-gradle-plugin:$kotlinVersion")
    }
}

allprojects {
    repositories {
        mavenLocal {
            content {
                excludeGroup("com.github.xpenatan.jParser")
            }
        }
        google()
        mavenCentral()
        maven { url = uri("https://central.sonatype.com/repository/maven-snapshots/") }
        maven {
            url = uri("http://teavm.org/maven/repository/")
            isAllowInsecureProtocol = true
        }
    }

    configurations.configureEach {
        // Check for updates every sync
        resolutionStrategy.cacheChangingModulesFor(0, "seconds")
    }
}

easyPublishing {
    modules(
        ":webgpu:core",
        ":webgpu:shared:jni",
        ":webgpu:shared:c",
        ":webgpu:desktop:c",
        ":webgpu:desktop:jni",
        ":webgpu:desktop:ffm",
        ":webgpu:web:wasm",
        ":webgpu:android:jni",
    )

    groupId.set(LibExt.groupId)
    releaseVersion.set(LibExt.releaseVersion)
    snapshotVersion.set(LibExt.snapshotVersion)

    snapshotRepositoryUrl.set("https://central.sonatype.com/repository/maven-snapshots/")
    releaseRepositoryUrl.set("https://central.sonatype.com")
    username.set(providers.environmentVariable("CENTRAL_PORTAL_USERNAME"))
    password.set(providers.environmentVariable("CENTRAL_PORTAL_PASSWORD"))
    signingKey.set(providers.environmentVariable("SIGNING_KEY"))
    signingPassword.set(providers.environmentVariable("SIGNING_PASSWORD"))

    pomName.set(LibExt.libName)
    pomDescription.set("WebGPU Java Bindings for wgpu-native and Dawn")
    projectUrl.set("https://github.com/xpenatan/jWebGPU")

    developerId.set("Xpe")
    developerName.set("Natan")

    scmUrl.set("https://github.com/xpenatan/jWebGPU")
    scmConnection.set("scm:git:https://github.com/xpenatan/jWebGPU.git")
    scmDeveloperConnection.set("scm:git:ssh://git@github.com/xpenatan/jWebGPU.git")
}

afterEvaluate {
    // These projects share leaf names. Keep their Gradle component identities distinct while
    // their Maven publications continue to use the public jWebGPU group configured above.
    project(":webgpu:shared:jni").group = "${LibExt.groupId}.shared"
    project(":webgpu:desktop:jni").group = "${LibExt.groupId}.desktop"
    project(":webgpu:android:jni").group = "${LibExt.groupId}.android"
    project(":webgpu:shared:c").group = "${LibExt.groupId}.shared"
    project(":webgpu:desktop:c").group = "${LibExt.groupId}.desktop"
}
