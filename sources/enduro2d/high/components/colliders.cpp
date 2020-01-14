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

namespace e2d
{
    const char* component_inspector<rect_collider>::title = "rect_collider";

    void component_inspector<rect_collider>::operator()(gcomponent<rect_collider>& c) const {
        if ( v2f size = c->size();
            ImGui::DragFloat2("size", size.data(), 1.f, 0.f, std::numeric_limits<f32>::max()) )
        {
            c->size(size);
        }

        if ( v2f pivot = c->pivot();
            ImGui::DragFloat2("pivot", pivot.data(), 0.01f) )
        {
            c->pivot(pivot);
        }
    }
}

namespace e2d
{
    const char* component_inspector<circle_collider>::title = "circle_collider";

    void component_inspector<circle_collider>::operator()(gcomponent<circle_collider>& c) const {
        if ( f32 radius = c->radius();
            ImGui::DragFloat("radius", &radius, 1.f, 0.f, std::numeric_limits<f32>::max()) )
        {
            c->radius(radius);
        }

        if ( v2f pivot = c->pivot();
            ImGui::DragFloat2("pivot", pivot.data(), 0.01f) )
        {
            c->pivot(pivot);
        }
    }
}

namespace e2d
{
    const char* component_inspector<polygon_collider>::title = "polygon_collider";

    void component_inspector<polygon_collider>::operator()(gcomponent<polygon_collider>& c) const {
        const str points_label = strings::rformat(
            "points(%0)###points",
            c->points().size());

        if ( ImGui::TreeNode(points_label.c_str()) ) {
            E2D_DEFER([](){ ImGui::TreePop(); });

            int count = math::numeric_cast<int>(c->points().size());
            if ( ImGui::DragInt("count", &count, 1.f, 0, std::numeric_limits<int>::max()) ) {
                c->points().resize(math::numeric_cast<std::size_t>(count));
            }

            for ( std::size_t i = 0; i < c->points().size(); ++i ) {
                ImGui::PushID(math::numeric_cast<int>(i));
                E2D_DEFER([](){ ImGui::PopID(); });

                if ( v2f point = c->points()[i];
                    ImGui::DragFloat2("###point", point.data(), 1.f) )
                {
                    c->points()[i] = point;
                }
            }
        }

        if ( v2f pivot = c->pivot();
            ImGui::DragFloat2("pivot", pivot.data(), 0.01f) )
        {
            c->pivot(pivot);
        }
    }
}
