/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/scene.hpp>

namespace e2d
{
    const char* component_loader<scene>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "depth" : { "type" : "number" }
        }
    })json";

    bool component_loader<scene>::operator()(
        scene& component,
        const component_loader<>::fill_context& ctx) const
    {
        if ( ctx.root.HasMember("depth") ) {
            auto depth = component.depth();
            if ( !json_utils::try_parse_value(ctx.root["depth"], depth) ) {
                the<debug>().error("SCENE: Incorrect formatting of 'depth' property");
                return false;
            }
            component.depth(depth);
        }

        return true;
    }

    bool component_loader<scene>::operator()(
        asset_dependencies& dependencies,
        const component_loader<>::collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}
