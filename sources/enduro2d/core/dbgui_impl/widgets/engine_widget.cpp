/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "engine_widget.hpp"

namespace e2d::dbgui_widgets
{
    bool engine_widget::show() {
        if ( !modules::is_initialized<engine>() ) {
            return false;
        }

        engine& e = the<engine>();

        {
            imgui_utils::show_formatted_text("time: %0", e.time());
            imgui_utils::show_formatted_text("delta time: %0", e.delta_time());
        }

        ImGui::Separator();

        {
            imgui_utils::show_formatted_text("frame rate: %0", e.frame_rate());
            imgui_utils::show_formatted_text("frame count: %0", e.frame_count());
            imgui_utils::show_formatted_text("realtime time: %0", e.realtime_time());
        }

        return true;
    }

    const engine_widget::description& engine_widget::desc() const noexcept {
        return desc_;
    }
}
