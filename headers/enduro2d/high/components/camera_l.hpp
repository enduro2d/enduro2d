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
            asset_dependencies& dependencies,
            const component_loader<>::collect_context& ctx) const
        {
            E2D_UNUSED(dependencies, ctx);
            return true;
        }

        bool operator()(
            camera& component,
            const component_loader<>::fill_context& ctx) const
        {
            if ( ctx.root.HasMember("depth") ) {
                auto depth = component.depth();
                if ( !json_utils::try_parse_value(ctx.root["depth"], depth) ) {
                    the<debug>().error("CAMERA: Incorrect formatting of 'depth' property");
                    return false;
                }
                component.depth(depth);
            }

            if ( ctx.root.HasMember("viewport") ) {
                auto viewport = component.viewport();
                if ( !json_utils::try_parse_value(ctx.root["viewport"], viewport) ) {
                    the<debug>().error("CAMERA: Incorrect formatting of 'viewport' property");
                    return false;
                }
                component.viewport(viewport);
            }

            if ( ctx.root.HasMember("projection") ) {
                auto projection = component.projection();
                if ( !json_utils::try_parse_value(ctx.root["projection"], projection) ) {
                    the<debug>().error("CAMERA: Incorrect formatting of 'projection' property");
                    return false;
                }
                component.projection(projection);
            }

            if ( ctx.root.HasMember("target") ) {
                //TODO(BlackMat): add 'target' property parsing
            }

            if ( ctx.root.HasMember("background") ) {
                auto background = component.background();
                if ( !json_utils::try_parse_value(ctx.root["background"], background) ) {
                    the<debug>().error("CAMERA: Incorrect formatting of 'background' property");
                    return false;
                }
                component.background(background);
            }

            return true;
        }
    };
}
