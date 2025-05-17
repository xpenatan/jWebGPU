import java.nio.file.Files
import java.nio.file.StandardCopyOption
import java.util.jar.JarFile
import kotlin.sequences.forEach

plugins {
    id("org.gretty") version("3.1.0")
    id("org.teavm") version("0.12.0")
}

gretty {
    contextPath = "/"
    extraResourceBase("build/dist/webapp")
}

dependencies {
    api(project(":demos:backend:core"))
    implementation(project(":webgpu:webgpu-core"))
    implementation(project(":webgpu:webgpu-teavm"))
    implementation("com.github.xpenatan:jMultiplatform:0.1.2")

    api("org.teavm:teavm-tooling:${LibExt.teaVMVersion}")
    api("org.teavm:teavm-core:${LibExt.teaVMVersion}")
    api("org.teavm:teavm-classlib:${LibExt.teaVMVersion}")
    api("org.teavm:teavm-jso:${LibExt.teaVMVersion}")
    api("org.teavm:teavm-jso-apis:${LibExt.teaVMVersion}")
    api("org.teavm:teavm-jso-impl:${LibExt.teaVMVersion}")
}
