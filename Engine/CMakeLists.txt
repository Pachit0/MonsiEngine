cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

add_library(Engine SHARED
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
 "src/Layer.cpp" "include/Layer.h" "include/LayerStack.h" "src/LayerStack.cpp" "ImGui/ImGuiLayer.cpp" "ImGui/ImGuiLayer.h" "include/Input.h" "Platform/Windows/WindowsInput.h" "Platform/Windows/WindowsInput.cpp" "include/MonsiKeyCodes.h" "include/MonsiMouseButtonCodes.h" "include/MonsiControllerButtonCodes.h")

if(WIN32)
    target_compile_definitions(Engine PRIVATE MONSI_PLATFORM_WINDOWS ENGINE_BUILD_DLL MONSI_ENABLE_ASSERTS GLFW_INCLUDE_NONE IMGUI_IMPL_OPENGL_LOADER_CUSTOM IMGUI_DISABLE_OBSOLETE_FUNCTIONS)
endif()

add_subdirectory(thirdparty)

target_include_directories(Engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Events
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
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

# Copy DLL to Sandbox
if(WIN32)
    add_custom_command(TARGET Engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Engine>
            $<TARGET_FILE_DIR:Sandbox>
    )
endif()

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()
