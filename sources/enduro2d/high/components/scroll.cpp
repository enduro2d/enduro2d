/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/scroll.hpp>

namespace e2d
{
    const char* factory_loader<scroll>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {},
        "definitions" : {}
    })json";

    bool factory_loader<scroll>::operator()(
        scroll& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<scroll>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<scroll>::title = ICON_FA_SCROLL " scroll";

    void component_inspector<scroll>::operator()(gcomponent<scroll>& c) const {
        E2D_UNUSED(c);
    }
}
