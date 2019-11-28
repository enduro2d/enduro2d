/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/colliders.hpp>

namespace e2d
{
    const char* factory_loader<box_collider>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
        }
    })json";

    bool factory_loader<box_collider>::operator()(
        box_collider& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<box_collider>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}


namespace e2d
{
    const char* factory_loader<circle_collider>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
        }
    })json";

    bool factory_loader<circle_collider>::operator()(
        circle_collider& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<circle_collider>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<polygon_collider>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
        }
    })json";

    bool factory_loader<polygon_collider>::operator()(
        polygon_collider& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<polygon_collider>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}
