import org.teavm.gradle.api.SourceFilePolicy
import java.nio.file.Files
import java.nio.file.StandardCopyOption
import java.util.jar.JarFile
import kotlin.sequences.forEach

plugins {
    id("java")
    id("java-library")
    id("org.gretty") version("3.1.0")
    id("org.teavm") version("0.12.0")
}

java.sourceCompatibility = JavaVersion.VERSION_11
java.targetCompatibility = JavaVersion.VERSION_11

gretty {
    contextPath = "/"
    extraResourceBase("build/dist/webapp")
}

dependencies {
    implementation(project(":demos:triangle:core"))
    implementation(project(":demos:backend:teavm"))
}

teavm {
    js {
        outputDir = layout.buildDirectory.dir("dist/webapp").get().asFile
        relativePathInOutputDir = ""
        targetFileName = "app.js"
        sourceMap = true
        sourceFilePolicy = SourceFilePolicy.COPY
        obfuscated = false
        debugInformation = true
        addedToWebApp = true
        mainClass = "com.github.xpenatan.webgpu.demo.triangle.MainTeaVM"
    }

}

tasks.register<Copy>("copyWebappToDist") {
    group = "build"
    description = "Copies the contents of webapp to the build/dist folder"
    from("src/main/resources/webapp")
    into(layout.buildDirectory.dir("dist/webapp"))
    dependsOn("findAndCopyJavaScriptInClasspath")
}

tasks.register("findAndCopyJavaScriptInClasspath") {
    group = "build"
    description = "Finds JavaScript files from the webgpu-teavm dependency and copies them to build/dist."

    doLast {
        val distDir = layout.buildDirectory.dir("dist/webapp/scripts").get().asFile
        if (!distDir.exists()) distDir.mkdirs()

        val runtimeClasspath = configurations.findByName("runtimeClasspath")
        if (runtimeClasspath == null) {
            println("No runtimeClasspath configuration found.")
            return@doLast
        }

        // Only include files from the webgpu-teavm dependency
        val targetArtifact = "webgpu-teavm"

        val jsFiles = mutableListOf<File>()

        runtimeClasspath.resolve().forEach { file ->
            // Only process files/JARs whose name contains the target artifact
            if (!file.name.contains(targetArtifact)) return@forEach

            if (file.isDirectory) {
                file.walkTopDown()
                    .filter { it.isFile && it.extension == "js" }
                    .forEach { jsFile ->
                        val destFile = File(distDir, jsFile.name)
                        jsFile.copyTo(destFile, overwrite = true)
                        jsFiles.add(destFile)
                    }
            } else if (file.extension == "jar") {
                JarFile(file).use { jar ->
                    jar.entries().asSequence()
                        .filter { !it.isDirectory && it.name.endsWith(".js") }
                        .forEach { entry ->
                            val outFile = File(distDir, entry.name.substringAfterLast('/'))
                            jar.getInputStream(entry).use { input ->
                                Files.copy(input, outFile.toPath(), StandardCopyOption.REPLACE_EXISTING)
                            }
                            jsFiles.add(outFile)
                        }
                }
            }
        }

        if (jsFiles.isEmpty()) {
            println("No JavaScript files found in webgpu-teavm dependency.")
        } else {
            println("JavaScript files from webgpu-teavm copied to ${distDir.absolutePath}:")
            jsFiles.forEach { println(it) }
        }
    }
}

tasks.register("webgpu_demo_triangle_build_dist") {
    group = "webgpu_demo_teavm"
    description = "Generates JavaScript and copies webapp resources to build/dist"
    dependsOn("generateJavaScript", "copyWebappToDist")
    tasks.findByName("copyWebappToDist")?.mustRunAfter("generateJavaScript")
}

tasks.register("webgpu_demo_triangle_run_teavm") {
    group = "webgpu_demo_teavm"
    description = "Run teavm app"
    val list = listOf("webgpu_demo_triangle_build_dist", "jettyRun")
    dependsOn(list)
    tasks.findByName("jettyRun")?.mustRunAfter("webgpu_demo_triangle_build_dist")
}
