plugins {
    id("java")
    id("maven-publish")
    id("signing")
}

buildscript {
    repositories {
        mavenCentral()
        google()
    }

    val kotlinVersion = "2.1.10"

    dependencies {
        classpath("org.jetbrains.kotlin:kotlin-gradle-plugin:$kotlinVersion")
    }
}

allprojects()  {
    repositories {
        mavenLocal()
        google()
        mavenCentral()
        maven { url = uri("https://oss.sonatype.org/content/repositories/snapshots/") }
        maven { url = uri("https://oss.sonatype.org/content/repositories/releases/") }
        maven { url = uri("https://jitpack.io") }
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

configure(allprojects) {
    apply {
        plugin("java")
        plugin("java-library")
    }
    java.sourceCompatibility = JavaVersion.VERSION_11
    java.targetCompatibility = JavaVersion.VERSION_11
}

var libProjects = mutableSetOf(
    project(":webgpu:webgpu-core"),
    project(":webgpu:webgpu-desktop"),
    project(":webgpu:webgpu-teavm"),
)

configure(libProjects) {
    apply(plugin = "maven-publish")
    group = LibExt.groupId
    version = LibExt.libVersion
}

configure(libProjects) {
    apply(plugin = "signing")
    apply(plugin = "maven-publish")

    publishing {
        repositories {
            maven {
                url = uri {
                    val ver = project.version.toString()
                    val isSnapshot = ver.uppercase().contains("SNAPSHOT")
                    val repoUrl = "https://oss.sonatype.org/service/local/staging/deploy/maven2/"
                    val repoUrlSnapshot = "https://oss.sonatype.org/content/repositories/snapshots/"
                    if (isSnapshot) repoUrlSnapshot else repoUrl
                }
                credentials {
                    username = System.getenv("USER")
                    password = System.getenv("PASSWORD")
                }
            }
        }
    }

    tasks.withType<Javadoc> {
        options.encoding = "UTF-8"
        (options as StandardJavadocDocletOptions).addStringOption("Xdoclint:none", "-quiet")
    }

    publishing.publications.configureEach {
        if (this is MavenPublication) {
            pom {
                name.set("jWebGPU")
                description.set("Java native binding for WebGPU")
                url.set("https://github.com/xpenatan/jWebGPU")
                developers {
                    developer {
                        id.set("Xpe")
                        name.set("Natan")
                    }
                }
                scm {
                    connection.set("scm:git:git://https://github.com/xpenatan/jWebGPU.git")
                    developerConnection.set("scm:git:ssh://https://github.com/xpenatan/jWebGPU.git")
                    url.set("http://https://github.com/xpenatan/jWebGPU/tree/master")
                }
                licenses {
                    license {
                        name.set("The Apache License, Version 2.0")
                        url.set("https://www.apache.org/licenses/LICENSE-2.0.txt")
                    }
                }
            }
        }
    }

    val signingKey = System.getenv("SIGNING_KEY")
    val signingPassword = System.getenv("SIGNING_PASSWORD")
    if (signingKey != null && signingPassword != null) {
        signing {
            useInMemoryPgpKeys(signingKey, signingPassword)
            publishing.publications.configureEach {
                sign(this)
            }
        }
    }
}