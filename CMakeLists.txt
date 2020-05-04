cmake_minimum_required(VERSION 3.14)

project(Alternite
    VERSION 0.1
    LANGUAGES CXX C
)

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "")
    set (CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug, Release, RelWithDebInfo, MinSizeRel" FORCE)
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

CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    GIT_TAG v1.5.0
    OPTIONS
        "SPDLOG_BUILD_EXAMPLE OFF"
)

CPMAddPackage(
    NAME glm
    GITHUB_REPOSITORY g-truc/glm
    GIT_TAG 0.9.9.8
)

CPMAddPackage(
    NAME freetype
    GITHUB_REPOSITORY aseprite/freetype2
    GIT_TAG VER-2-10-0
)

set (sources
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Core/Logger.cpp
    ${PROJECT_SOURCE_DIR}/src/Core/Input.cpp

    ${PROJECT_SOURCE_DIR}/src/Game/Game.cpp
    ${PROJECT_SOURCE_DIR}/src/Game/Player.cpp
    ${PROJECT_SOURCE_DIR}/src/Game/Ground.cpp

    ${PROJECT_SOURCE_DIR}/src/Renderer/Window.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/Renderer.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/Rendererable.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/Shader.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/Texture.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/Framebuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/Framebuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/Camera.cpp

    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/Font.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/distance-field.c
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/edtaa3func.c
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/texture-atlas.c
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/texture-font.c
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/utf8-utils.c
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/vector.c
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/platform.c
)

set (headers
    ${PROJECT_SOURCE_DIR}/src/Core/Scene.h
    ${PROJECT_SOURCE_DIR}/src/Core/Logger.h
    ${PROJECT_SOURCE_DIR}/src/Core/Common.h
    ${PROJECT_SOURCE_DIR}/src/Core/Input.h

    ${PROJECT_SOURCE_DIR}/src/Game/Game.h
    ${PROJECT_SOURCE_DIR}/src/Game/Player.h
    ${PROJECT_SOURCE_DIR}/src/Game/Ground.h
    ${PROJECT_SOURCE_DIR}/src/Game/Entity.h

    ${PROJECT_SOURCE_DIR}/src/Renderer/Window.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Renderer.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Rendererable.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Shader.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Texture.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Framebuffer.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Camera.h

    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/Font.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/distance-field.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/edtaa3func.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/texture-atlas.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/texture-font.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/utf8-utils.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/vector.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/vec234.h
    ${PROJECT_SOURCE_DIR}/src/Renderer/Text/platform.h
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
    PRIVATE spdlog
    PRIVATE freetype
)

target_include_directories(Alternite
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    PRIVATE ${glfw_SOURCE_DIR}/include
    PRIVATE ${glad_BINARY_DIR}/include
    PRIVATE ${freetype_SOURCE_DIR}/include
    PRIVATE ${spdlog_SOURCE_DIR}
    PRIVATE ${glm_SOURCE_DIR}
)

target_compile_definitions(Alternite
    PRIVATE GLFW_INCLUDE_NONE
)