/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_85A0D62C0D794C4C9DDFB1BAED3C0A18
#define E2D_INCLUDE_GUARD_85A0D62C0D794C4C9DDFB1BAED3C0A18
#pragma once

#include "_high.hpp"
#include "component.hpp"

namespace e2d
{
    template < typename Component >
    component_factory& component_factory::register_component(str_hash type) {
        std::lock_guard<std::mutex> guard(mutex_);
        if ( creators_.count(type) > 0 ) {
            throw bad_component_factory_operation();
        }
        component_creator_base_iptr creator(new component_creator<Component>());
        creators_.emplace(type, std::move(creator));
        return *this;
    }
}

#endif
