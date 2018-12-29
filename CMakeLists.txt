cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(enduro2d)

#
# global warning mode
#

add_compile_options(
    $<$<PLATFORM_ID:Darwin>:-Wno-deprecated-declarations>)

#
# global linking mode
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
# global coverage mode
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
# global sanitizer mode
#

option(E2D_BUILD_WITH_SANITIZER "Build with sanitizer" OFF)
if(E2D_BUILD_WITH_SANITIZER AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_definitions(-DE2D_BUILD_WITH_SANITIZER)
    set(E2D_SANITIZER_FLAGS "-fno-omit-frame-pointer -fsanitize=address")
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

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

if(APPLE)
    find_library(Cocoa Cocoa)
    find_library(IOKit IOKit)
    find_library(CoreVideo CoreVideo)
    find_library(Foundation Foundation)
endif(APPLE)

set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(glew-cmake_BUILD_MULTI_CONTEXT OFF CACHE BOOL "" FORCE)
add_subdirectory(modules/glew)

set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(modules/glfw)

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

target_include_directories(${PROJECT_NAME}
    PRIVATE headers
    PRIVATE sources
    PRIVATE modules/glew/include
    PRIVATE modules/glfw/include)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

target_compile_options(${PROJECT_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4>
    PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
        -Wall -Wextra -Wpedantic>)

#
# e2d public variables
#

set(E2D_LIBRARIES
    enduro2d
    glfw
    libglew_static
    ${OPENGL_gl_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT})

if(APPLE)
    list(APPEND E2D_LIBRARIES
        ${Cocoa}
        ${IOKit}
        ${CoreVideo}
        ${Foundation})
endif(APPLE)

if(WIN32)
    list(APPEND E2D_LIBRARIES
        winmm.lib)
endif(WIN32)

set(E2D_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/headers)

get_directory_property(E2D_PARENT_DIRECTORY
    PARENT_DIRECTORY)
if(E2D_PARENT_DIRECTORY)
    set(E2D_LIBRARIES ${E2D_LIBRARIES} PARENT_SCOPE)
    set(E2D_INCLUDE_DIRS ${E2D_INCLUDE_DIRS} PARENT_SCOPE)
endif(E2D_PARENT_DIRECTORY)

#
# subdirectories
#

option(E2D_BUILD_SAMPLES "Build samples" ON)
if(E2D_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

option(E2D_BUILD_TOOLSET "Build toolset" ON)
if(E2D_BUILD_TOOLSET)
    add_subdirectory(toolset)
endif()

option(E2D_BUILD_UNTESTS "Build untests" ON)
if(E2D_BUILD_UNTESTS)
    enable_testing()
    add_subdirectory(untests)
endif()
