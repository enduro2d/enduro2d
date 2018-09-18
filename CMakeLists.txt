cmake_minimum_required(VERSION 3.9.2 FATAL_ERROR)
project(enduro2d)

#
# global defines
#

enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(E2D_DEBUG_FLAGS       " -D_DEBUG -DNRELEASE")
set(E2D_RELEASE_FLAGS     " -DNDEBUG -D_RELEASE")
set(CMAKE_C_FLAGS_DEBUG     ${CMAKE_C_FLAGS_DEBUG}${E2D_DEBUG_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG   ${CMAKE_CXX_FLAGS_DEBUG}${E2D_DEBUG_FLAGS})
set(CMAKE_C_FLAGS_RELEASE   ${CMAKE_C_FLAGS_RELEASE}${E2D_RELEASE_FLAGS})
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE}${E2D_RELEASE_FLAGS})

#
# warnings
#

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_compile_options(-Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
endif()

#
# e2d sources
#

file(GLOB_RECURSE e2d_headers
  headers/enduro2d/*.hpp
  headers/enduro2d/*.inl)

file(GLOB_RECURSE e2d_sources
  sources/enduro2d/*.cpp
  sources/enduro2d/*.hpp
  headers/enduro2d/*.inl)

if(APPLE)
  file(GLOB_RECURSE e2d_sources_mm
    sources/enduro2d/*.mm)
  list(APPEND e2d_sources ${e2d_sources_mm})
endif()

#
# 3rd party
#

file(GLOB_RECURSE e2d_3rdparty
  sources/3rdparty/*.*)

#
# e2d library
#

add_library(${PROJECT_NAME} STATIC
  ${e2d_headers}
  ${e2d_sources}
  ${e2d_3rdparty})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
  ${e2d_headers}
  ${e2d_sources}
  ${e2d_3rdparty})

target_include_directories(${PROJECT_NAME} PRIVATE
  headers
  sources)

#
# subdirectories
#

option(E2D_BUILD_SAMPLES "Build samples" ON)
if(E2D_BUILD_SAMPLES)
  add_subdirectory(samples)
endif()

option(E2D_BUILD_UNTESTS "Build untests" ON)
if(E2D_BUILD_UNTESTS)
  add_subdirectory(untests)
endif()
