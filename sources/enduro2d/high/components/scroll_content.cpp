/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/scroll_content.hpp>

namespace e2d
{
    const char* factory_loader<scroll_content>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
    })json";

    bool factory_loader<scroll_content>::operator()(
        scroll_content& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<scroll_content>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<scroll_content>::title = ICON_FA_HAND_ROCK " scroll_content";

    void component_inspector<scroll_content>::operator()(gcomponent<scroll_content>& c) const {
        E2D_UNUSED(c);
    }
}
