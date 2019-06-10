/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/core/debug.hpp>
#include <enduro2d/core/audio.hpp>

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
#  endif
#endif

#ifndef E2D_AUDIO_MODE
#  error E2D_AUDIO_MODE not detected
#endif
