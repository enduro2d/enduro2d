/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "engine_widget.hpp"

namespace e2d::dbgui_widgets
{
    engine_widget::engine_widget() {
        desc_.title = "Engine";
    }

    bool engine_widget::show() {
        if ( !modules::is_initialized<engine>() ) {
            return false;
        }

        engine& e = the<engine>();

        {
            imgex::show_text("time: %0", e.time());
            imgex::show_text("delta time: %0", e.delta_time());
        }

        ImGui::Separator();

        {
            imgex::show_text("frame rate: %0", e.frame_rate());
            imgex::show_text("frame count: %0", e.frame_count());
            imgex::show_text("realtime time: %0", e.realtime_time());
        }

        return true;
    }

    const engine_widget::description& engine_widget::desc() const noexcept {
        return desc_;
    }
}
