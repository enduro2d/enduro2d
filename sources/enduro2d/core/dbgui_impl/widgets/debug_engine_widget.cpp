/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "debug_engine_widget.hpp"

namespace e2d::dbgui_widgets
{
    bool debug_engine_widget::show() {
        if ( !modules::is_initialized<engine>() ) {
            return false;
        }

        engine& e = the<engine>();

        {
            ImGui::Text("%s", strings::rformat("time: %0", e.time()).c_str());
            ImGui::Text("%s", strings::rformat("delta time: %0", e.delta_time()).c_str());
        }

        ImGui::Separator();

        {
            ImGui::Text("%s", strings::rformat("frame rate: %0", e.frame_rate()).c_str());
            ImGui::Text("%s", strings::rformat("frame count: %0", e.frame_count()).c_str());
            ImGui::Text("%s", strings::rformat("realtime time: %0", e.realtime_time()).c_str());
        }

        return true;
    }
}
