/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/high/_high.hpp>

namespace e2d::bindings
{
    template < typename T >
    struct component_wrapper {
        gcomponent<T> component;
    };
}

namespace sol
{
    template < typename T >
    struct unique_usertype_traits<e2d::bindings::component_wrapper<T>> {
        using type = e2d::gcomponent<T>;
        using actual_type = e2d::bindings::component_wrapper<T>;
        static const bool value = true;

        static bool is_null(const actual_type& ptr) {
            return !ptr.component.exists();
        }

        static type* get(actual_type& ptr) {
            return &ptr.component;
        }
    };
}
