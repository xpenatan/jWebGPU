include(":webgpu:webgpu-dawn")
include(":webgpu:webgpu-wgpu")
include(":webgpu:webgpu-base")
include(":webgpu:webgpu-build")
include(":webgpu:webgpu-core")
include(":webgpu:webgpu-desktop")
include(":webgpu:webgpu-teavm")

include(":demos:backend:core")
include(":demos:backend:desktop")
include(":demos:backend:teavm")
include(":demos:triangle:core")
include(":demos:triangle:desktop")
include(":demos:triangle:teavm")

//includeBuild("E:\\Dev\\Projects\\java\\jParser") {
//    dependencySubstitution {
//        substitute(module("com.github.xpenatan.jParser:jParser-base")).using(project(":jParser:base"))
//        substitute(module("com.github.xpenatan.jParser:jParser-build")).using(project(":jParser:builder"))
//        substitute(module("com.github.xpenatan.jParser:jParser-build-tool")).using(project(":jParser:builder-tool"))
//        substitute(module("com.github.xpenatan.jParser:jParser-core")).using(project(":jParser:core"))
//        substitute(module("com.github.xpenatan.jParser:jParser-cpp")).using(project(":jParser:cpp"))
//        substitute(module("com.github.xpenatan.jParser:jParser-idl")).using(project(":jParser:idl"))
//        substitute(module("com.github.xpenatan.jParser:jParser-teavm")).using(project(":jParser:teavm"))
//        substitute(module("com.github.xpenatan.jParser:loader-core")).using(project(":jParser:loader:loader-core"))
//        substitute(module("com.github.xpenatan.jParser:loader-teavm")).using(project(":jParser:loader:loader-teavm"))
//    }
//}