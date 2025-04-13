cmake_minimum_required(VERSION 3.10)
project(WebGPUStaticLibs)

# Enable static library output
set(BUILD_SHARED_LIBS OFF)

# Add Dawn as a submodule
set(DAWN_FETCH_DEPENDENCIES ON)
add_subdirectory(build/dawn)

# Define static library for WebGPU
add_library(webgpu_static STATIC
  build/build/dawn/src/dawn/native/webgpu_absl_format.cpp
  build/build/dawn/src/dawn/native/webgpu_dawn.cpp
  # Add other source files as needed
)

# Link dependencies
target_link_libraries(webgpu_static PRIVATE
  dawn::webgpu_dawn
  dawn::dawn_proc
  dawn::dawn_common
  dawn::dawn_platform
)

# Include directories
target_include_directories(webgpu_static PUBLIC
  ${CMAKE_SOURCE_DIR}/build/dawn/include
  ${CMAKE_SOURCE_DIR}/build/dawn/out/Release/gen/include
)

# Platform-specific configurations
if(EMSCRIPTEN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_WEBGPU=1")
elseif(WIN32)
  target_link_libraries(webgpu_static PRIVATE dxgi d3d12)
elseif(APPLE)
  target_link_libraries(webgpu_static PRIVATE "-framework Metal -framework QuartzCore")
endif()