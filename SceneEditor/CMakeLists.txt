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
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/thirdparty
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/thirdparty/spdlog/include
)

target_link_libraries( Editor PRIVATE 
    Engine
)