include(":webgpu:webgpu-dawn") // Experimental module to build dawn source

include(":webgpu:webgpu-download") // Module to download the sources

include(":webgpu:webgpu-build")
include(":webgpu:webgpu-base")
include(":webgpu:webgpu-core")
include(":webgpu:webgpu-desktop")
include(":webgpu:webgpu-teavm")
include(":webgpu:webgpu-android")

// A temp backend code for testing
include(":demos:backend:core")
include(":demos:backend:desktop")
include(":demos:backend:teavm")
include(":demos:backend:android")

include(":demos:app:core")
include(":demos:app:desktop")
include(":demos:app:teavm")
include(":demos:app:android")

//includeBuild("E:\\Dev\\Projects\\java\\jParser") {
//    dependencySubstitution {
//        substitute(module("com.github.xpenatan.jParser:jParser-base")).using(project(":jParser:jParser-base"))
//        substitute(module("com.github.xpenatan.jParser:jParser-build")).using(project(":jParser:jParser-build"))
//        substitute(module("com.github.xpenatan.jParser:jParser-build-tool")).using(project(":jParser:jParser-build-tool"))
//        substitute(module("com.github.xpenatan.jParser:jParser-core")).using(project(":jParser:jParser-core"))
//        substitute(module("com.github.xpenatan.jParser:jParser-cpp")).using(project(":jParser:jParser-cpp"))
//        substitute(module("com.github.xpenatan.jParser:jParser-idl")).using(project(":jParser:jParser-idl"))
//        substitute(module("com.github.xpenatan.jParser:jParser-teavm")).using(project(":jParser:jParser-teavm"))
//        substitute(module("com.github.xpenatan.jParser:idl-core")).using(project(":idl:idl-core"))
//        substitute(module("com.github.xpenatan.jParser:idl-teavm")).using(project(":idl:idl-teavm"))
//        substitute(module("com.github.xpenatan.jParser:loader-core")).using(project(":loader:loader-core"))
//        substitute(module("com.github.xpenatan.jParser:loader-teavm")).using(project(":loader:loader-teavm"))
//    }
//}