cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(enduro2d)

#
# linking mode
#

if(MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" OFF)
    if(NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
        foreach(flag CMAKE_C_FLAGS
                     CMAKE_C_FLAGS_DEBUG
                     CMAKE_C_FLAGS_RELEASE
                     CMAKE_C_FLAGS_MINSIZEREL
                     CMAKE_C_FLAGS_RELWITHDEBINFO
                     CMAKE_CXX_FLAGS
                     CMAKE_CXX_FLAGS_DEBUG
                     CMAKE_CXX_FLAGS_RELEASE
                     CMAKE_CXX_FLAGS_MINSIZEREL
                     CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            if(${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
            if(${flag} MATCHES "/MDd")
                string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
            endif()
        endforeach()
    endif()
endif(MSVC)

#
# coverage mode
#

option(E2D_BUILD_WITH_COVERAGE "Build with coverage" OFF)
if(E2D_BUILD_WITH_COVERAGE AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_definitions(-DE2D_BUILD_WITH_COVERAGE)
    set(E2D_COVERAGE_FLAGS "--coverage")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${E2D_COVERAGE_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${E2D_COVERAGE_FLAGS}")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} ${E2D_COVERAGE_FLAGS}")
endif()

#
# sanitizer modes
#

option(E2D_BUILD_WITH_ASAN "Build with address sanitizer" OFF)
if(E2D_BUILD_WITH_ASAN AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_definitions(-DE2D_BUILD_WITH_ASAN)
    set(E2D_SANITIZER_FLAGS "-fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${E2D_SANITIZER_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${E2D_SANITIZER_FLAGS}")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} ${E2D_SANITIZER_FLAGS}")
endif()

option(E2D_BUILD_WITH_UBSAN "Build with undefined sanitizer" OFF)
if(E2D_BUILD_WITH_UBSAN AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_definitions(-DE2D_BUILD_WITH_UBSAN)
    set(E2D_SANITIZER_FLAGS "-fsanitize=undefined")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${E2D_SANITIZER_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${E2D_SANITIZER_FLAGS}")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} ${E2D_SANITIZER_FLAGS}")
endif()

#
# e2d sources
#

file(GLOB_RECURSE E2D_HEADERS
    headers/enduro2d/*.hpp
    headers/enduro2d/*.inl)

file(GLOB_RECURSE E2D_SOURCES
    sources/enduro2d/*.cpp
    sources/enduro2d/*.hpp
    headers/enduro2d/*.inl)

if(APPLE)
    file(GLOB_RECURSE E2D_SOURCES_MM
        sources/enduro2d/*.mm)
    list(APPEND E2D_SOURCES
        ${E2D_SOURCES_MM})
endif()

#
# e2d internal 3rd party
#

file(GLOB_RECURSE E2D_3RDPARTY
    headers/3rdparty/*.*
    sources/3rdparty/*.*)

#
# e2d external 3rd party
#

set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(modules/glfw)
set_target_properties(glfw PROPERTIES FOLDER modules)

set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(glew-cmake_BUILD_MULTI_CONTEXT OFF CACHE BOOL "" FORCE)
add_subdirectory(modules/glew)
set_target_properties(libglew_static PROPERTIES FOLDER modules)

#
# e2d library target
#

add_library(${PROJECT_NAME} STATIC
    ${E2D_HEADERS}
    ${E2D_SOURCES}
    ${E2D_3RDPARTY})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    ${E2D_HEADERS}
    ${E2D_SOURCES}
    ${E2D_3RDPARTY})

target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
    PRIVATE libglew_static
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:winmm.lib>)

target_include_directories(${PROJECT_NAME}
    PUBLIC headers
    PRIVATE sources)

target_compile_options(${PROJECT_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
        /W3 /MP>
    PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
        -Wall -Wextra -Wpedantic>
    PRIVATE
    $<$<PLATFORM_ID:Darwin>:
        -Wno-deprecated-declarations>)

target_compile_features(${PROJECT_NAME}
    PUBLIC cxx_std_17)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:
        -D_CRT_SECURE_NO_WARNINGS
        -D_SCL_SECURE_NO_WARNINGS>)

#
# subdirectories
#

option(E2D_BUILD_SAMPLES "Build samples" ON)
if(E2D_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

option(E2D_BUILD_UNTESTS "Build untests" ON)
if(E2D_BUILD_UNTESTS)
    enable_testing()
    add_subdirectory(untests)
endif()
