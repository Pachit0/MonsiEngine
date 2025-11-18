add_library(Engine SHARED
    src/Engine.cpp
 "include/Core.h" "Monsi.h"  "include/EntryPoint.h" )

add_subdirectory(thirdparty)

target_include_directories(Engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(WIN32)
    target_compile_definitions(Engine PRIVATE ENGINE_BUILD_DLL)
endif()

target_link_libraries(Engine
    PUBLIC
        glad
        glfw
        imgui
        glm
        spdlog::spdlog
)

# Copy Engine.dll to Sandbox output folder after build
if(WIN32)
    add_custom_command(TARGET Engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Engine>
            $<TARGET_FILE_DIR:Sandbox>
    )
endif()