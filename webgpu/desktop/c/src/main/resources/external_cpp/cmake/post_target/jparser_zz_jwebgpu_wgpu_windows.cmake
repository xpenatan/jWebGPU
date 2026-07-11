# This hook sorts after the generated jParser binding hooks. It supplies only
# the transitive dependencies needed by the Windows WGPU static library.
if(NOT DEFINED JPARSER_TEAVMC_APP_TARGET OR "${JPARSER_TEAVMC_APP_TARGET}" STREQUAL "")
  if(DEFINED TEAVM_APP_TARGET AND NOT "${TEAVM_APP_TARGET}" STREQUAL "")
    set(JPARSER_TEAVMC_APP_TARGET "${TEAVM_APP_TARGET}")
  else()
    message(FATAL_ERROR "Set JPARSER_TEAVMC_APP_TARGET before including the jWebGPU WGPU dependency hook.")
  endif()
endif()

if(NOT DEFINED JPARSER_JWEBGPU_TEAVMC_LINKAGE OR "${JPARSER_JWEBGPU_TEAVMC_LINKAGE}" STREQUAL "")
  message(FATAL_ERROR "Include jparser_jwebgpu_teavm_c.cmake before the jWebGPU WGPU dependency hook.")
endif()

if(JPARSER_JWEBGPU_TEAVMC_LINKAGE STREQUAL "STATIC" AND WIN32)
  if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "The packaged jWebGPU WGPU TeaVM C static payload supports Windows x64 only.")
  endif()

  if(NOT DEFINED JPARSER_JWEBGPU_TEAVMC_ROOT OR "${JPARSER_JWEBGPU_TEAVMC_ROOT}" STREQUAL "")
    get_filename_component(JPARSER_JWEBGPU_TEAVMC_EXTERNAL_CPP_ROOT
      "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
    set(JPARSER_JWEBGPU_TEAVMC_ROOT
      "${JPARSER_JWEBGPU_TEAVMC_EXTERNAL_CPP_ROOT}/jparser/jwebgpu")
  endif()

  if(NOT DEFINED JPARSER_JWEBGPU_WGPU_NATIVE_LIBRARY
      OR "${JPARSER_JWEBGPU_WGPU_NATIVE_LIBRARY}" STREQUAL "")
    set(JPARSER_JWEBGPU_WGPU_NATIVE_LIBRARY
      "${JPARSER_JWEBGPU_TEAVMC_ROOT}/native/windows_x64/deps/wgpu_native.lib")
  endif()
  if(NOT EXISTS "${JPARSER_JWEBGPU_WGPU_NATIVE_LIBRARY}")
    message(FATAL_ERROR
      "Missing packaged wgpu_native static library: ${JPARSER_JWEBGPU_WGPU_NATIVE_LIBRARY}")
  endif()

  if(NOT DEFINED JPARSER_JWEBGPU_WGPU_SYSTEM_LIBRARIES)
    set(JPARSER_JWEBGPU_WGPU_SYSTEM_LIBRARIES
      onecore_apiset.lib
      dxguid.lib
      ws2_32.lib
      userenv.lib
      ntdll.lib
      opengl32.lib
      d3dcompiler.lib
      ole32.lib
      propsys.lib
      runtimeobject.lib
      user32.lib
      gdi32.lib
      oleaut32.lib)
  endif()

  target_link_libraries(${JPARSER_TEAVMC_APP_TARGET} PRIVATE
    "${JPARSER_JWEBGPU_WGPU_NATIVE_LIBRARY}"
    ${JPARSER_JWEBGPU_WGPU_SYSTEM_LIBRARIES})
endif()
