cmake_minimum_required(VERSION 3.16)

project(Engine)

set(CMAKE_CXX_STANDARD 17)

add_library(Engine STATIC
    src/Engine.cpp
    src/Logger.cpp
    include/Core.h
    include/EntryPoint.h
    include/Logger.h
    Events/Events.h
    Events/MouseEvent.h
    Events/KeyEvent.h
    Events/ApplicationEvent.h
    Events/EventFormatter.h
    MonsiPch.h
    MonsiPch.cpp
    Platform/Windows/PlatformWindows.cpp
    Platform/Windows/PlatformWindows.h
    include/Window.h
    Renderer/Shader.h
    Renderer/Shader.cpp
    Renderer/Buffers.h
    Renderer/Buffers.cpp
    "src/Layer.cpp" "include/Layer.h" "include/LayerStack.h" "src/LayerStack.cpp" "ImGui/ImGuiLayer.cpp" "ImGui/ImGuiLayer.h" "include/Input.h" "Platform/Windows/WindowsInput.h" "Platform/Windows/WindowsInput.cpp" "include/MonsiKeyCodes.h" "include/MonsiMouseButtonCodes.h" "include/MonsiControllerButtonCodes.h" "ImGui/ImGuiBuild.cpp" "Renderer/RenderContext.h" "Platform/OpenGL/OpenGLContext.h" "Platform/OpenGL/OpenGLContext.cpp" "Renderer/Buffers.h" "Renderer/Buffers.cpp" "Platform/OpenGL/OpenGLBuffer.h" "Platform/OpenGL/OpenGLBuffer.cpp" "Renderer/Renderer.cpp" "Renderer/Renderer.h" "Platform/OpenGL/OpenGLShader.h" "Platform/OpenGL/OpenGLShader.cpp" "Renderer/VertexArray.h" "Renderer/VertexArray.cpp" "Platform/OpenGL/OpenGLVertexArray.h" "Platform/OpenGL/OpenGLVertexArray.cpp")

if(WIN32)
    target_compile_definitions(Engine PRIVATE MONSI_PLATFORM_WINDOWS GLFW_INCLUDE_NONE MONSI_BUILD_DEBUG)
endif()

add_subdirectory(thirdparty)

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Events
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
)

target_precompile_headers(Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/MonsiPch.h)

target_link_libraries(Engine
    PUBLIC 
           glad
           glm
           spdlog::spdlog_header_only
           imgui
           glfw
)

if(WIN32)
target_link_libraries(Engine PUBLIC opengl32)
endif()

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()