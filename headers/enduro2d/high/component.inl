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
    //
    // component_creator
    //

    template < typename Component >
    bool component_creator<Component>::fill_prototype(
        ecs::prototype& prototype,
        const component_loader<>::fill_context& ctx) const
    {
        Component component;
        if ( !loader_(ctx.parent_address, ctx.root, ctx.dependencies, component) ) {
            return false;
        }
        prototype.component<Component>(component);
        return true;
    }

    template < typename Component >
    bool component_creator<Component>::collect_dependencies(
        asset_dependencies& dependencies,
        const component_loader<>::collect_context& ctx) const
    {
        return loader_(ctx.parent_address, ctx.root, dependencies);
    }

    //
    // component_factory
    //

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
