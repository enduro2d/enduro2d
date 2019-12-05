/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "debug_window_widget.hpp"

namespace e2d::dbgui_widgets
{
    bool debug_window_widget::show() {
        if ( !modules::is_initialized<window>() ) {
            return false;
        }

        window& w = the<window>();

        {
            if ( bool visible = w.visible();
                ImGui::Checkbox("visible", &visible) )
            {
                if ( visible ) {
                    w.show();
                } else {
                    w.hide();
                }
            }
        }

        {
            bool focused = w.focused();
            ImGui::Checkbox("focused", &focused);
        }

        {
            if ( bool minimized = w.minimized();
                ImGui::Checkbox("minimized", &minimized) )
            {
                if ( minimized ) {
                    w.minimize();
                } else {
                    w.restore();
                }
            }
        }

        ImGui::Separator();

        {
            ImGui::Text("%s", strings::rformat("real size: %0", w.real_size()).c_str());
            ImGui::Text("%s", strings::rformat("virtual size: %0", w.virtual_size()).c_str());
            ImGui::Text("%s", strings::rformat("framebuffer size: %0", w.framebuffer_size()).c_str());
        }

        ImGui::Separator();

        {
            if ( bool fullscreen = w.fullscreen();
                ImGui::Checkbox("fullscreen", &fullscreen) )
            {
                w.toggle_fullscreen(fullscreen);
            }
        }

        {
            if ( bool cursor = !w.is_cursor_hidden();
                ImGui::Checkbox("system cursor", &cursor) )
            {
                if ( cursor ) {
                    w.show_cursor();
                } else {
                    w.hide_cursor();
                }
            }
        }

        {
            char title_buf[128] = {0};
            strings::format(title_buf, std::size(title_buf), w.title());
            if ( ImGui::InputText("title", title_buf, std::size(title_buf)) ) {
                w.set_title(title_buf);
            }
        }

        return true;
    }
}
