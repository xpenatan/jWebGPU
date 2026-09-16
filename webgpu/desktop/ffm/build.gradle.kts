import buildlogic.configureDesktopNativeJars

plugins {
    id("java-library")
}

val moduleName = "webgpu-desktop-ffm"

base {
    archivesName.set(moduleName)
}

dependencies {
    implementation(libs.jParserRuntimeDesktopFFM)
    implementation(libs.jParserApiCore)
    implementation(libs.jParserLoaderCore)
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
    sourceCompatibility = JavaVersion.toVersion(libs.versions.javaFFM.get())
    targetCompatibility = JavaVersion.toVersion(libs.versions.javaFFM.get())
    withJavadocJar()
    withSourcesJar()
}

publishing {
    publications {
        create<MavenPublication>("maven") {
            artifactId = moduleName
            from(components["java"])
        }
    }
}

configureDesktopNativeJars(
    bridge = "ffm",
    artifactName = moduleName,
    jParserVersion = libs.versions.jParser.get()
)
