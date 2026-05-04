project(Editor)

file(GLOB SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_executable( Editor 
    ${SOURCES}
    ${HEADERS}
)

target_include_directories( Editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries( Editor PRIVATE 
    Engine
)

if(PRODUCTION_BUILD)	

    target_compile_definitions(Editor PUBLIC SHADER_PATH="./assets/Shader") 
    target_compile_definitions(Editor PUBLIC TEXTURE_PATH="./assets/Textures") 
    target_compile_definitions(Editor PUBLIC PRODUCTION_BUILD=1) 

else()

    target_compile_definitions(Editor PUBLIC SHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../Sandbox/assets/Shader/")
    target_compile_definitions(Editor PUBLIC TEXTURE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../Sandbox/assets/Textures/")

endif()