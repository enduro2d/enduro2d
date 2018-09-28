/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

int e2d_main() {
    modules::initialize<debug>();
    modules::initialize<input>();
    modules::initialize<render>();
    modules::initialize<window>(v2u{640, 480}, "Enduro2D", false);

    the<debug>().add_sink<debug_console_sink>();
    the<window>().register_event_listener<window_input_source>(the<input>());

    const keyboard& k = the<input>().keyboard();
    while ( !the<window>().should_close() && !k.is_key_just_released(keyboard_key::escape) ) {
        the<render>().set_clear_color({1.f, 0.4f, 0.f});
        the<render>().clear(true, true, true);
        the<window>().swap_buffers(true);
        the<input>().frame_tick();
        window::poll_events();
    }
    return 0;
}
