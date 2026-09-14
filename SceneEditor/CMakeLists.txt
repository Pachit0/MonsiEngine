cmake_minimum_required(VERSION 4.4)

project(Editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(USE_STATIC_C_RUNTIME OFF CACHE BOOL "" FORCE)

file(GLOB SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Panel/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Panel/*.h"
)

add_executable( Editor 
    ${SOURCES}
    ${HEADERS}
)

target_include_directories( Editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Panel
)

if(WIN32)
    target_compile_definitions(Editor PRIVATE MONSI_BUILD_DEBUG)
endif()

target_link_libraries( Editor PRIVATE 
    Engine
)

if(PRODUCTION_BUILD)

    target_compile_definitions(Editor PUBLIC SHADER_PATH="./assets/Shader")
    target_compile_definitions(Editor PUBLIC TEXTURE_PATH="./assets/Textures")
    target_compile_definitions(Editor PUBLIC MODEL_PATH="./assets/Models") 
    target_compile_definitions(Editor PUBLIC PRODUCTION_BUILD=1) 

else()

    target_compile_definitions(Editor PUBLIC SHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../Sandbox/assets/Shader/")
    target_compile_definitions(Editor PUBLIC TEXTURE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../Sandbox/assets/Textures/")
    target_compile_definitions(Editor PUBLIC MODEL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../Sandbox/assets/Models/")

endif()