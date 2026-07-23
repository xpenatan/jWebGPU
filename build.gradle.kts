plugins {
    id("java")
    alias(libs.plugins.easyPublishing)
}

val jWebGPUGroup = "com.github.xpenatan.jWebGPU"
group = jWebGPUGroup

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

    groupId.set(jWebGPUGroup)
    releaseVersion.set(libs.versions.jWebGPU)
    snapshotVersion.set(libs.versions.snapshot)

    snapshotRepositoryUrl.set("https://central.sonatype.com/repository/maven-snapshots/")
    releaseRepositoryUrl.set("https://central.sonatype.com")
    username.set(providers.environmentVariable("CENTRAL_PORTAL_USERNAME"))
    password.set(providers.environmentVariable("CENTRAL_PORTAL_PASSWORD"))
    signingKey.set(providers.environmentVariable("SIGNING_KEY"))
    signingPassword.set(providers.environmentVariable("SIGNING_PASSWORD"))

    pomName.set("jWebGPU")
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
    project(":webgpu:shared:jni").group = "$jWebGPUGroup.shared"
    project(":webgpu:desktop:jni").group = "$jWebGPUGroup.desktop"
    project(":webgpu:android:jni").group = "$jWebGPUGroup.android"
    project(":webgpu:shared:c").group = "$jWebGPUGroup.shared"
    project(":webgpu:desktop:c").group = "$jWebGPUGroup.desktop"
}
