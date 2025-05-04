plugins {
    id("org.gretty") version("3.1.0")
}

gretty {
    contextPath = "/"
    extraResourceBase("build/dist/webapp")
}

dependencies {
    implementation(project(":demos:triangle:core"))
    implementation(project(":webgpu:webgpu-teavm"))

    implementation("com.badlogicgames.gdx:gdx:${LibExt.gdxVersion}")
    implementation("com.github.xpenatan.gdx-teavm:backend-teavm:${LibExt.gdxTeaVMVersion}")
}

val mainClassName = "com.github.xpenatan.webgpu.demo.triangle.Build"

tasks.register<JavaExec>("webgpu_demo_triangle_build") {
    group = "webgpu_demo_teavm"
    description = "Build Demo"
    mainClass.set(mainClassName)
    classpath = sourceSets["main"].runtimeClasspath
}

tasks.register("webgpu_demo_triangle_run_teavm") {
    group = "webgpu_demo_teavm"
    description = "Run teavm app"
    val list = listOf("webgpu_demo_triangle_build", "jettyRun")
    dependsOn(list)

    tasks.findByName("jettyRun")?.mustRunAfter("webgpu_demo_triangle_build")
}