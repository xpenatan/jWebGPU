import java.io.File
import java.util.*

object LibExt {
    const val groupId = "com.github.xpenatan.jWebGPU"
    const val libName = "jWebGPU"
    val libVersion: String = getVersion()

    //Library dependencies
    const val jParserVersion = "1.0.0-b11"
    const val teaVMVersion = "0.12.1"
    const val jMultiplatform = "0.1.3"

    //Example dependencies
    const val jUnitVersion = "4.12"

    const val exampleUseRepoLibs = true
}

private fun getVersion(): String {
    val isReleaseStr = System.getenv("RELEASE")
    val isRelease = isReleaseStr != null && isReleaseStr.toBoolean()
    var libVersion = "-SNAPSHOT"
    val file = File("gradle.properties")
    if(file.exists()) {
        val properties = Properties()
        properties.load(file.inputStream())
        val version = properties.getProperty("version")
        if(isRelease) {
            libVersion = version
        }
    }
    else {
        if(isRelease) {
            throw RuntimeException("properties should exist")
        }
    }
    println("Lib Version: $libVersion")
    return libVersion
}