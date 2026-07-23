import java.nio.file.Files
import java.nio.file.StandardCopyOption
import java.util.jar.JarFile
import kotlin.sequences.forEach

plugins {
    id("java")
    id("java-library")
    alias(libs.plugins.gretty)
}

project.extra["webAppDir"] = File(projectDir, "build/dist/webapp")
gretty {
    contextPath = "/"
}

dependencies {
    implementation(project(":demos:app:core"))
    implementation(project(":demos:backend:web"))
}

java {
    sourceCompatibility = JavaVersion.toVersion(libs.versions.javaWeb.get())
    targetCompatibility = JavaVersion.toVersion(libs.versions.javaWeb.get())
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
    description = "Finds JavaScript files from the webgpu-web dependency and copies them to build/dist."

    doLast {
        val distDir = layout.buildDirectory.dir("dist/webapp/scripts").get().asFile
        if (!distDir.exists()) distDir.mkdirs()

        val runtimeClasspath = configurations.findByName("runtimeClasspath")
        if (runtimeClasspath == null) {
            println("No runtimeClasspath configuration found.")
            return@doLast
        }

        // Only include files from the webgpu-web dependency
        val targetArtifact = "webgpu-web"

        val jsFiles = mutableListOf<File>()

        runtimeClasspath.resolve().forEach { file ->
            // Only process files/JARs whose name contains the target artifact
            println("Checking classpath file: ${file.name}")
            if (!(file.name.contains(targetArtifact) || file.name.contains("runtime"))) return@forEach

            if (file.isDirectory) {
                file.walkTopDown()
                    .filter { it.isFile && (it.extension == "js" || it.extension == "wasm") }
                    .forEach { jsFile ->
                        val destFile = File(distDir, jsFile.name)
                        jsFile.copyTo(destFile, overwrite = true)
                        jsFiles.add(destFile)
                    }
            } else if (file.extension == "jar") {
                JarFile(file).use { jar ->
                    jar.entries().asSequence()
                        .filter { !it.isDirectory && (it.name.endsWith(".js") || it.name.endsWith(".wasm")) }
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
            println("No JavaScript files found in webgpu-web dependency.")
        } else {
            println("JavaScript files from webgpu-web copied to ${distDir.absolutePath}:")
            jsFiles.forEach { println(it) }
        }
    }
}

tasks.register<JavaExec>("webgpu_demo_app_build") {
    group = "webgpu_demo_web"
    description = "Generates JavaScript and copies webapp resources to build/dist"
    mainClass.set("com.github.xpenatan.webgpu.demo.app.Build")
    classpath = sourceSets["main"].runtimeClasspath
    finalizedBy("copyWebappToDist")
}

tasks.register("webgpu_demo_app_web_run") {
    group = "webgpu_demo_web"
    description = "Run web app"
    val list = listOf("webgpu_demo_app_build", "jettyRun")
    dependsOn(list)
    tasks.findByName("jettyRun")?.mustRunAfter("webgpu_demo_app_build")
    tasks.findByName("jettyRun")?.mustRunAfter("copyWebappToDist")
}

afterEvaluate {
    tasks.named("prepareInplaceWebAppFolder").configure {
        dependsOn("copyWebappToDist")
    }
}
