/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "actor.hpp"

namespace e2d
{
    template <>
    class component_loader<actor> {
    public:
        constexpr static const char*
        schema_source = R"json({
            "type" : "object",
            "required" : [],
            "additionalProperties" : false,
            "properties" : {
                "translation" : { "$ref": "#/common_definitions/v3" },
                "rotation" : { "$ref": "#/common_definitions/q4" },
                "scale" : { "$ref": "#/common_definitions/v3" }
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
            actor& component) const
        {
            E2D_UNUSED(
                parent_address,
                dependencies);

            if ( !component.node() ) {
                component.node(node::create());
            }

            if ( root.HasMember("translation") ) {
                auto translation = component.node()->translation();
                if ( !json_utils::try_parse_value(root["translation"], translation) ) {
                    the<debug>().error("ACTOR: Incorrect formatting of 'translation' property");
                    return false;
                }
                component.node()->translation(translation);
            }

            if ( root.HasMember("rotation") ) {
                auto rotation = component.node()->rotation();
                if ( !json_utils::try_parse_value(root["rotation"], rotation) ) {
                    the<debug>().error("ACTOR: Incorrect formatting of 'rotation' property");
                    return false;
                }
                component.node()->rotation(rotation);
            }

            if ( root.HasMember("scale") ) {
                auto scale = component.node()->scale();
                if ( !json_utils::try_parse_value(root["scale"], scale) ) {
                    the<debug>().error("ACTOR: Incorrect formatting of 'scale' property");
                    return false;
                }
                component.node()->scale(scale);
            }

            return true;
        }
    };
}
