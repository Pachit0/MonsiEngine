project(Game)

file(GLOB SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../SceneEditor/Panel/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../SceneEditor/Panel/*.h"
)

add_executable( Game 
    ${SOURCES}
    ${HEADERS}
)

target_include_directories( Game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../SceneEditor/Panel
)

target_link_libraries( Game PRIVATE 
    Engine
)

if(MSVC)
    target_compile_options( Game PRIVATE /GR)
endif()

if(PRODUCTION_BUILD)	

    target_compile_definitions(Game PUBLIC SHADER_PATH="./assets/Shader") 
    target_compile_definitions(Game PUBLIC TEXTURE_PATH="./assets/Textures") 
    target_compile_definitions(Game PUBLIC MODEL_PATH="./assets/Models") 
    target_compile_definitions(Game PUBLIC PRODUCTION_BUILD=1) 

else()

    target_compile_definitions(Game PUBLIC SHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/Shader/")
    target_compile_definitions(Game PUBLIC TEXTURE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/Textures/")
    target_compile_definitions(Game PUBLIC MODEL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/Models/")

endif()