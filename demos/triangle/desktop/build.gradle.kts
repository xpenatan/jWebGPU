import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform

val lwjglVersion = "3.3.3"
val lwjglNatives = "natives-windows"


dependencies {
    implementation(project(":demos:triangle:core"))
    implementation(project(":demos:backend:desktop"))
}

val mainClassName = "com.github.xpenatan.webgpu.demo.triangle.MainDesktop"

tasks.register<JavaExec>("webgpu_demo_triangle_desktop") {
    group = "demos"
    mainClass.set(mainClassName)
    classpath = sourceSets["main"].runtimeClasspath

    if(DefaultNativePlatform.getCurrentOperatingSystem().isMacOsX) {
        jvmArgs("-XstartOnFirstThread")
    }
}