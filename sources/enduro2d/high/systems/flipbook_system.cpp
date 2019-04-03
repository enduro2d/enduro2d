/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/systems/flipbook_system.hpp>

namespace
{
    using namespace e2d;
}

namespace e2d
{
    //
    // flipbook_system::internal_state
    //

    class flipbook_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process(ecs::registry& owner) {
            E2D_UNUSED(owner);
        }
    };

    //
    // flipbook_system
    //

    flipbook_system::flipbook_system()
    : state_(new internal_state()) {}
    flipbook_system::~flipbook_system() noexcept = default;

    void flipbook_system::process(ecs::registry& owner) {
        state_->process(owner);
    }
}
