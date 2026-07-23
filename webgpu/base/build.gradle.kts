plugins {
    id("java")
    id("java-library")
}

java {
    sourceCompatibility = JavaVersion.toVersion(libs.versions.javaMain.get())
    targetCompatibility = JavaVersion.toVersion(libs.versions.javaMain.get())
}

dependencies {
    implementation(libs.jParserLoaderCore)
    implementation(libs.jParserApiCore)
    implementation(libs.jParserRuntimeBase)
    implementation(libs.jParserRuntimeCore)
}
