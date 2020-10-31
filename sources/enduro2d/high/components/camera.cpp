/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/camera.hpp>

namespace e2d
{
    const char* factory_loader<camera>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "depth" : { "type" : "integer" },
            "mode" : { "$ref": "#/definitions/modes" },
            "znear" : { "type" : "number" },
            "zfar" : { "type" : "number" },
            "view" : { "$ref": "#/common_definitions/m4" },
            "viewport" : { "$ref": "#/common_definitions/b2" },
            "projection" : { "$ref": "#/common_definitions/m4" },
            "background" : { "$ref": "#/common_definitions/color" }
        },
        "definitions" : {
            "modes" : {
                "type" : "string",
                "enum" : [
                    "manual",
                    "stretch",
                    "flexible",
                    "fixed_fit",
                    "fixed_crop"
                ]
            }
        }
    })json";

    bool factory_loader<camera>::operator()(
        camera& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("depth") ) {
            i32 depth = component.depth();
            if ( !json_utils::try_parse_value(ctx.root["depth"], depth) ) {
                the<debug>().error("CAMERA: Incorrect formatting of 'depth' property");
                return false;
            }
            component.depth(depth);
        }

        if ( ctx.root.HasMember("mode") ) {
            camera::modes mode = component.mode();
            if ( !json_utils::try_parse_value(ctx.root["mode"], mode) ) {
                the<debug>().error("CAMERA: Incorrect formatting of 'mode' property");
                return false;
            }
            component.mode(mode);
        }

        if ( ctx.root.HasMember("znear") ) {
            f32 znear = component.znear();
            if ( !json_utils::try_parse_value(ctx.root["znear"], znear) ) {
                the<debug>().error("CAMERA: Incorrect formatting of 'znear' property");
                return false;
            }
            component.znear(znear);
        }

        if ( ctx.root.HasMember("zfar") ) {
            f32 zfar = component.zfar();
            if ( !json_utils::try_parse_value(ctx.root["zfar"], zfar) ) {
                the<debug>().error("CAMERA: Incorrect formatting of 'zfar' property");
                return false;
            }
            component.zfar(zfar);
        }

        if ( ctx.root.HasMember("view") ) {
            m4f view = component.view();
            if ( !json_utils::try_parse_value(ctx.root["view"], view) ) {
                the<debug>().error("CAMERA: Incorrect formatting of 'view' property");
                return false;
            }
            component.view(view);
        }

        if ( ctx.root.HasMember("viewport") ) {
            b2f viewport = component.viewport();
            if ( !json_utils::try_parse_value(ctx.root["viewport"], viewport) ) {
                the<debug>().error("CAMERA: Incorrect formatting of 'viewport' property");
                return false;
            }
            component.viewport(viewport);
        }

        if ( ctx.root.HasMember("projection") ) {
            m4f projection = component.projection();
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
            color background = component.background();
            if ( !json_utils::try_parse_value(ctx.root["background"], background) ) {
                the<debug>().error("CAMERA: Incorrect formatting of 'background' property");
                return false;
            }
            component.background(background);
        }

        return true;
    }

    bool factory_loader<camera>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<camera::input>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
    })json";

    bool factory_loader<camera::input>::operator()(
        camera::input& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<camera::input>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<camera::gizmos>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
    })json";

    bool factory_loader<camera::gizmos>::operator()(
        camera::gizmos& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<camera::gizmos>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<camera>::title = ICON_FA_VIDEO " camera";

    void component_inspector<camera>::operator()(gcomponent<camera>& c) const {
        if ( bool input = c.component<camera::input>().exists();
            ImGui::Checkbox("input", &input) )
        {
            if ( input ) {
                c.component<camera::input>().ensure();
            } else {
                c.component<camera::input>().remove();
            }
        }

        ImGui::SameLine();

        if ( bool gizmos = c.component<camera::gizmos>().exists();
            ImGui::Checkbox("gizmos", &gizmos) )
        {
            if ( gizmos ) {
                c.component<camera::gizmos>().ensure();
            } else {
                c.component<camera::gizmos>().remove();
            }
        }

        if ( i32 depth = c->depth();
            ImGui::DragInt("depth", &depth, 1.f) )
        {
            c->depth(depth);
        }

        if ( camera::modes mode = c->mode();
            imgui_utils::show_enum_combo_box("mode", &mode) )
        {
            c->mode(mode);
        }

        if ( ImGui::TreeNode("clipping") ) {
            DEFER([](){ ImGui::TreePop(); });

            if ( f32 znear = c->znear();
                ImGui::DragFloat("znear", &znear, 1.f) )
            {
                c->znear(znear);
                c->zfar(math::max(c->zfar(), c->znear()));
            }

            if ( f32 zfar = c->zfar();
                ImGui::DragFloat("zfar", &zfar, 1.f) )
            {
                c->zfar(zfar);
                c->znear(math::min(c->znear(), c->zfar()));
            }
        }

        ///TODO(BlackMat): add 'view' inspector

        if ( ImGui::TreeNode("viewport") ) {
            DEFER([](){ ImGui::TreePop(); });

            if ( b2f viewport = c->viewport();
                ImGui::DragFloat2("position", viewport.position.data(), 0.01f) )
            {
                c->viewport(viewport);
            }

            if ( b2f viewport = c->viewport();
                ImGui::DragFloat2("size", viewport.size.data(), 0.01f, 0.f, std::numeric_limits<f32>::max()) )
            {
                c->viewport(viewport);
            }
        }

        ///TODO(BlackMat): add 'projection' inspector
        ///TODO(BlackMat): add 'target' inspector

        if ( color background = c->background();
            ImGui::ColorEdit4("background", background.data()) )
        {
            c->background(background);
        }
    }
}
