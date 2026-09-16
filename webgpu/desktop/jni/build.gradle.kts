import buildlogic.configureDesktopNativeJars

plugins {
    id("java-library")
}

val moduleName = "webgpu-desktop-jni"

base {
    archivesName.set(moduleName)
}

dependencies {
    api(project(":webgpu:shared:jni"))

    testImplementation(libs.junit)
}

java {
    sourceCompatibility = JavaVersion.toVersion(libs.versions.javaMain.get())
    targetCompatibility = JavaVersion.toVersion(libs.versions.javaMain.get())
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
    bridge = "jni",
    artifactName = moduleName,
    jParserVersion = libs.versions.jParser.get()
)
