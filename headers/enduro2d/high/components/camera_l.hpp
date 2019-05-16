/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "camera.hpp"

namespace e2d
{
    template <>
    class component_loader<camera> {
    public:
        constexpr static const char*
        schema_source = R"json({
            "type" : "object",
            "required" : [],
            "additionalProperties" : false,
            "properties" : {
                "depth" : { "type" : "number" },
                "viewport" : { "$ref": "#/common_definitions/b2" },
                "projection" : { "$ref": "#/common_definitions/m4" },
                "background" : { "$ref": "#/common_definitions/color" }
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
            camera& component) const
        {
            E2D_UNUSED(
                parent_address,
                dependencies);

            if ( root.HasMember("depth") ) {
                auto depth = component.depth();
                if ( !json_utils::try_parse_value(root["depth"], depth) ) {
                    the<debug>().error("CAMERA: Incorrect formatting of 'depth' property");
                    return false;
                }
                component.depth(depth);
            }

            if ( root.HasMember("viewport") ) {
                auto viewport = component.viewport();
                if ( !json_utils::try_parse_value(root["viewport"], viewport) ) {
                    the<debug>().error("CAMERA: Incorrect formatting of 'viewport' property");
                    return false;
                }
                component.viewport(viewport);
            }

            if ( root.HasMember("projection") ) {
                auto projection = component.projection();
                if ( !json_utils::try_parse_value(root["projection"], projection) ) {
                    the<debug>().error("CAMERA: Incorrect formatting of 'projection' property");
                    return false;
                }
                component.projection(projection);
            }

            if ( root.HasMember("target") ) {
                //TODO(BlackMat): add 'target' property parsing
            }

            if ( root.HasMember("background") ) {
                auto background = component.background();
                if ( !json_utils::try_parse_value(root["background"], background) ) {
                    the<debug>().error("CAMERA: Incorrect formatting of 'background' property");
                    return false;
                }
                component.background(background);
            }

            return true;
        }
    };
}
