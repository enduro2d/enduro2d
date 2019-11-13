/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_core_binds.hpp"

namespace e2d::bindings::core
{
    void bind_window(sol::state& l) {
        l.new_usertype<window>("window",
            sol::no_constructor,

            "hide", &window::hide,
            "show", &window::show,
            "restore", &window::restore,
            "minimize", &window::minimize,

            "enabled", sol::property(&window::enabled),
            "visible", sol::property(&window::visible),
            "focused", sol::property(&window::focused),
            "minimized", sol::property(&window::minimized),

            "fullscreen", sol::property(
                &window::fullscreen,
                &window::toggle_fullscreen),

            "cursor_hidden", sol::property(
                &window::is_cursor_hidden,
                [](window& w, bool yesno){
                    if ( yesno ) {
                        w.hide_cursor();
                    } else {
                        w.show_cursor();
                    }
                }),

            "real_size", sol::property([](const window& w){
                return w.real_size().cast_to<f32>();
            }),

            "virtual_size", sol::property([](const window& w){
                return w.virtual_size().cast_to<f32>();
            }),

            "framebuffer_size", sol::property([](const window& w){
                return w.framebuffer_size().cast_to<f32>();
            }),

            "title", sol::property(
                &window::title,
                &window::set_title),

            "should_close", sol::property(
                &window::should_close,
                &window::set_should_close)
        );
    }
}
