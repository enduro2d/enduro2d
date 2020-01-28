/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_base.hpp"

//
// E2D_COMPILER
//

#define E2D_COMPILER_CLANG 1
#define E2D_COMPILER_GCC   2
#define E2D_COMPILER_MSVC  3

#ifndef E2D_COMPILER
#  if defined(__clang__)
#    define E2D_COMPILER E2D_COMPILER_CLANG
#  elif defined(__GNUC__)
#    define E2D_COMPILER E2D_COMPILER_GCC
#  elif defined(_MSC_VER)
#    define E2D_COMPILER E2D_COMPILER_MSVC
#  endif
#endif

#ifndef E2D_COMPILER
#  error E2D_COMPILER not detected
#endif

//
// E2D_PLATFORM
//

#define E2D_PLATFORM_IOS     1
#define E2D_PLATFORM_LINUX   2
#define E2D_PLATFORM_MACOSX  3
#define E2D_PLATFORM_WINDOWS 4
#define E2D_PLATFORM_ANDROID 5

#ifndef E2D_PLATFORM
#  if defined(__ANDROID__) || defined(ANDROID)
#    define E2D_PLATFORM E2D_PLATFORM_ANDROID
#  elif defined(__linux__) || defined(__linux) || defined(linux)
#    define E2D_PLATFORM E2D_PLATFORM_LINUX
#  elif defined(macintosh) || defined(Macintosh) || defined(__APPLE__)
#    include <TargetConditionals.h>
#    if (TARGET_OS_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)
#      define E2D_PLATFORM E2D_PLATFORM_IOS
#    elif (TARGET_OS_MAC == 1)
#      define E2D_PLATFORM E2D_PLATFORM_MACOSX
#    endif
#  elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#    define E2D_PLATFORM E2D_PLATFORM_WINDOWS
#  endif
#endif

#ifndef E2D_PLATFORM
#  error E2D_PLATFORM not detected
#endif

//
// E2D_BUILD_MODE
//

#define E2D_BUILD_MODE_DEBUG   1
#define E2D_BUILD_MODE_RELEASE 2

#ifndef E2D_BUILD_MODE
#  if defined(RELEASE) || defined(_RELEASE) || defined(NDEBUG)
#    define E2D_BUILD_MODE E2D_BUILD_MODE_RELEASE
#  else
#    define E2D_BUILD_MODE E2D_BUILD_MODE_DEBUG
#  endif
#endif

#ifndef E2D_BUILD_MODE
#  error E2D_BUILD_MODE not detected
#endif

//
// E2D_FILES_MODE
//

#define E2D_FILES_MODE_POSIX  1
#define E2D_FILES_MODE_WINAPI 2

#ifndef E2D_FILES_MODE
#  if defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_IOS
#    define E2D_FILES_MODE E2D_FILES_MODE_POSIX
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_LINUX
#    define E2D_FILES_MODE E2D_FILES_MODE_POSIX
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_MACOSX
#    define E2D_FILES_MODE E2D_FILES_MODE_POSIX
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_WINDOWS
#    define E2D_FILES_MODE E2D_FILES_MODE_WINAPI
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_ANDROID
#    define E2D_FILES_MODE E2D_FILES_MODE_POSIX
#  endif
#endif

#ifndef E2D_FILES_MODE
#  error E2D_FILES_MODE not detected
#endif

//
// E2D_AUDIO_MODE
//

#define E2D_AUDIO_MODE_NONE 1
#define E2D_AUDIO_MODE_BASS 2

#ifndef E2D_AUDIO_MODE
#  if defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_IOS
#    define E2D_AUDIO_MODE E2D_AUDIO_MODE_NONE
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_LINUX
#    define E2D_AUDIO_MODE E2D_AUDIO_MODE_BASS
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_MACOSX
#    define E2D_AUDIO_MODE E2D_AUDIO_MODE_BASS
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_WINDOWS
#    define E2D_AUDIO_MODE E2D_AUDIO_MODE_BASS
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_ANDROID
#    define E2D_AUDIO_MODE E2D_AUDIO_MODE_BASS
#  endif
#endif

#ifndef E2D_AUDIO_MODE
#  error E2D_AUDIO_MODE not detected
#endif

//
// E2D_RENDER_MODE
//

#define E2D_RENDER_MODE_NONE 1
#define E2D_RENDER_MODE_OPENGL 2
#define E2D_RENDER_MODE_OPENGLES 3

#ifndef E2D_RENDER_MODE
#  if defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_IOS
#    define E2D_RENDER_MODE E2D_RENDER_MODE_OPENGLES
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_LINUX
#    define E2D_RENDER_MODE E2D_RENDER_MODE_OPENGL
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_MACOSX
#    define E2D_RENDER_MODE E2D_RENDER_MODE_OPENGL
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_WINDOWS
#    define E2D_RENDER_MODE E2D_RENDER_MODE_OPENGL
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_ANDROID
#    define E2D_RENDER_MODE E2D_RENDER_MODE_OPENGLES
#  endif
#endif

#ifndef E2D_RENDER_MODE
#  error E2D_RENDER_MODE not detected
#endif

//
// E2D_WINDOW_MODE
//

#define E2D_WINDOW_MODE_NONE 1
#define E2D_WINDOW_MODE_GLFW 2

#ifndef E2D_WINDOW_MODE
#  if defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_IOS
#    define E2D_WINDOW_MODE E2D_WINDOW_MODE_NONE
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_LINUX
#    define E2D_WINDOW_MODE E2D_WINDOW_MODE_GLFW
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_MACOSX
#    define E2D_WINDOW_MODE E2D_WINDOW_MODE_GLFW
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_WINDOWS
#    define E2D_WINDOW_MODE E2D_WINDOW_MODE_GLFW
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_ANDROID
#    define E2D_WINDOW_MODE E2D_WINDOW_MODE_NONE
#  endif
#endif

#ifndef E2D_WINDOW_MODE
#  error E2D_WINDOW_MODE not detected
#endif
