cmake_minimum_required(VERSION 3.16)

project(Engine)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Scene/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Renderer/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImGui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ModelLoader/*.cpp"
)

file(GLOB THIRD_PARTY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb_image/*.cpp"
)

set(ALL_SOURCES ${SOURCES} ${THIRD_PARTY_SOURCES})

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Scene/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Renderer/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImGui/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/debug/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ModelLoader/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/*.h"
)

file(GLOB THIRD_PARTY_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb_image/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/entt/entt.hpp"
)

set(ALL_HEADERS ${HEADERS} ${THIRD_PARTY_HEADERS})

add_library(Engine STATIC ${ALL_SOURCES} ${ALL_HEADERS})

target_compile_definitions(Engine PRIVATE GLFW_INCLUDE_NONE MONSI_BUILD_DEBUG ENGINE_PROFILER)

add_subdirectory(thirdparty)

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Platform
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelLoader
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/Events
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/entt
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/include
)

target_precompile_headers(Engine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/MonsiPch.h")

target_link_libraries(Engine
    PUBLIC 
           glad
           glm
           spdlog::spdlog_header_only
           imgui
           glfw
           stb_image
           entt
           implot
           assimp
)

if(WIN32)
    target_link_libraries(Engine PUBLIC opengl32)
else(LINUX)
    target_link_libraries(Engine PUBLIC GL ${X11_LIBRARIES})
endif()

if(PRODUCTION_BUILD)	

    target_compile_definitions(Engine PUBLIC SHADER_PATH="./assets/Shader")
    target_compile_definitions(Engine PUBLIC TEXTURE_PATH="./assets/Textures")
    target_compile_definitions(Engine PUBLIC MODEL_PATH="./assets/Models")
    target_compile_definitions(Engine PUBLIC FONT_PATH="./assets/Fonts")
    target_compile_definitions(Engine PUBLIC PRODUCTION_BUILD=1) 

else()

    target_compile_definitions(Engine PUBLIC SHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../Sandbox/assets/Shader/")
    target_compile_definitions(Engine PUBLIC TEXTURE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../Sandbox/assets/Textures/")
    target_compile_definitions(Engine PUBLIC MODEL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../Sandbox/assets/Models/")
    target_compile_definitions(Engine PUBLIC FONT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../Sandbox/assets/Fonts/")

endif()