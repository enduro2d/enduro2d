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
            str_view parent_address,
            const rapidjson::Value& root,
            asset_dependencies& dependencies) const
        {
            if ( root.HasMember("sprite") ) {
                dependencies.add_dependency<sprite_asset>(
                    path::combine(parent_address, root["sprite"].GetString()));
            }

            return true;
        }

        bool operator()(
            str_view parent_address,
            const rapidjson::Value& root,
            const asset_group& dependencies,
            sprite_renderer& component) const
        {
            if ( root.HasMember("tint") ) {
                auto tint = component.tint();
                if ( !json_utils::try_parse_value(root["tint"], tint) ) {
                    the<debug>().error("SPRITE_RENDERER: Incorrect formatting of 'tint' property");
                    return false;
                }
                component.tint(tint);
            }

            if ( root.HasMember("filtering") ) {
                auto filtering = component.filtering();
                if ( !json_utils::try_parse_value(root["filtering"], filtering) ) {
                    the<debug>().error("SPRITE_RENDERER: Incorrect formatting of 'filtering' property");
                    return false;
                }
                component.filtering(filtering);
            }

            if ( root.HasMember("sprite") ) {
                auto sprite = dependencies.find_asset<sprite_asset>(
                    path::combine(parent_address, root["sprite"].GetString()));
                if ( !sprite ) {
                    the<debug>().error("SPRITE_RENDERER: Dependency 'sprite' is not found:\n"
                        "--> Parent address: %0\n"
                        "--> Dependency address: %1",
                        parent_address,
                        root["sprite"].GetString());
                    return false;
                }
                component.sprite(sprite);
            }

            return true;
        }
    };
}
