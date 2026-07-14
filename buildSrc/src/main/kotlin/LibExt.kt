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

    const val javaMainTarget = "1.8"
    const val javaWebTarget = "17"
    const val javaFFMTarget = "25"

    //Library dependencies
    const val jParserVersion = "1.2.3"
    const val jMultiplatform = "0.1.3"

    //Example dependencies
    const val jUnitVersion = "4.12"

    const val exampleUseRepoLibs = false
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
