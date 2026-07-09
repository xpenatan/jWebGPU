plugins {
    id("java-library")
}

val moduleName = "webgpu-jni"

base {
    archivesName.set(moduleName)
}

dependencies {
    api("com.github.xpenatan.jParser:runtime-jni:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:api-core:${LibExt.jParserVersion}")
    api("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")

    listOf("windows_x64", "linux_x64", "mac_x64", "mac_arm64").forEach { platform ->
        runtimeOnly("com.github.xpenatan.jParser:runtime-jni_$platform:${LibExt.jParserVersion}")
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
