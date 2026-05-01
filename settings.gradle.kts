include(":webgpu:webgpu-dawn") // Experimental module to build dawn source

include(":webgpu:webgpu-download") // Module to download the sources

include(":webgpu:webgpu-build")
include(":webgpu:webgpu-base")
include(":webgpu:webgpu-core")
include(":webgpu:webgpu-jni")
include(":webgpu:webgpu-ffm")
include(":webgpu:webgpu-web")
include(":webgpu:webgpu-android")

// A temp backend code for testing
include(":demos:backend:core")
include(":demos:backend:desktop")
include(":demos:backend:web")
include(":demos:backend:android")

include(":demos:app:core")
include(":demos:app:desktop-jni")
include(":demos:app:desktop-ffm")
include(":demos:app:web")
include(":demos:app:android")

//includeBuild("E:\\Dev\\Projects\\java\\jParser") {
//    dependencySubstitution {
//        substitute(module("com.github.xpenatan.jParser:jParser-base")).using(project(":jParser:jParser-base"))
//        substitute(module("com.github.xpenatan.jParser:jParser-build")).using(project(":jParser:jParser-build"))
//        substitute(module("com.github.xpenatan.jParser:jParser-build-tool")).using(project(":jParser:jParser-build-tool"))
//        substitute(module("com.github.xpenatan.jParser:jParser-core")).using(project(":jParser:jParser-core"))
//        substitute(module("com.github.xpenatan.jParser:jParser-jni")).using(project(":jParser:jParser-jni"))
//        substitute(module("com.github.xpenatan.jParser:jParser-ffm")).using(project(":jParser:jParser-ffm"))
//        substitute(module("com.github.xpenatan.jParser:jParser-idl")).using(project(":jParser:jParser-idl"))
//        substitute(module("com.github.xpenatan.jParser:jParser-web")).using(project(":jParser:jParser-web"))
//        substitute(module("com.github.xpenatan.jParser:api-core")).using(project(":idl:api:api-core"))
//        substitute(module("com.github.xpenatan.jParser:api-web")).using(project(":idl:api:api-web"))
//        substitute(module("com.github.xpenatan.jParser:runtime-core")).using(project(":idl:runtime:runtime-core"))
//        substitute(module("com.github.xpenatan.jParser:runtime-web")).using(project(":idl:runtime:runtime-web"))
//        substitute(module("com.github.xpenatan.jParser:runtime-jni")).using(project(":idl:runtime:runtime-jni"))
//        substitute(module("com.github.xpenatan.jParser:runtime-ffm")).using(project(":idl:runtime:runtime-ffm"))
//        substitute(module("com.github.xpenatan.jParser:runtime-android")).using(project(":idl:runtime:runtime-android"))
//        substitute(module("com.github.xpenatan.jParser:loader-core")).using(project(":loader:loader-core"))
//        substitute(module("com.github.xpenatan.jParser:loader-web")).using(project(":loader:loader-web"))
//    }
//}

//includeBuild("E:/Dev/Projects/java/teavm") {
//    dependencySubstitution {
//        substitute(module("org.teavm:teavm-tooling")).using(project(":tools:core"))
//        substitute(module("org.teavm:teavm-core")).using(project(":core"))
//        substitute(module("org.teavm:teavm-classlib")).using(project(":classlib"))
//        substitute(module("org.teavm:teavm-jso")).using(project(":jso:core"))
//        substitute(module("org.teavm:teavm-jso-apis")).using(project(":jso:apis"))
//        substitute(module("org.teavm:teavm-jso-impl")).using(project(":jso:impl"))
//        substitute(module("org.teavm:teavm-gradle-plugin")).using(project(":tools:gradle"))
//        substitute(module("org.teavm:teavm-junit")).using(project(":tools:junit"))
//    }
//}