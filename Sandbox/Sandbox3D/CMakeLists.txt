project(Sandbox3D)

#The Panel folder won't stay like that.
#In the future I will figure out how to make it a single Hierarchy Panel

file(GLOB SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Panel/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Panel/*.h"
)

add_executable( Sandbox3D
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(Sandbox3D PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Sandbox3D PRIVATE 
    Engine
)

if(PRODUCTION_BUILD)	

    target_compile_definitions(Sandbox3D PUBLIC SHADER_PATH="./../assets/Shader") 
    target_compile_definitions(Sandbox3D PUBLIC TEXTURE_PATH="./../assets/Textures") 
    target_compile_definitions(Sandbox3D PUBLIC MODEL_PATH="./../assets/Models")
    target_compile_definitions(Sandbox3D PUBLIC PRODUCTION_BUILD=1) 

else()

    target_compile_definitions(Sandbox3D PUBLIC SHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../assets/Shader/")
    target_compile_definitions(Sandbox3D PUBLIC TEXTURE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../assets/Textures/")
    target_compile_definitions(Sandbox3D PUBLIC MODEL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../assets/Models/")

endif()