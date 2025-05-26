plugins {
    id("java")
    id("java-library")
}

java.sourceCompatibility = JavaVersion.VERSION_11
java.targetCompatibility = JavaVersion.VERSION_11

dependencies {
    implementation("com.github.xpenatan.jParser:jParser-base:${LibExt.jParserVersion}")
    implementation("com.github.xpenatan.jParser:loader-core:${LibExt.jParserVersion}")
}