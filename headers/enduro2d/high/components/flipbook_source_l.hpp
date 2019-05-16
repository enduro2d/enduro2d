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
            str_view parent_address,
            const rapidjson::Value& root,
            asset_dependencies& dependencies) const
        {
            if ( root.HasMember("flipbook") ) {
                dependencies.add_dependency<flipbook_asset>(
                    path::combine(parent_address, root["flipbook"].GetString()));
            }

            return true;
        }

        bool operator()(
            str_view parent_address,
            const rapidjson::Value& root,
            const asset_group& dependencies,
            flipbook_source& component) const
        {
            if ( root.HasMember("flipbook") ) {
                auto flipbook = dependencies.find_asset<flipbook_asset>(
                    path::combine(parent_address, root["flipbook"].GetString()));
                if ( !flipbook ) {
                    the<debug>().error("FLIPBOOK_SOURCE: Dependency 'flipbook' is not found:\n"
                        "--> Parent address: %0\n"
                        "--> Dependency address: %1",
                        parent_address,
                        root["flipbook"].GetString());
                    return false;
                }
                component.flipbook(flipbook);
            }

            return true;
        }
    };
}
