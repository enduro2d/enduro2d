/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "sprite_renderer.hpp"

namespace e2d
{
    template <>
    class component_loader<sprite_renderer> {
    public:
        constexpr static const char*
        schema_source = R"json({
            "type" : "object",
            "required" : [],
            "additionalProperties" : false,
            "properties" : {
                "tint" : { "$ref": "#/common_definitions/color" },
                "filtering" : { "type" : "boolean" },
                "sprite" : { "$ref": "#/common_definitions/address" }
            }
        })json";

        bool operator()(
            asset_dependencies& dependencies,
            const component_loader<>::collect_context& ctx) const
        {
            if ( ctx.root.HasMember("sprite") ) {
                dependencies.add_dependency<sprite_asset>(
                    path::combine(ctx.parent_address, ctx.root["sprite"].GetString()));
            }

            return true;
        }

        bool operator()(
            sprite_renderer& component,
            const component_loader<>::fill_context& ctx) const
        {
            if ( ctx.root.HasMember("tint") ) {
                auto tint = component.tint();
                if ( !json_utils::try_parse_value(ctx.root["tint"], tint) ) {
                    the<debug>().error("SPRITE_RENDERER: Incorrect formatting of 'tint' property");
                    return false;
                }
                component.tint(tint);
            }

            if ( ctx.root.HasMember("filtering") ) {
                auto filtering = component.filtering();
                if ( !json_utils::try_parse_value(ctx.root["filtering"], filtering) ) {
                    the<debug>().error("SPRITE_RENDERER: Incorrect formatting of 'filtering' property");
                    return false;
                }
                component.filtering(filtering);
            }

            if ( ctx.root.HasMember("sprite") ) {
                auto sprite = ctx.dependencies.find_asset<sprite_asset>(
                    path::combine(ctx.parent_address, ctx.root["sprite"].GetString()));
                if ( !sprite ) {
                    the<debug>().error("SPRITE_RENDERER: Dependency 'sprite' is not found:\n"
                        "--> Parent address: %0\n"
                        "--> Dependency address: %1",
                        ctx.parent_address,
                        ctx.root["sprite"].GetString());
                    return false;
                }
                component.sprite(sprite);
            }

            return true;
        }
    };
}
