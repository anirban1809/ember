cmake_minimum_required(VERSION 3.14)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_FORCE_WARN_APPLE_SDK_AND_XCODE_CHECK ON CACHE BOOL "" FORCE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_BUILD_TYPE Release)  # Ensures Release mode
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")  # Enable optimizations
set(CMAKE_BUILD_TYPE Debug)  # Ensures Debug mode
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")  # Add debug symbols and disable optimizations

include_directories(
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/glew-2.2.0/include
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_SOURCE_DIR}/vendor/assimp/build/include
    ${CMAKE_SOURCE_DIR}/vendor/glfw-3.4.bin.MACOS/include
    ${CMAKE_SOURCE_DIR}/vendor/imgui    
    ${CMAKE_SOURCE_DIR}/vendor/stb
)

# Add internal projects
add_subdirectory(lib)
add_subdirectory(editor)
