/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_core_binds.hpp"

namespace e2d::bindings::core
{
    void bind_window(sol::state& l) {
        l.new_usertype<window>("window",
            sol::no_constructor,

            "hide", [](window& w) {
                w.hide();
            },

            "show", [](window& w) {
                w.show();
            },

            "restore", [](window& w) {
                w.restore();
            },

            "minimize", [](window& w) {
                w.minimize();
            },

            "enabled", sol::property([](const window& w) -> bool {
                return w.enabled();
            }),

            "visible", sol::property([](const window& w) -> bool {
                return w.visible();
            }),

            "focused", sol::property([](const window& w) -> bool {
                return w.focused();
            }),

            "minimized", sol::property([](const window& w) -> bool {
                return w.minimized();
            }),

            "fullscreen", sol::property(
                [](const window& w) -> bool {
                    return w.fullscreen();
                },
                [](window& w, bool yesno){
                    w.toggle_fullscreen(yesno);
                }),

            "cursor_hidden", sol::property(
                &window::is_cursor_hidden,
                [](window& w, bool yesno){
                    if ( yesno ) {
                        w.hide_cursor();
                    } else {
                        w.show_cursor();
                    }
                }),

            "real_size", sol::property([](const window& w) -> v2f {
                return w.real_size().cast_to<f32>();
            }),

            "virtual_size", sol::property([](const window& w) -> v2f {
                return w.virtual_size().cast_to<f32>();
            }),

            "framebuffer_size", sol::property([](const window& w) -> v2f {
                return w.framebuffer_size().cast_to<f32>();
            }),

            "title", sol::property(
                [](const window& w) -> str {
                    return w.title();
                },
                [](window& w, str_view t){
                    w.set_title(t);
                }),

            "should_close", sol::property(
                [](const window& w) -> bool {
                    return w.should_close();
                },
                [](window& w, bool yesno){
                    w.set_should_close(yesno);
                })
        );
    }
}
