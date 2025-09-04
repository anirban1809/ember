cmake_minimum_required(VERSION 3.20)
project(OpenGLView LANGUAGES C CXX OBJC OBJCXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "" FORCE)

# Detect architecture
if(NOT CMAKE_OSX_ARCHITECTURES)
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "" FORCE)
  else()
    set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "" FORCE)
  endif()
endif()

# --- Node / N-API headers via cmake-js ---
include_directories(${CMAKE_JS_INC})
execute_process(
  COMMAND node -p "require('node-addon-api').include"
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  OUTPUT_VARIABLE NAPI_INC_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE "^-I" "" NAPI_INC_PATH "${NAPI_INC_PATH}")
separate_arguments(NAPI_INC_PATH)
include_directories(${NAPI_INC_PATH})
add_definitions(-DNAPI_VERSION=8 -DGL_SILENCE_DEPRECATION=1)

# --- Vendored libs ---
set(GLFW_ROOT "${CMAKE_SOURCE_DIR}/vendor/glfw-3.4.bin.MACOS")
set(GLEW_ROOT "${CMAKE_SOURCE_DIR}/vendor/glew-2.2.0")

set(GLFW_INCLUDE "${GLFW_ROOT}/include")
set(GLEW_INCLUDE "${GLEW_ROOT}/include")

set(GLFW_LIB "${GLFW_ROOT}/lib-arm64/libglfw.3.dylib")
set(GLEW_LIB "${GLEW_ROOT}/lib/libGLEW.2.2.0.dylib")

include_directories(${GLFW_INCLUDE} ${GLEW_INCLUDE})

# --- macOS frameworks ---
find_library(COCOA_FRAMEWORK      Cocoa)
find_library(QUARTZCORE_FRAMEWORK QuartzCore)
find_library(OPENGL_FRAMEWORK     OpenGL)
find_library(IOKIT_FRAMEWORK      IOKit)
find_library(COREVIDEO_FRAMEWORK  CoreVideo)

# --- Sources ---
set(SRC
  native/Core/Addon.cc
  native/renderer/gl_backend.cc
  native/Environment/MacOS/OpenGLView.mm
  native/Environment/MacOS/BridgeGLLayer.mm
)

set_source_files_properties(
  native/platform/mac/OpenGLView.mm
  PROPERTIES
  LANGUAGE OBJCXX
)

set_source_files_properties(
  native/platform/mac/BridgeGLLayer.m
  PROPERTIES
  LANGUAGE OBJCXX
)

# --- Build Node addon (.node) ---
add_library(OpenGLView MODULE ${SRC} ${CMAKE_JS_SRC})
set_target_properties(OpenGLView PROPERTIES
  PREFIX "" 
  SUFFIX ".node"
  OUTPUT_NAME "OpenGLView"
  OBJCXX_VISIBILITY_PRESET default
  BUILD_RPATH "@loader_path;@loader_path/..;@loader_path/../.."
  INSTALL_RPATH "@loader_path;@loader_path/..;@loader_path/../.."
)

# ARC for ObjC++
target_compile_options(OpenGLView PRIVATE $<$<COMPILE_LANGUAGE:OBJCXX>:-fobjc-arc>)

# Link frameworks + vendored dylibs
target_link_libraries(OpenGLView PRIVATE
  ${COCOA_FRAMEWORK}
  ${QUARTZCORE_FRAMEWORK}
  ${OPENGL_FRAMEWORK}
  ${IOKIT_FRAMEWORK}
  ${COREVIDEO_FRAMEWORK}
  "${GLEW_LIB}"
  "${GLFW_LIB}"
)

if(APPLE)
  target_link_options(OpenGLView PRIVATE
    "-undefined" "dynamic_lookup"
    "-ObjC" "-all_load"
  )
endif()

# --- Copy vendored dylibs next to the addon and rewrite install names ---
get_filename_component(GLEW_BASENAME "${GLEW_LIB}" NAME)
get_filename_component(GLFW_BASENAME "${GLFW_LIB}" NAME)

add_custom_command(TARGET OpenGLView POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Copying vendored dylibs to addon dir"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${GLEW_LIB}" "$<TARGET_FILE_DIR:OpenGLView>/${GLEW_BASENAME}"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${GLFW_LIB}" "$<TARGET_FILE_DIR:OpenGLView>/${GLFW_BASENAME}"

  COMMAND install_name_tool -id "@rpath/${GLEW_BASENAME}" "$<TARGET_FILE_DIR:OpenGLView>/${GLEW_BASENAME}"
  COMMAND install_name_tool -id "@rpath/${GLFW_BASENAME}" "$<TARGET_FILE_DIR:OpenGLView>/${GLFW_BASENAME}"

  COMMAND install_name_tool -change "${GLEW_LIB}" "@rpath/${GLEW_BASENAME}" "$<TARGET_FILE:OpenGLView>"
  COMMAND install_name_tool -change "${GLFW_LIB}" "@rpath/${GLFW_BASENAME}" "$<TARGET_FILE:OpenGLView>"
)
