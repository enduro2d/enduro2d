/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

namespace
{
    class game_system final : public ecs::system {
        void process(ecs::registry& owner) override {
            E2D_UNUSED(owner);
            const keyboard& k = the<input>().keyboard();
            if ( k.is_key_just_released(keyboard_key::f12) ) {
                the<dbgui>().toggle_visible(!the<dbgui>().visible());
            }
            if ( k.is_key_just_released(keyboard_key::escape) ) {
                the<window>().set_should_close(true);
            }
        }
    };

    class game final : public high_application {
    public:
        bool initialize() final {
            the<world>().registry().add_system<game_system>();
            return true;
        }
    };
}

int e2d_main(int argc, char *argv[]) {
    const auto starter_params = starter::parameters(
        engine::parameters("sample_03", "enduro2d")
            .timer_params(engine::timer_parameters()
                .maximal_framerate(100)));
    modules::initialize<starter>(
        argc, argv, starter_params).start<game>();
    return 0;
}
