project(Sandbox)

add_executable(Sandbox
    src/SandboxApp.cpp
)

target_include_directories(Sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Sandbox PRIVATE 
    Engine
    spdlog
)

if(WIN32)
    target_compile_definitions(Sandbox PRIVATE MONSI_PLATFORM_WINDOWS)
endif()