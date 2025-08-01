import java.net.URLEncoder
import java.nio.file.Files
import java.nio.file.Paths

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
        classpath("com.android.tools.build:gradle:8.3.0")
        classpath("org.jetbrains.kotlin:kotlin-gradle-plugin:$kotlinVersion")
    }
}

allprojects()  {
    repositories {
        mavenLocal()
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

configure(allprojects) {

}

var libProjects = mutableSetOf(
    project(":webgpu:webgpu-core"),
    project(":webgpu:webgpu-desktop"),
    project(":webgpu:webgpu-teavm"),
    project(":webgpu:webgpu-android"),
)

configure(libProjects) {
    apply(plugin = "signing")
    apply(plugin = "maven-publish")

    publishing {
        repositories {
            maven {
                var isSnapshot = LibExt.libVersion.endsWith("-SNAPSHOT")
                isSnapshot = false;
                url = if (isSnapshot) {
                    uri("https://central.sonatype.com/repository/maven-snapshots/")
                } else {
                    uri(rootProject.layout.buildDirectory.dir("staging-deploy"))
                }
                if(isSnapshot) {
                    credentials {
                        username = System.getenv("CENTRAL_PORTAL_USERNAME")
                        password = System.getenv("CENTRAL_PORTAL_PASSWORD")
                    }
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

if(!LibExt.libVersion.endsWith("-SNAPSHOT")) {
    tasks.register<Zip>("zipStagingDeploy") {
        dependsOn(libProjects.map { it.tasks.named("publish") })
        from(rootProject.layout.buildDirectory.dir("staging-deploy"))
        archiveFileName.set("staging-deploy.zip")
        destinationDirectory.set(rootProject.layout.buildDirectory)
        onlyIf { !project.version.toString().endsWith("-SNAPSHOT") }
    }

    tasks.register("uploadToMavenCentral") {
        dependsOn("zipStagingDeploy")
        doLast {
            if (!project.version.toString().endsWith("-SNAPSHOT")) {
                // Define paths
                val stagingDir = rootProject.layout.buildDirectory.dir("staging-deploy").get().asFile
                val zipFile = rootProject.layout.buildDirectory.file("staging-deploy.zip").get().asFile

                if (!stagingDir.exists()) {
                    throw GradleException("Staging directory $stagingDir does not exist. Ensure the publish task ran successfully.")
                }

                if (!zipFile.exists()) {
                    throw GradleException("Zip file ${zipFile.absolutePath} was not created. Check the zip command output.")
                }

                if (!Files.isReadable(Paths.get(zipFile.absolutePath))) {
                    throw GradleException("Zip file ${zipFile.absolutePath} is not readable. Check file permissions.")
                }

                val username = System.getenv("CENTRAL_PORTAL_USERNAME") ?: throw GradleException("CENTRAL_PORTAL_USERNAME environment variable not set")
                val password = System.getenv("CENTRAL_PORTAL_PASSWORD") ?: throw GradleException("CENTRAL_PORTAL_PASSWORD environment variable not set")

                val rawBundleName = "${LibExt.libName}-${LibExt.libVersion}"
                val encodedBundleName = URLEncoder.encode(rawBundleName, "UTF-8")

                exec {
                    commandLine = listOf(
                        "curl",
                        "-u",
                        "$username:$password",
                        "--request",
                        "POST",
                        "--form",
                        "bundle=@${zipFile.absolutePath}",
                        "https://central.sonatype.com/api/v1/publisher/upload?name=${encodedBundleName}"
                    )
                }
            }
        }
    }
    tasks.named("publish") {
        finalizedBy(rootProject.tasks.named("uploadToMavenCentral"))
    }
}