add_library(Engine SHARED
    src/Engine.cpp
    src/Logger.cpp
    "include/Core.h" 
    "Monsi.h"
    "include/EntryPoint.h"  
    "include/Logger.h" 
    "Events/Events.h" 
    "Events/MouseEvent.h" 
    "Events/KeyEvent.h" 
    "Events/ApplicationEvent.h" 
    "Events/EventFormatter.h" 
    "mopch.h" 
    "mopch.cpp"
)
 
if(WIN32)
    target_compile_definitions(Engine PRIVATE MONSI_PLATFORM_WINDOWS ENGINE_BUILD_DLL)
endif()

add_subdirectory(thirdparty)

target_include_directories(Engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Events
)

target_precompile_headers(Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/mopch.h)

target_link_libraries(Engine
    PUBLIC
        glad
        glfw
        imgui
        glm
        spdlog::spdlog_header_only
)

# Copy Engine.dll to Sandbox output folder after build
if(WIN32)
    add_custom_command(TARGET Engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Engine>
            $<TARGET_FILE_DIR:Sandbox>
    )
endif()