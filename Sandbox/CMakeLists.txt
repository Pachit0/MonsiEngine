project(Sandbox)

add_executable(Sandbox
    src/SandboxApp.cpp
 "include/ExampleLayer.h" "src/ExampleLayer.cpp")

target_include_directories(Sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/thirdparty
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/thirdparty/spdlog/include
)

target_link_libraries(Sandbox PRIVATE 
    Engine
)

if(WIN32)
    target_compile_definitions(Sandbox PRIVATE MONSI_PLATFORM_WINDOWS)
endif()