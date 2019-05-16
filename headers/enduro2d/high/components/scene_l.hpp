/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "scene.hpp"

namespace e2d
{
    template <>
    class component_loader<scene> {
    public:
        constexpr static const char*
        schema_source = R"json({
            "type" : "object",
            "required" : [],
            "additionalProperties" : false,
            "properties" : {
                "depth" : { "type" : "number" }
            }
        })json";

        bool operator()(
            str_view parent_address,
            const rapidjson::Value& root,
            asset_dependencies& dependencies) const
        {
            E2D_UNUSED(
                parent_address,
                root,
                dependencies);

            return true;
        }

        bool operator()(
            str_view parent_address,
            const rapidjson::Value& root,
            const asset_group& dependencies,
            scene& component) const
        {
            E2D_UNUSED(
                parent_address,
                dependencies);

            if ( root.HasMember("depth") ) {
                auto depth = component.depth();
                if ( !json_utils::try_parse_value(root["depth"], depth) ) {
                    the<debug>().error("SCENE: Incorrect formatting of 'depth' property");
                    return false;
                }
                component.depth(depth);
            }

            return true;
        }
    };
}
