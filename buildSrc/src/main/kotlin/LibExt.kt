import java.io.File
import java.util.Properties

object LibExt {
    const val groupId = "com.github.xpenatan.jWebGPU"
    const val libName = "jWebGPU"
    const val snapshotVersion = "-SNAPSHOT"
    var isRelease = false
    val releaseVersion: String
        get() = readReleaseVersion()
    val libVersion: String
        get() {
            return if(isRelease) releaseVersion else snapshotVersion
        }

    const val javaMainTarget = "1.8"
    const val javaWebTarget = "17"
    const val javaFFMTarget = "25"

    //Library dependencies
    const val jParserVersion = "-SNAPSHOT"
    const val jMultiplatform = "0.1.3"

    //Example dependencies
    const val jUnitVersion = "4.12"

    const val exampleUseRepoLibs = false
}

private fun readReleaseVersion(): String {
    val file = File("gradle.properties")
    if(file.exists()) {
        val properties = Properties()
        properties.load(file.inputStream())
        return properties.getProperty("version")
            ?: throw RuntimeException("version property should exist")
    }
    throw RuntimeException("properties should exist")
}
