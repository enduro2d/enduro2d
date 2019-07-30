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

#define E2D_PLATFORM_LINUX   1
#define E2D_PLATFORM_IOS     2
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
