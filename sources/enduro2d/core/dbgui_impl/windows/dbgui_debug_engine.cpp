/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../dbgui.hpp"

namespace e2d::imgui::windows
{
    void show_debug_engine(bool* open) {
        if ( !modules::is_initialized<engine>() ) {
            if ( open ) {
                *open = false;
            }
            return;
        }

        engine& e = the<engine>();
        const char* window_title = "Engine Debug";

        if ( E2D_DEFER([](){ ImGui::End(); });
            ImGui::Begin(window_title, open, ImGuiWindowFlags_None) )
        {
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
        }
    }
}
