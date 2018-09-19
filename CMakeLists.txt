cmake_minimum_required(VERSION 3.9.2 FATAL_ERROR)
project(enduro2d)

#
# build mode
#

foreach(flags CMAKE_C_FLAGS_DEBUG
              CMAKE_CXX_FLAGS_DEBUG)
  set(${flags} "${${flags}} -D_DEBUG -DNRELEASE")
endforeach(flags)

foreach(flags CMAKE_C_FLAGS_RELEASE
              CMAKE_C_FLAGS_MINSIZEREL
              CMAKE_C_FLAGS_RELWITHDEBINFO
              CMAKE_CXX_FLAGS_RELEASE
              CMAKE_CXX_FLAGS_MINSIZEREL
              CMAKE_CXX_FLAGS_RELWITHDEBINFO)
  set(${flags} "${${flags}} -DNDEBUG -D_RELEASE")
endforeach(flags)

#
# warning mode
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
  list(APPEND E2D_SOURCES ${E2D_SOURCES_MM})
endif()

#
# e2d 3rd party
#

file(GLOB_RECURSE E2D_3RDPARTY
  sources/3rdparty/*.*)

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

target_include_directories(${PROJECT_NAME} PRIVATE
  headers
  sources)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO)

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
