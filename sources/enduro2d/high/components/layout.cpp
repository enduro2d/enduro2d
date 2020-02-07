/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/layout.hpp>

namespace e2d
{
    const char* factory_loader<layout>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "mode" : { "$ref": "#/definitions/modes" },
            "valign" : { "$ref": "#/definitions/valigns" },
            "halign" : { "$ref": "#/definitions/haligns" },
            "spacing" : { "type" : "number" }
        },
        "definitions" : {
            "modes" : {
                "type" : "string",
                "enum" : [
                    "vertical",
                    "horizontal"
                ]
            },
            "valigns" : {
                "type" : "string",
                "enum" : [
                    "top",
                    "center",
                    "bottom"
                ]
            },
            "haligns" : {
                "type" : "string",
                "enum" : [
                    "left",
                    "center",
                    "right"
                ]
            }
        }
    })json";

    bool factory_loader<layout>::operator()(
        layout& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("mode") ) {
            layout::modes mode = component.mode();
            if ( !json_utils::try_parse_value(ctx.root["mode"], mode) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'mode' property");
                return false;
            }
            component.mode(mode);
        }

        if ( ctx.root.HasMember("valign") ) {
            layout::valigns valign = component.valign();
            if ( !json_utils::try_parse_value(ctx.root["valign"], valign) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'valign' property");
                return false;
            }
            component.valign(valign);
        }

        if ( ctx.root.HasMember("halign") ) {
            layout::haligns halign = component.halign();
            if ( !json_utils::try_parse_value(ctx.root["halign"], halign) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'halign' property");
                return false;
            }
            component.halign(halign);
        }

        if ( ctx.root.HasMember("spacing") ) {
            f32 spacing = component.spacing();
            if ( !json_utils::try_parse_value(ctx.root["spacing"], spacing) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'spacing' property");
                return false;
            }
            component.spacing(spacing);
        }

        return true;
    }

    bool factory_loader<layout>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<layout_item>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "size" : { "$ref": "#/common_definitions/v2" }
        }
    })json";

    bool factory_loader<layout_item>::operator()(
        layout_item& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("size") ) {
            v2f size = component.size();
            if ( !json_utils::try_parse_value(ctx.root["size"], size) ) {
                the<debug>().error("LAYOUT_ITEM: Incorrect formatting of 'size' property");
                return false;
            }
            component.size(size);
        }

        return true;
    }

    bool factory_loader<layout_item>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<layout>::title = ICON_FA_BARS " layout";

    void component_inspector<layout>::operator()(gcomponent<layout>& c) const {
        if ( layout::modes mode = c->mode();
            imgui_utils::show_enum_combo_box("mode", &mode) )
        {
            c->mode(mode);
        }

        if ( layout::valigns valign = c->valign();
            imgui_utils::show_enum_combo_box("valign", &valign) )
        {
            c->valign(valign);
        }

        if ( layout::haligns halign = c->halign();
            imgui_utils::show_enum_combo_box("halign", &halign) )
        {
            c->halign(halign);
        }

        if ( f32 spacing = c->spacing();
            ImGui::DragFloat("spacing", &spacing, 1.f) )
        {
            c->spacing(spacing);
        }
    }
}

namespace e2d
{
    const char* component_inspector<layout_item>::title = ICON_FA_GRIP_LINES " layout_item";

    void component_inspector<layout_item>::operator()(gcomponent<layout_item>& c) const {
        if ( v2f size = c->size();
            ImGui::DragFloat2("size", size.data(), 1.f) )
        {
            c->size(size);
        }
    }

    void component_inspector<layout_item>::operator()(
        gcomponent<layout_item>& c,
        gizmos_context& ctx) const
    {
        ctx.draw_wire_rect(
            c->size() * 0.5f,
            c->size(),
            ctx.selected() ? color32(255,255,255) : color32(127,127,127));
    }
}
