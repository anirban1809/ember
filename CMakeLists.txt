# cmake_minimum_required(VERSION 3.10)
# project(release)

# set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# # set(CMAKE_BUILD_TYPE Release)  # Ensures Release mode
# # set(CMAKE_CXX_FLAGS_RELEASE "-O3")  # Enable optimizations
# set(CMAKE_BUILD_TYPE Debug)  # Ensures Debug mode
# set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")  # Add debug symbols and disable optimizations

# set(LOCAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# set(GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/vendor/glew-2.2.0/include")
# set(IMGUI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/vendor/imgui")
# set(GLEW_LIBRARY "${CMAKE_SOURCE_DIR}/vendor/glew-2.2.0/lib/libGLEW.a")
# # Define GLFW Paths
# set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/vendor/glfw-3.4.bin.MACOS/include")
# set(GLFW_LIBRARY "${CMAKE_SOURCE_DIR}/vendor/glfw-3.4.bin.MACOS/lib-arm64/libglfw.3.dylib")

# # Include GLFW Headers
# include_directories(${GLFW_INCLUDE_DIR})
# include_directories(${GLEW_INCLUDE_DIR})
# include_directories(${LOCAL_INCLUDE_DIR})
# include_directories(${IMGUI_INCLUDE_DIR})

# # Add Executable
# file(GLOB_RECURSE SOURCES_LOCAL "./src/*.cpp")
# file(GLOB_RECURSE SOURCES_DEPS "${IMGUI_INCLUDE_DIR}/*.cpp")

# add_executable(release ${SOURCES_LOCAL} ${SOURCES_DEPS} "main.cpp")

# target_link_libraries(release
#     #link GLEW and GLFW
#     ${GLEW_LIBRARY}
#     ${GLFW_LIBRARY}
#     "-framework OpenGL"
# )


cmake_minimum_required(VERSION 3.14)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_FORCE_WARN_APPLE_SDK_AND_XCODE_CHECK ON CACHE BOOL "" FORCE)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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
