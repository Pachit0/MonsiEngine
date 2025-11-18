message(STATUS "Configuring third-party libraries...")

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# GLM
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

# ImGui
file(GLOB IMGUI_SRC
    imgui-docking/imgui.cpp
    imgui-docking/imgui_draw.cpp
    imgui-docking/imgui_widgets.cpp
    imgui-docking/imgui_tables.cpp
    imgui-docking/backends/imgui_impl_glfw.cpp
    imgui-docking/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
    imgui-docking
    imgui-docking/backends
)
target_link_libraries(imgui PUBLIC glfw glad)

#Glad
add_library(glad STATIC
    glad/src/glad.c
)
target_include_directories(glad PUBLIC glad/include)

# spdlog
add_subdirectory(spdlog)
