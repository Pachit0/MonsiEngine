project(Sandbox)

file(GLOB SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_executable( Sandbox
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(Sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Sandbox PRIVATE 
    Engine
)

if(PRODUCTION_BUILD)	

    target_compile_definitions(Sandbox PUBLIC SHADER_PATH="./assets/Shader") 
    target_compile_definitions(Sandbox PUBLIC TEXTURE_PATH="./assets/Textures") 
    target_compile_definitions(Sandbox PUBLIC PRODUCTION_BUILD=1) 

else()

    target_compile_definitions(Sandbox PUBLIC SHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/Shader/")
    target_compile_definitions(Sandbox PUBLIC TEXTURE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/Textures/")

endif()