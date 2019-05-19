/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "flipbook_source.hpp"

namespace e2d
{
    template <>
    class component_loader<flipbook_source> {
    public:
        constexpr static const char*
        schema_source = R"json({
            "type" : "object",
            "required" : [],
            "additionalProperties" : false,
            "properties" : {
                "flipbook" : { "$ref": "#/common_definitions/address" }
            }
        })json";

        bool operator()(
            asset_dependencies& dependencies,
            const component_loader<>::collect_context& ctx) const
        {
            if ( ctx.root.HasMember("flipbook") ) {
                dependencies.add_dependency<flipbook_asset>(
                    path::combine(ctx.parent_address, ctx.root["flipbook"].GetString()));
            }

            return true;
        }

        bool operator()(
            flipbook_source& component,
            const component_loader<>::fill_context& ctx) const
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
    };
}
