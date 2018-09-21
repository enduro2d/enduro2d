/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

int main() {
    modules::initialize<debug>()
        .add_sink<debug_console_sink>();

    window& w = modules::initialize<window>(
        v2u{640, 480}, "Enduro2D", false, false);

    the<debug>()
        .trace("SAMPLE: window real size: %0", w.real_size())
        .trace("SAMPLE: window virtual size: %0", w.virtual_size())
        .trace("SAMPLE: window framebuffer size: %0", w.framebuffer_size());

    auto closing_time = time::now_s() + make_seconds<i64>(5);
    while ( !w.should_close() && time::now_s() < closing_time ) {
        w.swap_buffers();
        window::poll_events();
    }
}
