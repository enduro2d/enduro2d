/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/colliders.hpp>

namespace
{
    using namespace e2d;

    template < typename Collider >
    bool collider_base_loader(
        impl::collider_base<Collider>& component,
        const factory_loader<>::fill_context& ctx)
    {
        if ( ctx.root.HasMember("offset") ) {
            v2f offset = component.offset();
            if ( !json_utils::try_parse_value(ctx.root["offset"], offset) ) {
                the<debug>().error("COLLIDER_BASE: Incorrect formatting of 'offset' property");
                return false;
            }
            component.offset(offset);
        }

        return true;
    }

    template < typename Collider >
    void collider_base_inspector(impl::collider_base<Collider>& c) {
        if ( v2f offset = c.offset();
            ImGui::DragFloat2("offset", offset.data(), 1.f) )
        {
            c.offset(offset);
        }
    }
}

namespace e2d
{
    const char* factory_loader<rect_collider>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "size" : { "$ref": "#/common_definitions/v2" },
            "offset" : { "$ref": "#/common_definitions/v2" }
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

        return collider_base_loader(component, ctx);
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
            "offset" : { "$ref": "#/common_definitions/v2" }
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

        return collider_base_loader(component, ctx);
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
            "offset" : { "$ref": "#/common_definitions/v2" }
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

        return collider_base_loader(component, ctx);
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
    const char* component_inspector<rect_collider>::title = ICON_FA_DICE_FOUR " rect_collider";

    void component_inspector<rect_collider>::operator()(gcomponent<rect_collider>& c) const {
        if ( v2f size = c->size();
            ImGui::DragFloat2("size", size.data(), 1.f) )
        {
            c->size(size);
        }

        collider_base_inspector(*c);
    }

    void component_inspector<rect_collider>::operator()(
        gcomponent<rect_collider>& c,
        gizmos_context& ctx) const
    {
        ctx.draw_wire_rect(
            c->offset(),
            c->size(),
            ctx.selected() ? color32::cyan() : color32::green());
    }
}

namespace e2d
{
    const char* component_inspector<circle_collider>::title = ICON_FA_DICE_ONE " circle_collider";

    void component_inspector<circle_collider>::operator()(gcomponent<circle_collider>& c) const {
        if ( f32 radius = c->radius();
            ImGui::DragFloat("radius", &radius, 1.f) )
        {
            c->radius(radius);
        }

        collider_base_inspector(*c);
    }

    void component_inspector<circle_collider>::operator()(
        gcomponent<circle_collider>& c,
        gizmos_context& ctx) const
    {
        const u32 segments = math::numeric_cast<u32>(
            math::to_rad_v(math::two_pi<f32>()) *
            c->radius() *
            0.2f);

        ctx.draw_wire_circle(
            c->offset(),
            c->radius(),
            segments,
            ctx.selected() ? color32::cyan() : color32::green());
    }
}

namespace e2d
{
    const char* component_inspector<polygon_collider>::title = ICON_FA_DICE_SIX " polygon_collider";

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

        collider_base_inspector(*c);
    }

    void component_inspector<polygon_collider>::operator()(
        gcomponent<polygon_collider>& c,
        gizmos_context& ctx) const
    {
        ctx.draw_wire_polygon(
            c->points(),
            c->offset(),
            ctx.selected() ? color32::cyan() : color32::green());
    }
}
