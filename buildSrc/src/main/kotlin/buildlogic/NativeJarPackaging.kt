package buildlogic

import org.gradle.api.GradleException
import org.gradle.api.Project
import org.gradle.api.artifacts.Configuration
import org.gradle.api.artifacts.VersionCatalogsExtension
import org.gradle.api.attributes.Category
import org.gradle.api.attributes.LibraryElements
import org.gradle.api.attributes.Usage
import org.gradle.api.publish.PublishingExtension
import org.gradle.api.publish.maven.MavenPublication
import org.gradle.api.tasks.TaskProvider
import org.gradle.api.tasks.bundling.Jar
import org.gradle.kotlin.dsl.create
import org.gradle.kotlin.dsl.getByType
import org.gradle.kotlin.dsl.named
import org.gradle.kotlin.dsl.register
import java.io.File

/** Packages the existing JNI/FFM native outputs for each desktop platform and backend. */
fun Project.configureDesktopNativeJars(bridge: String, artifactName: String, jParserVersion: String) {
    val nativeRoot = rootProject.file("webgpu/builder/build/c++/libs")
    val platformPaths = linkedMapOf(
        "windows_x64" to "windows/vc/$bridge/jWebGPU64.dll",
        "linux_x64" to "linux/$bridge/libjWebGPU64.so",
        "mac_x64" to "mac/$bridge/libjWebGPU64.dylib",
        "mac_arm64" to "mac/arm/$bridge/libjWebGPUarm64.dylib"
    )
    val taskNames = gradle.startParameter.taskNames
    fun requested(name: String) = taskNames.any { it == name || it.endsWith(":$name") }
    // Local release preparation packages available outputs; a full publish still requires all platforms.
    val availableOnly = requested("prepareRelease") && !requested("publish") && !requested("publishRelease")
    val publications = extensions.getByType<PublishingExtension>().publications

    for ((platform, relativePath) in platformPaths) {
        for (backend in listOf("wgpu", "dawn")) {
            val suffix = "${backend}_$platform"
            val artifactId = "$artifactName-$suffix"
            val nativeFile = nativeRoot.resolve("$backend/$relativePath")
            val runtimeArtifact = "runtime-desktop-${bridge}_$platform"
            val nativeJar = tasks.register<Jar>("nativeJar_$suffix") {
                from(nativeFile) { into("native/$backend") }
                archiveBaseName.set(artifactId)
                archiveClassifier.set("")
                doFirst {
                    if (!nativeFile.isFile) {
                        throw GradleException("Missing desktop ${bridge.uppercase()} $backend native library for $platform: ${nativeFile.absolutePath}")
                    }
                }
            }

            nativeRuntime("nativeRuntime_$suffix", artifactId, nativeJar).dependencies.add(
                dependencies.create("com.github.xpenatan.jParser:$runtimeArtifact:$jParserVersion")
            )

            if (!availableOnly || nativeFile.isFile) {
                publications.create<MavenPublication>("mavenNative_$suffix") {
                    this.artifactId = artifactId
                    artifact(nativeJar)
                    pom.withXml {
                        val dependency = asNode().appendNode("dependencies").appendNode("dependency")
                        dependency.appendNode("groupId", "com.github.xpenatan.jParser")
                        dependency.appendNode("artifactId", runtimeArtifact)
                        dependency.appendNode("version", jParserVersion)
                        dependency.appendNode("scope", "runtime")
                    }
                }
            }
        }
    }
}

/** Exposes the same native artifact to includeBuild consumers, with its own capability. */
private fun Project.nativeRuntime(name: String, artifactName: String, jar: TaskProvider<Jar>): Configuration {
    val groupId = extensions.getByType<VersionCatalogsExtension>()
        .named("libs").findVersion("jWebGPUGroup").get().requiredVersion
    return configurations.create(name) {
        isCanBeConsumed = true
        isCanBeResolved = false
        attributes {
            attribute(Usage.USAGE_ATTRIBUTE, objects.named(Usage.JAVA_RUNTIME))
            attribute(Category.CATEGORY_ATTRIBUTE, objects.named(Category.LIBRARY))
            attribute(LibraryElements.LIBRARY_ELEMENTS_ATTRIBUTE, objects.named(LibraryElements.JAR))
        }
        outgoing.capability("$groupId:$artifactName:${project.version}")
        outgoing.artifact(jar)
    }
}
