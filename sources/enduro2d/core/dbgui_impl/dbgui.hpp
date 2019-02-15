/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include <enduro2d/core/dbgui.hpp>

#include <enduro2d/core/debug.hpp>
#include <enduro2d/core/engine.hpp>
#include <enduro2d/core/input.hpp>
#include <enduro2d/core/render.hpp>
#include <enduro2d/core/window.hpp>

#include <3rdparty/imgui/imgui.h>

namespace e2d { namespace dbgui_shaders
{
    const char* vertex_source_cstr() noexcept;
    const char* fragment_source_cstr() noexcept;
}}

namespace e2d { namespace dbgui_widgets
{
    void show_main_menu();
    void show_engine_stats();
    void show_window_stats();
}}
