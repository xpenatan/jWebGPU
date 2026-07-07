plugins {
    id("java-library")
}

val moduleName = "webgpu-jni"

base {
    archivesName.set(moduleName)
}

val taskNames = gradle.startParameter.taskNames
fun isTaskRequested(taskName: String): Boolean {
    return taskNames.any { it == taskName || it.endsWith(":$taskName") }
}
val isPrepareDeployTask = isTaskRequested("prepareReleaseDeploy") || isTaskRequested("prepareSnapshotDeploy")
val isPublishTask = taskNames.any { it.contains("publish", ignoreCase = true) }
val isPublicationMetadataTask = taskNames.any {
    it.contains("Publication", ignoreCase = true)
            && (it.contains("generatePom", ignoreCase = true) || it.contains("generateMetadata", ignoreCase = true))
}
val includeDesktopNativeArtifactDependencies = isPrepareDeployTask || isPublishTask || isPublicationMetadataTask

dependencies {
    api("com.github.xpenatan.jParser:runtime-jni:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:api-core:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")

    listOf("windows_x64", "linux_x64", "mac_x64", "mac_arm64").forEach { platform ->
        runtimeOnly("com.github.xpenatan.jParser:runtime-jni_$platform:${LibExt.jParserVersion}")
        if(includeDesktopNativeArtifactDependencies) {
            runtimeOnly("${LibExt.groupId}:webgpu-desktop-jni_$platform:${LibExt.libVersion}")
        }
    }
}

sourceSets {
    main {
        java.setSrcDirs(listOf("src/main/java"))
    }
}

tasks.named("clean") {
    doFirst {
        project.delete(files("$projectDir/src/main/java"))
    }
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
    targetCompatibility = JavaVersion.toVersion(LibExt.javaMainTarget)
}

java {
    withJavadocJar()
    withSourcesJar()
}

publishing {
    publications {
        create<MavenPublication>("maven") {
            artifactId = moduleName
            groupId = LibExt.groupId
            version = LibExt.libVersion
            from(components["java"])
        }
    }
}
