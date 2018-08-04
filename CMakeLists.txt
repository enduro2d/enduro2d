cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
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
# e2d sources
#

file(GLOB_RECURSE e2d_headers
  headers/enduro2d/*.*)

file(GLOB_RECURSE e2d_sources
  sources/enduro2d/*.*)

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

add_subdirectory(samples)
add_subdirectory(untests)
