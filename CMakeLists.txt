cmake_minimum_required(VERSION 3.20)
project(MonsiEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Engine)
add_subdirectory(Sandbox)
add_subdirectory(SceneEditor)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_options(/arch:AVX2) #make sure SIMD optimizations take place
endif()

option(PRODUCTION_BUILD "Make this a production build" OFF)