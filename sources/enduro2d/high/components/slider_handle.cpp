/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/slider_handle.hpp>

namespace e2d
{
    const char* factory_loader<slider_handle>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
    })json";

    bool factory_loader<slider_handle>::operator()(
        slider_handle& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<slider_handle>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<slider_handle>::title = ICON_FA_HAND_ROCK " slider_handle";

    void component_inspector<slider_handle>::operator()(gcomponent<slider_handle>& c) const {
        E2D_UNUSED(c);
    }
}
