/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/systems/render_system.hpp>

namespace e2d
{
    //
    // render_system::internal_state
    //

    class render_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;
    };

    //
    // render_system
    //

    render_system::render_system()
    : state_(new internal_state()) {}
    render_system::~render_system() noexcept = default;

    void render_system::process(ecs::registry& owner) {
        E2D_UNUSED(owner);
    }
}
