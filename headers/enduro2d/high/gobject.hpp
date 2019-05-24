/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    class gobject;
    using gobject_iptr = intrusive_ptr<gobject>;
    using const_gobject_iptr = intrusive_ptr<const gobject>;
}

namespace e2d
{
    class gobject final
        : private noncopyable
        , public ref_counter<gobject> {
    public:
        gobject(ecs::registry& registry);
        gobject(ecs::registry& registry, const ecs::prototype& proto);
        ~gobject() noexcept;

        ecs::entity entity() noexcept;
        ecs::const_entity entity() const noexcept;
        ecs::entity_filler entity_filler() noexcept;

        template < typename T >
        ecs::component<T> get_component() noexcept;

        template < typename T >
        ecs::const_component<T> get_component() const noexcept;
    private:
        ecs::entity entity_;
    };
}

namespace e2d
{
    template < typename T >
    ecs::component<T> gobject::get_component() noexcept {
        return ecs::component<T>(entity_);
    }

    template < typename T >
    ecs::const_component<T> gobject::get_component() const noexcept {
        return ecs::const_component<T>(entity_);
    }
}
