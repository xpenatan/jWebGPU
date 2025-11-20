import java.nio.file.Files
import java.nio.file.StandardCopyOption
import java.util.jar.JarFile
import kotlin.sequences.forEach

plugins {
    id("java")
    id("java-library")
    id("org.gretty") version("4.1.10")
}

project.extra["webAppDir"] = File(projectDir, "build/dist/webapp")
gretty {
    contextPath = "/"
}

dependencies {
    implementation(project(":demos:app:core"))
    implementation(project(":demos:backend:teavm"))
}

java {
    sourceCompatibility = JavaVersion.toVersion(LibExt.java11Target)
    targetCompatibility = JavaVersion.toVersion(LibExt.java11Target)
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

tasks.register<JavaExec>("webgpu_demo_app_build") {
    group = "webgpu_demo_teavm"
    description = "Generates JavaScript and copies webapp resources to build/dist"
    mainClass.set("com.github.xpenatan.webgpu.demo.app.Build")
    classpath = sourceSets["main"].runtimeClasspath
    dependsOn("copyWebappToDist")
}

tasks.register("webgpu_demo_app_run_teavm") {
    group = "webgpu_demo_teavm"
    description = "Run teavm app"
    val list = listOf("webgpu_demo_app_build", "jettyRun")
    dependsOn(list)
    tasks.findByName("jettyRun")?.mustRunAfter("webgpu_demo_app_build")
}

afterEvaluate {
    tasks.named("prepareInplaceWebAppFolder").configure {
        dependsOn("copyWebappToDist")
    }
}
