plugins {
    id("java-library")
}

val moduleName = "webgpu-jni"

base {
    archivesName.set(moduleName)
}

dependencies {
    api(libs.jParserRuntimeJni)
    api(libs.jParserApiCore)
    api(libs.jParserLoaderCore)
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
    sourceCompatibility = JavaVersion.toVersion(libs.versions.javaMain.get())
    targetCompatibility = JavaVersion.toVersion(libs.versions.javaMain.get())
}

java {
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
