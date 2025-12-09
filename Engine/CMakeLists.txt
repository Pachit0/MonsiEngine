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
)

if(WIN32)
    target_compile_definitions(Engine PRIVATE MONSI_PLATFORM_WINDOWS ENGINE_BUILD_DLL MONSI_ENABLE_ASSERTS)
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
