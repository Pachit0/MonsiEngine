project(Sandbox2D)

file(GLOB SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_executable( Sandbox2D
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(Sandbox2D PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Sandbox2D PRIVATE 
    Engine
)

if(PRODUCTION_BUILD)	

    target_compile_definitions(Sandbox2D PUBLIC SHADER_PATH="./../assets/Shader") 
    target_compile_definitions(Sandbox2D PUBLIC TEXTURE_PATH="./../assets/Textures") 
    target_compile_definitions(Sandbox2D PUBLIC PRODUCTION_BUILD=1) 

else()

    target_compile_definitions(Sandbox2D PUBLIC SHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../assets/Shader/")
    target_compile_definitions(Sandbox2D PUBLIC TEXTURE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../assets/Textures/")

endif()