cmake_minimum_required(VERSION 3.16)

project(Engine)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Renderer/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImGui/*.cpp"
)

file(GLOB THIRD_PARTY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb_image/*.cpp"
)

set(ALL_SOURCES ${SOURCES} ${THIRD_PARTY_SOURCES})

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Renderer/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImGui/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/debug/*.h"
)

file(GLOB THIRD_PARTY_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb_image/*.h"
)

set(ALL_HEADERS ${HEADERS} ${THIRD_PARTY_HEADERS})

add_library(Engine STATIC ${ALL_SOURCES} ${ALL_HEADERS})

#add_library( Engine STATIC
#    "src/Engine.cpp"
#    "src/Logger.cpp"
#    "Core/Core.h"
#    "Core/EntryPoint.h"
#    "Core/Logger.h"
#    "Events/Events.h"
#    "Events/MouseEvent.h"
#    "Events/KeyEvent.h"
#    "Events/ApplicationEvent.h"
#    "Events/EventFormatter.h"
#    "MonsiPch.h"
#    "MonsiPch.cpp"
#    "Platform/Windows/PlatformWindows.cpp"
#    "Platform/Windows/PlatformWindows.h"
#    "Core/Window.h"
#    "Renderer/Shader.h"
#    "Renderer/Shader.cpp"
#    "Renderer/Buffers.h"
#    "Renderer/Buffers.cpp"
#    "src/Layer.cpp"
#    "Core/Layer.h" 
#    "Core/LayerStack.h" 
#    "src/LayerStack.cpp" 
#    "ImGui/ImGuiLayer.cpp" 
#    "ImGui/ImGuiLayer.h" 
#    "Core/Input.h"
#    "Platform/Windows/WindowsInput.h"
#    "Platform/Windows/WindowsInput.cpp"
#    "Core/MonsiKeyCodes.h"
#    "Core/MonsiMouseButtonCodes.h"
#    "Core/MonsiControllerButtonCodes.h"
#    "ImGui/ImGuiBuild.cpp"
#    "Renderer/RenderContext.h"
#    "Platform/OpenGL/OpenGLContext.h" 
#    "Platform/OpenGL/OpenGLContext.cpp"
#    "Renderer/Buffers.h" 
#    "Renderer/Buffers.cpp" 
#    "Platform/OpenGL/OpenGLBuffer.h"
#    "Platform/OpenGL/OpenGLBuffer.cpp"
#    "Renderer/Renderer.cpp"
#    "Renderer/Renderer.h"
#    "Platform/OpenGL/OpenGLShader.h"
#    "Platform/OpenGL/OpenGLShader.cpp" 
#    "Renderer/VertexArray.h" 
#    "Renderer/VertexArray.cpp"
#    "Platform/OpenGL/OpenGLVertexArray.h" 
#    "Platform/OpenGL/OpenGLVertexArray.cpp"
#    "Renderer/RendererAPI.h" 
#    "Renderer/RendererAPI.cpp"
#    "Renderer/RenderCommand.h" 
#    "Renderer/RenderCommand.cpp" 
#    "Platform/OpenGL/OpenGLRendererAPI.h" 
#    "Platform/OpenGL/OpenGLRendererAPI.cpp" 
#    "Renderer/OrthographicCamera.h" 
#    "Renderer/OrthographicCamera.cpp" 
#    "Core/TimeStep.h" 
#    "Renderer/Texture.h" 
#    "Renderer/Texture.cpp" 
#    "Platform/OpenGL/OpenGLTexture.h" 
#    "Platform/OpenGL/OpenGLTexture.cpp"
#    "thirdparty/stb_image/stb_image.cpp"    
#    "Renderer/OrthographicControl.h" 
#    "Renderer/OrthographicControl.cpp" 
#    "Renderer/Renderer2D.h" 
#    "Renderer/Renderer2D.cpp"   
#    "debug/instrumentor.h" 
#    "Renderer/Renderer3D.h" 
#    "Renderer/Renderer3D.cpp" 
#    "Renderer/PerspectiveCamera.h" 
#    "Renderer/PerspectiveCamera.cpp" 
#    "Renderer/PerspectiveControl.cpp" 
#    "Renderer/SubTexture2D.h"
#    "Renderer/SubTexture2D.cpp"
#    "Renderer/FrameBuffer.h"
#    "Renderer/FrameBuffer.cpp"
#    "Platform/OpenGL/OpenGLFrameBuffer.h"
#    "Platform/OpenGL/OpenGLFrameBuffer.cpp"
#)

if(WIN32)
    target_compile_definitions(Engine PRIVATE GLFW_INCLUDE_NONE MONSI_BUILD_DEBUG ENGINE_PROFILER)
endif()

add_subdirectory(thirdparty)

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Platform
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/Events
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
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
           implot
)

if(WIN32)
target_link_libraries(Engine PUBLIC opengl32)
endif()