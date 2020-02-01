/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/rigid_body.hpp>

namespace e2d
{
    const char* factory_loader<rigid_body>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "type" : { "$ref": "#/definitions/types" },
            "sleeping" : { "$ref": "#/definitions/sleepings" },
            "collision" : { "$ref": "#/definitions/collisions" },
            "gravity_scale" : { "type" : "number" },
            "fixed_rotation" : { "type" : "boolean" }
        },
        "definitions" : {
            "types" : {
                "type" : "string",
                "enum" : [
                    "dynamic",
                    "kinematic"
                ]
            },
            "sleepings" : {
                "type" : "string",
                "enum" : [
                    "never",
                    "start_awake",
                    "start_asleep"
                ]
            },
            "collisions" : {
                "type" : "string",
                "enum" : [
                    "discrete",
                    "continuous"
                ]
            }
        }
    })json";

    bool factory_loader<rigid_body>::operator()(
        rigid_body& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("type") ) {
            rigid_body::types type = component.type();
            if ( !json_utils::try_parse_value(ctx.root["type"], type) ) {
                the<debug>().error("RIGID_BODY: Incorrect formatting of 'type' property");
                return false;
            }
            component.type(type);
        }

        if ( ctx.root.HasMember("sleeping") ) {
            rigid_body::sleepings sleeping = component.sleeping();
            if ( !json_utils::try_parse_value(ctx.root["sleeping"], sleeping) ) {
                the<debug>().error("RIGID_BODY: Incorrect formatting of 'sleeping' property");
                return false;
            }
            component.sleeping(sleeping);
        }

        if ( ctx.root.HasMember("collision") ) {
            rigid_body::collisions collision = component.collision();
            if ( !json_utils::try_parse_value(ctx.root["collision"], collision) ) {
                the<debug>().error("RIGID_BODY: Incorrect formatting of 'collision' property");
                return false;
            }
            component.collision(collision);
        }

        if ( ctx.root.HasMember("gravity_scale") ) {
            f32 gravity_scale = component.gravity_scale();
            if ( !json_utils::try_parse_value(ctx.root["gravity_scale"], gravity_scale) ) {
                the<debug>().error("RIGID_BODY: Incorrect formatting of 'gravity_scale' property");
                return false;
            }
            component.gravity_scale(gravity_scale);
        }

        if ( ctx.root.HasMember("fixed_rotation") ) {
            bool fixed_rotation = component.fixed_rotation();
            if ( !json_utils::try_parse_value(ctx.root["fixed_rotation"], fixed_rotation) ) {
                the<debug>().error("RIGID_BODY: Incorrect formatting of 'fixed_rotation' property");
                return false;
            }
            component.fixed_rotation(fixed_rotation);
        }

        return true;
    }

    bool factory_loader<rigid_body>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<rigid_body>::title = ICON_FA_WEIGHT_HANGING " rigid_body";

    void component_inspector<rigid_body>::operator()(gcomponent<rigid_body>& c) const {
        if ( rigid_body::types type = c->type();
            imgui_utils::show_enum_combo_box("type", &type) )
        {
            c->type(type);
        }

        if ( rigid_body::sleepings sleeping = c->sleeping();
            imgui_utils::show_enum_combo_box("sleeping", &sleeping) )
        {
            c->sleeping(sleeping);
        }

        if ( rigid_body::collisions collision = c->collision();
            imgui_utils::show_enum_combo_box("collision", &collision) )
        {
            c->collision(collision);
        }

        if ( f32 gravity_scale = c->gravity_scale();
            ImGui::DragFloat("gravity_scale", &gravity_scale, 0.01f) )
        {
            c->gravity_scale(gravity_scale);
        }

        if ( bool fixed_rotation = c->fixed_rotation();
            ImGui::Checkbox("fixed_rotation", &fixed_rotation) )
        {
            c->fixed_rotation(fixed_rotation);
        }
    }
}
