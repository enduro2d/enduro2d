/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/flipbook_source.hpp>

namespace e2d
{
    const char* factory_loader<flipbook_source>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "flipbook" : { "$ref": "#/common_definitions/address" }
        }
    })json";

    bool factory_loader<flipbook_source>::operator()(
        flipbook_source& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("flipbook") ) {
            auto flipbook = ctx.dependencies.find_asset<flipbook_asset>(
                path::combine(ctx.parent_address, ctx.root["flipbook"].GetString()));
            if ( !flipbook ) {
                the<debug>().error("FLIPBOOK_SOURCE: Dependency 'flipbook' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["flipbook"].GetString());
                return false;
            }
            component.flipbook(flipbook);
        }

        return true;
    }

    bool factory_loader<flipbook_source>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("flipbook") ) {
            dependencies.add_dependency<flipbook_asset>(
                path::combine(ctx.parent_address, ctx.root["flipbook"].GetString()));
        }

        return true;
    }
}
