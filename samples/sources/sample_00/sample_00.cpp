/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

int main() {
    modules::initialize<debug>();
    the<debug>().add_sink<debug_console_sink>();

    window w{{640, 480}, "Enduro2D", false, false};
    while ( !w.should_close() ) {
        w.swap_buffers();
        window::poll_events();
    }
}
