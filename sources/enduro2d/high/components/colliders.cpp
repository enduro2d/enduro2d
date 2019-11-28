/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/colliders.hpp>

namespace e2d
{
    const char* factory_loader<rect_collider>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "size" : { "$ref": "#/common_definitions/v2" },
            "pivot" : { "$ref": "#/common_definitions/v2" }
        }
    })json";

    bool factory_loader<rect_collider>::operator()(
        rect_collider& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("size") ) {
            v2f size = component.size();
            if ( !json_utils::try_parse_value(ctx.root["size"], size) ) {
                the<debug>().error("RECT_COLLIDER: Incorrect formatting of 'size' property");
                return false;
            }
            component.size(size);
        }

        if ( ctx.root.HasMember("pivot") ) {
            v2f pivot = component.pivot();
            if ( !json_utils::try_parse_value(ctx.root["pivot"], pivot) ) {
                the<debug>().error("RECT_COLLIDER: Incorrect formatting of 'pivot' property");
                return false;
            }
            component.pivot(pivot);
        }

        return true;
    }

    bool factory_loader<rect_collider>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}


namespace e2d
{
    const char* factory_loader<circle_collider>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "radius" : { "type" : "number" },
            "pivot" : { "$ref": "#/common_definitions/v2" }
        }
    })json";

    bool factory_loader<circle_collider>::operator()(
        circle_collider& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("radius") ) {
            f32 radius = component.radius();
            if ( !json_utils::try_parse_value(ctx.root["radius"], radius) ) {
                the<debug>().error("CIRCLE_COLLIDER: Incorrect formatting of 'radius' property");
                return false;
            }
            component.radius(radius);
        }

        if ( ctx.root.HasMember("pivot") ) {
            v2f pivot = component.pivot();
            if ( !json_utils::try_parse_value(ctx.root["pivot"], pivot) ) {
                the<debug>().error("CIRCLE_COLLIDER: Incorrect formatting of 'pivot' property");
                return false;
            }
            component.pivot(pivot);
        }

        return true;
    }

    bool factory_loader<circle_collider>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<polygon_collider>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "points" : { "$ref": "#/definitions/points" },
            "pivot" : { "$ref": "#/common_definitions/v2" }
        },
        "definitions" : {
            "points" : {
                "type" : "array",
                "items" : { "$ref": "#/common_definitions/v2" }
            }
        }
    })json";

    bool factory_loader<polygon_collider>::operator()(
        polygon_collider& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("points") ) {
            vector<v2f> points = component.points();
            if ( !json_utils::try_parse_value(ctx.root["points"], points) ) {
                the<debug>().error("POLYGON_COLLIDER: Incorrect formatting of 'points' property");
                return false;
            }
            component.points(std::move(points));
        }

        if ( ctx.root.HasMember("pivot") ) {
            v2f pivot = component.pivot();
            if ( !json_utils::try_parse_value(ctx.root["pivot"], pivot) ) {
                the<debug>().error("POLYGON_COLLIDER: Incorrect formatting of 'pivot' property");
                return false;
            }
            component.pivot(pivot);
        }

        return true;
    }

    bool factory_loader<polygon_collider>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}
