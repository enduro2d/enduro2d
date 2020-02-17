/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/mask.hpp>

namespace e2d
{
    const char* factory_loader<mask>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "visible" : { "type" : "boolean" },
            "atlas" : { "$ref": "#/common_definitions/address" },
            "sprite" : { "$ref": "#/common_definitions/address" },
            "alpha_threshold" : { "type" : "number" }
        }
    })json";

    bool factory_loader<mask>::operator()(
        mask& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("visible") ) {
            bool visible = component.visible();
            if ( !json_utils::try_parse_value(ctx.root["visible"], visible) ) {
                the<debug>().error("MASK: Incorrect formatting of 'visible' property");
                return false;
            }
            component.visible(visible);
        }

        if ( ctx.root.HasMember("atlas") ) {
            auto sprite = ctx.dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(ctx.parent_address, ctx.root["atlas"].GetString()));
            if ( !sprite ) {
                the<debug>().error("MASK: Dependency 'atlas' is not found:\n"
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
                the<debug>().error("MASK: Dependency 'sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["sprite"].GetString());
                return false;
            }
            component.sprite(sprite);
        }

        if ( ctx.root.HasMember("alpha_threshold") ) {
            f32 alpha_threshold = component.alpha_threshold();
            if ( !json_utils::try_parse_value(ctx.root["alpha_threshold"], alpha_threshold) ) {
                the<debug>().error("MASK: Incorrect formatting of 'alpha_threshold' property");
                return false;
            }
            component.alpha_threshold(alpha_threshold);
        }

        return true;
    }

    bool factory_loader<mask>::operator()(
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
    const char* component_inspector<mask>::title = ICON_FA_MASK " mask";

    void component_inspector<mask>::operator()(gcomponent<mask>& c) const {
        if ( bool visible = c->visible();
            ImGui::Checkbox("visible", &visible) )
        {
            c->visible(visible);
        }

        ///TODO(BlackMat): add 'sprite' inspector

        if ( f32 alpha_threshold = c->alpha_threshold();
            ImGui::SliderFloat("alpha_threshold", &alpha_threshold, 0.f, 1.f) )
        {
            c->alpha_threshold(alpha_threshold);
        }
    }
}
