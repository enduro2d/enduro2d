/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/systems/sprite_system.hpp>

namespace e2d
{
    namespace systems
    {
        //
        // sprite_system::internal_state
        //

        class sprite_system::internal_state final : private noncopyable {
        public:
            internal_state() = default;
            ~internal_state() noexcept = default;
        };

        //
        // sprite_system
        //

        sprite_system::sprite_system()
        : state_(new internal_state) {}
        sprite_system::~sprite_system() noexcept = default;

        void sprite_system::process(ecs::registry& owner) {
            E2D_UNUSED(owner);
        }
    }
}
