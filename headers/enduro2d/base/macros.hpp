/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "configs.hpp"

//
// E2D_ASSERT
//

#if defined(E2D_BUILD_MODE) && E2D_BUILD_MODE == E2D_BUILD_MODE_DEBUG
#  if defined(E2D_COMPILER) && E2D_COMPILER == E2D_COMPILER_MSVC
#    include <crtdbg.h>
#    define E2D_ASSERT(expr)          _ASSERT_EXPR((expr), nullptr)
#    define E2D_ASSERT_MSG(expr, msg) _ASSERT_EXPR((expr), _CRT_WIDE(msg))
#  else
#    define E2D_ASSERT(expr)          assert((expr))
#    define E2D_ASSERT_MSG(expr, msg) assert((expr) && (msg))
#  endif
#else
#  define E2D_ASSERT(expr)            ((void)0)
#  define E2D_ASSERT_MSG(expr, msg)   ((void)0)
#endif

//
// E2D_UNUSED
//

#define E2D_UNUSED(expr) (void)(expr)
