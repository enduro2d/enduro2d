/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/core/dbgui.hpp>

#include <enduro2d/core/debug.hpp>
#include <enduro2d/core/engine.hpp>
#include <enduro2d/core/input.hpp>
#include <enduro2d/core/profiler.hpp>
#include <enduro2d/core/render.hpp>
#include <enduro2d/core/window.hpp>

#include <3rdparty/imgui/imgui.h>
#include <3rdparty/imgui/imgui_internal.h>

namespace e2d::imgui
{
    void show_main_dock_space();
}

namespace e2d::imgui::windows
{
    void show_debug_engine(bool* open);
    void show_debug_window(bool* open);
}
