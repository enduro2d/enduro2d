/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/systems/model_system.hpp>

namespace e2d
{
    //
    // model_system::internal_state
    //

    class model_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;
    };

    //
    // model_system
    //

    model_system::model_system()
    : state_(new internal_state) {}
    model_system::~model_system() noexcept = default;

    void model_system::process(ecs::registry& owner) {
        E2D_UNUSED(owner);
    }
}
