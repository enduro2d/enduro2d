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
                imgex::show_checkbox("visible", &visible) )
            {
                if ( visible ) {
                    w.show();
                } else {
                    w.hide();
                }
            }
        }

        {
            imgex::show_disabled_checkbox("focused", w.focused());
        }

        {
            if ( bool minimized = w.minimized();
                imgex::show_checkbox("minimized", &minimized) )
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
            imgex::show_text(
                "real size: %0",
                w.real_size());

            imgex::show_text(
                "virtual size: %0",
                w.virtual_size());

            imgex::show_text(
                "framebuffer size: %0",
                w.framebuffer_size());
        }

        ImGui::Separator();

        {
            if ( bool fullscreen = w.fullscreen();
                imgex::show_checkbox("fullscreen", &fullscreen) )
            {
                w.toggle_fullscreen(fullscreen);
            }
        }

        {
            if ( bool cursor = !w.is_cursor_hidden();
                imgex::show_checkbox("system cursor", &cursor) )
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

    const window_widget::description& window_widget::desc() const noexcept {
        return desc_;
    }
}
