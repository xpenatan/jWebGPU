import java.io.File
import java.util.*

object LibExt {
    const val groupId = "com.github.xpenatan.jWebGPU"
    const val libName = "jWebGPU"
    var isRelease = false
    var libVersion: String = ""
        get() {
            return getVersion()
        }

    const val java8Target = "1.8"
    const val java11Target = "11"

    //Library dependencies
    const val jParserVersion = "1.0.0-b24"
    const val teaVMVersion = "0.12.3"
    const val jMultiplatform = "0.1.3"

    //Example dependencies
    const val jUnitVersion = "4.12"

    const val exampleUseRepoLibs = true
}

private fun getVersion(): String {
    var libVersion = "-SNAPSHOT"
    val file = File("gradle.properties")
    if(file.exists()) {
        val properties = Properties()
        properties.load(file.inputStream())
        val version = properties.getProperty("version")
        if(LibExt.isRelease) {
            libVersion = version
        }
    }
    else {
        if(LibExt.isRelease) {
            throw RuntimeException("properties should exist")
        }
    }
    return libVersion
}
