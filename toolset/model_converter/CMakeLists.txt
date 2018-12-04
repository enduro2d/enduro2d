#
# fetch 3rd party
#

include(FetchContent)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp)

FetchContent_GetProperties(assimp)

if(NOT assimp_POPULATED)
    FetchContent_Populate(assimp)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
    add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR})
endif()

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

target_include_directories(model_converter
    PRIVATE ${assimp_SOURCE_DIR}/include
    PRIVATE ${assimp_BINARY_DIR}/include)

set_target_properties(model_converter PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
