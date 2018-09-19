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

    for ( std::size_t i = 0; i < 2; ++i ) {
        w.swap_buffers();
        window::poll_events();
        std::this_thread::sleep_for(time::to_chrono(make_seconds(2)));
        w.toggle_fullscreen(!w.fullscreen());
    }

    auto current_time = time::now_s();
    while ( !w.should_close() && current_time + make_seconds(5ll) < time::now_s() ) {
        w.swap_buffers();
        window::poll_events();
    }
}
