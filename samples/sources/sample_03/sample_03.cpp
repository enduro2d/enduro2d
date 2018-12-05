/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

namespace
{
    class game final : public application {
    public:
        bool initialize() final {
            return true;
        }

        bool frame_tick() final {
            const keyboard& k = the<input>().keyboard();
            if ( the<window>().should_close() || k.is_key_just_released(keyboard_key::escape) ) {
                return false;
            }

            the<render>().execute(render::command_block<64>()
                .add_command(render::clear_command()
                    .color_value({1.f, 0.4f, 0.f, 1.f}))
                .add_command(render::swap_command(true)));

            return true;
        }
    };
}

int e2d_main(int argc, char *argv[]) {
    const auto starter_params = starter::parameters(
        engine::parameters("sample_03", "enduro3d")
            .timer_params(engine::timer_parameters()
                .maximal_framerate(100)));
    modules::initialize<starter>(
        argc, argv, starter_params).start<game>();
    return 0;
}
