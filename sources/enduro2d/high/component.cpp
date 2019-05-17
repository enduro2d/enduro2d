/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/component.hpp>

namespace e2d
{
    bool component_factory::fill_prototype(
        str_hash type,
        ecs::prototype& prototype,
        const component_loader<>::fill_context& ctx) const
    {
        auto creator = find_creator(type);
        return creator
            ? creator->fill_prototype(prototype, ctx)
            : false;
    }

    bool component_factory::collect_dependencies(
        str_hash type,
        asset_dependencies& dependencies,
        const component_loader<>::collect_context& ctx) const
    {
        auto creator = find_creator(type);
        return creator
            ? creator->collect_dependencies(dependencies, ctx)
            : false;
    }

    component_creator_base_iptr component_factory::find_creator(str_hash type) const {
        std::lock_guard<std::mutex> guard(mutex_);
        const auto iter = creators_.find(type);
        return iter != creators_.end()
            ? iter->second
            : nullptr;
    }
}
