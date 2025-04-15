import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform

dependencies {
    implementation(project(":demos:triangle:core"))
    implementation(project(":webgpu:webgpu-desktop"))
}

val mainClassName = "com.github.xpenatan.webgpu.demo.triangle.Main"

tasks.register<JavaExec>("demo_triangle_desktop") {
    group = "demos"
    mainClass.set(mainClassName)
    classpath = sourceSets["main"].runtimeClasspath

    if(DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX) {
        jvmArgs("-XstartOnFirstThread")
    }
}