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
    modules::initialize<window>(v2u{640, 480}, "Enduro2D", false);

    the<debug>().register_sink<debug_console_sink>();
    the<window>().register_event_listener<window_input_source>(the<input>());
    the<window>().register_event_listener<window_trace_event_listener>(the<debug>());

    the<debug>()
        .trace("SAMPLE: window real size: %0", the<window>().real_size())
        .trace("SAMPLE: window virtual size: %0", the<window>().virtual_size())
        .trace("SAMPLE: window framebuffer size: %0", the<window>().framebuffer_size());

    const keyboard& k = the<input>().keyboard();
    while ( !the<window>().should_close() && !k.is_key_just_released(keyboard_key::escape) ) {
        the<window>().swap_buffers(true);
        the<input>().frame_tick();
        window::poll_events();
    }
    return 0;
}
