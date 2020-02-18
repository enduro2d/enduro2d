/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/masks.hpp>

namespace
{
    using namespace e2d;

    template < typename Mask >
    bool mask_base_loader(
        impl::mask_base<Mask>& component,
        const factory_loader<>::fill_context& ctx)
    {
        if ( ctx.root.HasMember("offset") ) {
            v2f offset = component.offset();
            if ( !json_utils::try_parse_value(ctx.root["offset"], offset) ) {
                the<debug>().error("MASK_BASE: Incorrect formatting of 'offset' property");
                return false;
            }
            component.offset(offset);
        }

        if ( ctx.root.HasMember("visible") ) {
            bool visible = component.visible();
            if ( !json_utils::try_parse_value(ctx.root["visible"], visible) ) {
                the<debug>().error("MASK_BASE: Incorrect formatting of 'visible' property");
                return false;
            }
            component.visible(visible);
        }

        return true;
    }

    template < typename Mask >
    void mask_base_inspector(impl::mask_base<Mask>& c) {
        if ( v2f offset = c.offset();
            ImGui::DragFloat2("offset", offset.data(), 1.f) )
        {
            c.offset(offset);
        }

        if ( bool visible = c.visible();
            ImGui::Checkbox("visible", &visible) )
        {
            c.visible(visible);
        }
    }
}

namespace e2d
{
    const char* factory_loader<rect_mask>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "size" : { "$ref": "#/common_definitions/v2" },
            "offset" : { "$ref": "#/common_definitions/v2" },
            "visible" : { "type" : "boolean" }
        }
    })json";

    bool factory_loader<rect_mask>::operator()(
        rect_mask& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("size") ) {
            v2f size = component.size();
            if ( !json_utils::try_parse_value(ctx.root["size"], size) ) {
                the<debug>().error("RECT_MASK: Incorrect formatting of 'size' property");
                return false;
            }
            component.size(size);
        }

        return mask_base_loader(component, ctx);
    }

    bool factory_loader<rect_mask>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<circle_mask>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "radius" : { "type" : "number" },
            "offset" : { "$ref": "#/common_definitions/v2" },
            "visible" : { "type" : "boolean" }
        }
    })json";

    bool factory_loader<circle_mask>::operator()(
        circle_mask& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("radius") ) {
            f32 radius = component.radius();
            if ( !json_utils::try_parse_value(ctx.root["radius"], radius) ) {
                the<debug>().error("CIRCLE_MASK: Incorrect formatting of 'radius' property");
                return false;
            }
            component.radius(radius);
        }

        return mask_base_loader(component, ctx);
    }

    bool factory_loader<circle_mask>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<sprite_mask>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "atlas" : { "$ref": "#/common_definitions/address" },
            "sprite" : { "$ref": "#/common_definitions/address" },
            "threshold" : { "type" : "number" },
            "offset" : { "$ref": "#/common_definitions/v2" },
            "visible" : { "type" : "boolean" }
        }
    })json";

    bool factory_loader<sprite_mask>::operator()(
        sprite_mask& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("atlas") ) {
            auto sprite = ctx.dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(ctx.parent_address, ctx.root["atlas"].GetString()));
            if ( !sprite ) {
                the<debug>().error("SPRITE_MASK: Dependency 'atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["atlas"].GetString());
                return false;
            }
            component.sprite(sprite);
        }

        if ( ctx.root.HasMember("sprite") ) {
            auto sprite = ctx.dependencies.find_asset<sprite_asset>(
                path::combine(ctx.parent_address, ctx.root["sprite"].GetString()));
            if ( !sprite ) {
                the<debug>().error("SPRITE_MASK: Dependency 'sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["sprite"].GetString());
                return false;
            }
            component.sprite(sprite);
        }

        if ( ctx.root.HasMember("threshold") ) {
            f32 threshold = component.threshold();
            if ( !json_utils::try_parse_value(ctx.root["threshold"], threshold) ) {
                the<debug>().error("SPRITE_MASK: Incorrect formatting of 'threshold' property");
                return false;
            }
            component.threshold(threshold);
        }

        return mask_base_loader(component, ctx);
    }

    bool factory_loader<sprite_mask>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(ctx.parent_address, ctx.root["atlas"].GetString()));
        }

        if ( ctx.root.HasMember("sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(ctx.parent_address, ctx.root["sprite"].GetString()));
        }

        return true;
    }
}

namespace e2d
{
    const char* component_inspector<rect_mask>::title = ICON_FA_MASK " rect_mask";

    void component_inspector<rect_mask>::operator()(gcomponent<rect_mask>& c) const {
        if ( v2f size = c->size();
            ImGui::DragFloat2("size", size.data(), 1.f) )
        {
            c->size(size);
        }

        mask_base_inspector(*c);
    }

    void component_inspector<rect_mask>::operator()(
        gcomponent<rect_mask>& c,
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
    const char* component_inspector<circle_mask>::title = ICON_FA_MASK " circle_mask";

    void component_inspector<circle_mask>::operator()(gcomponent<circle_mask>& c) const {
        if ( f32 radius = c->radius();
            ImGui::DragFloat("radius", &radius, 1.f) )
        {
            c->radius(radius);
        }

        mask_base_inspector(*c);
    }

    void component_inspector<circle_mask>::operator()(
        gcomponent<circle_mask>& c,
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
    const char* component_inspector<sprite_mask>::title = ICON_FA_MASK " sprite_mask";

    void component_inspector<sprite_mask>::operator()(gcomponent<sprite_mask>& c) const {
        ///TODO(BlackMat): add 'sprite' inspector

        if ( f32 threshold = c->threshold();
            ImGui::SliderFloat("threshold", &threshold, 0.f, 1.f) )
        {
            c->threshold(threshold);
        }

        mask_base_inspector(*c);
    }

    void component_inspector<sprite_mask>::operator()(
        gcomponent<sprite_mask>& c,
        gizmos_context& ctx) const
    {
        if ( const sprite_asset::ptr& spr_a = c->sprite() ) {
            const sprite& spr = spr_a->content();
            ctx.draw_wire_rect(
                c->offset() + (spr.texrect().position - spr.pivot()) + spr.texrect().size * 0.5f,
                spr.texrect().size,
                ctx.selected() ? color32::yellow() : color32::magenta());
        }
    }
}
