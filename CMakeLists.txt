cmake_minimum_required(VERSION 3.14)

project(Alternite
    VERSION 0.1
    LANGUAGES CXX
)

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "")
    set (CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug, Release, RelWithDebInfo, MinSizeRel")
endif()

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME glfw
    GITHUB_REPOSITORY glfw/glfw
    GIT_TAG 3.3.2
    OPTIONS
        "GLFW_BUILD_TESTS OFF"
        "GLFW_BUILD_EXAMPLES OFF"
        "GLFW_BUILD_DOCS OFF"
)

CPMAddPackage(
    NAME glad
    GITHUB_REPOSITORY Dav1dde/glad
    GIT_TAG v0.1.33
    OPTIONS 
        "GLAD_PROFILE core"
        "GLAD_API gl=4.6"
)

set (sources
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Alternite/Game.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/Window.cpp
)

set (headers
    ${PROJECT_SOURCE_DIR}/src/Alternite/Scene.h
    ${PROJECT_SOURCE_DIR}/src/Alternite/Game.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Window.h
)

add_executable(Alternite ${sources})
set_target_properties(Alternite
    PROPERTIES
    CXX_STANDARD 17
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    OUTPUT_NAME "main"
)

target_link_libraries(Alternite
    PRIVATE glfw
    PRIVATE glad
)

target_include_directories(Alternite
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    PRIVATE ${glfw_SOURCE_DIR}/include
    PRIVATE ${glad_BINARY_DIR}/include
)