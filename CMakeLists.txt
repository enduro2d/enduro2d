cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

if(NOT DEFINED PROJECT_NAME)
    set(E2D_BUILD_AS_STANDALONE ON)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14"
        CACHE STRING "Minimum OS X version to target for deployment" FORCE)
endif()

project(enduro2d)

set(E2D_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
set(E2D_ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT E2D_BUILD_AS_STANDALONE)
    set(E2D_SYSTEM_NAME ${E2D_SYSTEM_NAME} PARENT_SCOPE)
    set(E2D_ROOT_DIRECTORY ${E2D_ROOT_DIRECTORY} PARENT_SCOPE)
endif()

set(E2D_ALL_C_CONFIGS "CMAKE_C_FLAGS")
set(E2D_ALL_CXX_CONFIGS "CMAKE_CXX_FLAGS")

if(CMAKE_BUILD_TYPE)
    string(TOUPPER ${CMAKE_BUILD_TYPE} build_type)
    set(E2D_ALL_C_CONFIGS "${E2D_ALL_C_CONFIGS}" "CMAKE_C_FLAGS_${build_type}")
    set(E2D_ALL_CXX_CONFIGS "${E2D_ALL_CXX_CONFIGS}" "CMAKE_CXX_FLAGS_${build_type}")
endif()

foreach(type ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${type} build_type)
    set(E2D_ALL_C_CONFIGS "${E2D_ALL_C_CONFIGS}" "CMAKE_C_FLAGS_${build_type}")
    set(E2D_ALL_CXX_CONFIGS "${E2D_ALL_CXX_CONFIGS}" "CMAKE_CXX_FLAGS_${build_type}")
endforeach()

#
# linking mode
#

if(MSVC)
    option(E2D_BUILD_WITH_STATIC_CRT "Use static C runtime library" ON)
    if(E2D_BUILD_WITH_STATIC_CRT)
        foreach(flag ${E2D_ALL_C_CONFIGS} ${E2D_ALL_CXX_CONFIGS})
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

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(modules/defer.hpp)
add_subdirectory(modules/ecs.hpp)
add_subdirectory(modules/enum.hpp)
add_subdirectory(modules/flat.hpp)
add_subdirectory(modules/promise.hpp)

set(GLFW_INSTALL OFF CACHE INTERNAL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)

add_subdirectory(modules/glfw)
set_target_properties(glfw PROPERTIES FOLDER modules)

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
    PUBLIC defer.hpp
    PUBLIC ecs.hpp
    PUBLIC enum.hpp
    PUBLIC flat.hpp
    PUBLIC promise.hpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
    PRIVATE OpenGL::GL
    PRIVATE Threads::Threads
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:winmm.lib>)

target_include_directories(${PROJECT_NAME}
    PUBLIC headers
    PUBLIC headers/3rdparty
    PUBLIC headers/3rdparty/imgui
    PUBLIC headers/3rdparty/pugixml
    PRIVATE sources
    PRIVATE sources/3rdparty
    PRIVATE sources/3rdparty/glad)

target_compile_options(${PROJECT_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
        /W3 /MP /bigobj>
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
