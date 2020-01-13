/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "window_widget.hpp"

namespace e2d::dbgui_widgets
{
    window_widget::window_widget() {
        desc_.title = "Window";
    }

    bool window_widget::show() {
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
            imgui_utils::with_disabled_flag([&w](){
                bool focused = w.focused();
                ImGui::Checkbox("focused", &focused);
            });
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
            imgui_utils::show_formatted_text(
                "real size: %0",
                w.real_size());

            imgui_utils::show_formatted_text(
                "virtual size: %0",
                w.virtual_size());

            imgui_utils::show_formatted_text(
                "framebuffer size: %0",
                w.framebuffer_size());
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
            if ( str title = w.title();
                ImGui::InputText("title", &title) )
            {
                w.set_title(title);
            }
        }

        return true;
    }

    const window_widget::description& window_widget::desc() const noexcept {
        return desc_;
    }
}
