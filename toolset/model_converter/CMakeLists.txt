#
# external 3rd party
#

set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
add_subdirectory(modules/assimp)

#
# model converter executable
#

file(GLOB MODEL_CONVERTER_SOURCES
    sources/*.*)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    ${MODEL_CONVERTER_SOURCES})

add_executable(model_converter
    ${MODEL_CONVERTER_SOURCES})

target_link_libraries(model_converter
    assimp)

set_target_properties(model_converter PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
