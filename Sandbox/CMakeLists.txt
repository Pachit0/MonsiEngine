project(Sandbox)

add_executable( Sandbox
    "src/SandboxApp.cpp"
    "src/ExampleLayer.cpp"
 "src/Sandbox2D.cpp" "include/Sandbox2D.h")

target_include_directories(Sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/thirdparty
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/thirdparty/spdlog/include
)

target_link_libraries(Sandbox PRIVATE 
    Engine
)