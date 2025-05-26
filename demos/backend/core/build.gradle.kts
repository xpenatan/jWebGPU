plugins {
    id("java")
    id("java-library")
}

java.sourceCompatibility = JavaVersion.VERSION_11
java.targetCompatibility = JavaVersion.VERSION_11

dependencies {
    api(project(":webgpu:webgpu-core"))
}
