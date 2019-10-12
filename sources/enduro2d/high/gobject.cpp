/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#include <enduro2d/high/gobject.hpp>

namespace e2d
{
    const gobject::state_iptr& gobject::internal_state() const noexcept {
        return state_;
    }

    gobject::gobject(state_iptr state)
    : state_(std::move(state)) {}

    bool gobject::valid() const noexcept {
        return state_ && !state_->invalided();
    }

    gobject::operator bool() const noexcept {
        return valid();
    }

    void gobject::destroy() noexcept {
        if ( valid() ) {
            state_->destroy();
        }
    }

    ecs::entity gobject::raw_entity() noexcept {
        E2D_ASSERT(valid());
        return state_->raw_entity();
    }

    ecs::const_entity gobject::raw_entity() const noexcept {
        E2D_ASSERT(valid());
        return state_->raw_entity();
    }
}
