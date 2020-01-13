/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/label.hpp>

namespace e2d
{
    const char* factory_loader<label>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "text" : { "type" : "string" },
            "font" : { "$ref": "#/common_definitions/address" },
            "tint" : { "$ref": "#/common_definitions/color" },
            "halign" : { "$ref": "#/definitions/haligns" },
            "valign" : { "$ref": "#/definitions/valigns" },
            "leading" : { "type" : "number" },
            "tracking" : { "type" : "number" },
            "text_width" : { "type" : "number" },
            "glyph_dilate" : { "type" : "number" },
            "outline_width" : { "type" : "number" },
            "outline_color" : { "$ref": "#/common_definitions/color" }
        },
        "definitions" : {
            "haligns" : {
                "type" : "string",
                "enum" : [
                    "left",
                    "center",
                    "right"
                ]
            },
            "valigns" : {
                "type" : "string",
                "enum" : [
                    "top",
                    "center",
                    "bottom",
                    "baseline"
                ]
            }
        }
    })json";

    bool factory_loader<label>::operator()(
        label& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("text") ) {
            str text = component.text();
            if ( !json_utils::try_parse_value(ctx.root["text"], text) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'text' property");
                return false;
            }
            component.text(std::move(text));
        }

        if ( ctx.root.HasMember("font") ) {
            auto font = ctx.dependencies.find_asset<font_asset>(
                path::combine(ctx.parent_address, ctx.root["font"].GetString()));
            if ( !font ) {
                the<debug>().error("LABEL: Dependency 'font' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["font"].GetString());
                return false;
            }
            component.font(font);
        }

        if ( ctx.root.HasMember("tint") ) {
            color32 tint = component.tint();
            if ( !json_utils::try_parse_value(ctx.root["tint"], tint) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'tint' property");
                return false;
            }
            component.tint(tint);
        }

        if ( ctx.root.HasMember("halign") ) {
            label::haligns halign = component.halign();
            if ( !json_utils::try_parse_value(ctx.root["halign"], halign) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'halign' property");
                return false;
            }
            component.halign(halign);
        }

        if ( ctx.root.HasMember("valign") ) {
            label::valigns valign = component.valign();
            if ( !json_utils::try_parse_value(ctx.root["valign"], valign) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'valign' property");
                return false;
            }
            component.valign(valign);
        }

        if ( ctx.root.HasMember("leading") ) {
            f32 leading = component.leading();
            if ( !json_utils::try_parse_value(ctx.root["leading"], leading) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'leading' property");
                return false;
            }
            component.leading(leading);
        }

        if ( ctx.root.HasMember("tracking") ) {
            f32 tracking = component.tracking();
            if ( !json_utils::try_parse_value(ctx.root["tracking"], tracking) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'tracking' property");
                return false;
            }
            component.tracking(tracking);
        }

        if ( ctx.root.HasMember("text_width") ) {
            f32 text_width = component.text_width();
            if ( !json_utils::try_parse_value(ctx.root["text_width"], text_width) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'text_width' property");
                return false;
            }
            component.text_width(text_width);
        }

        if ( ctx.root.HasMember("glyph_dilate") ) {
            f32 glyph_dilate = component.glyph_dilate();
            if ( !json_utils::try_parse_value(ctx.root["glyph_dilate"], glyph_dilate) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'glyph_dilate' property");
                return false;
            }
            component.glyph_dilate(glyph_dilate);
        }

        if ( ctx.root.HasMember("outline_width") ) {
            f32 outline_width = component.outline_width();
            if ( !json_utils::try_parse_value(ctx.root["outline_width"], outline_width) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'outline_width' property");
                return false;
            }
            component.outline_width(outline_width);
        }

        if ( ctx.root.HasMember("outline_color") ) {
            color32 outline_color = component.outline_color();
            if ( !json_utils::try_parse_value(ctx.root["outline_color"], outline_color) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'outline_color' property");
                return false;
            }
            component.outline_color(outline_color);
        }

        return true;
    }

    bool factory_loader<label>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("font") ) {
            dependencies.add_dependency<font_asset>(
                path::combine(ctx.parent_address, ctx.root["font"].GetString()));
        }

        return true;
    }
}

namespace e2d
{
    const char* factory_loader<label::dirty>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
    })json";

    bool factory_loader<label::dirty>::operator()(
        label::dirty& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<label::dirty>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<label>::title = "label";

    void component_inspector<label>::operator()(gcomponent<label>& c) const {
        if ( str text = c->text();
            ImGui::InputTextMultiline("text", &text) )
        {
            c->text(std::move(text));
            c.owner().component<label::dirty>().ensure();
        }

        ///TODO(BlackMat): add 'font' inspector

        if ( color tint = color(c->tint());
            ImGui::ColorEdit4("tint", tint.data()) )
        {
            c->tint(color32(tint));
            c.owner().component<label::dirty>().ensure();
        }

        if ( label::haligns halign = c->halign();
            imgui_utils::show_enum_combo_box("halign", &halign) )
        {
            c->halign(halign);
            c.owner().component<label::dirty>().ensure();
        }

        if ( label::valigns valign = c->valign();
            imgui_utils::show_enum_combo_box("valign", &valign) )
        {
            c->valign(valign);
            c.owner().component<label::dirty>().ensure();
        }

        if ( f32 leading = c->leading();
            ImGui::DragFloat("leading", &leading, 0.01f) )
        {
            c->leading(leading);
            c.owner().component<label::dirty>().ensure();
        }

        if ( f32 tracking = c->tracking();
            ImGui::DragFloat("tracking", &tracking, 0.01f) )
        {
            c->tracking(tracking);
            c.owner().component<label::dirty>().ensure();
        }

        if ( f32 text_width = c->text_width();
            ImGui::DragFloat("text_width", &text_width, 1.f) )
        {
            c->text_width(text_width);
            c.owner().component<label::dirty>().ensure();
        }

        if ( f32 glyph_dilate = c->glyph_dilate();
            ImGui::SliderFloat("glyph_dilate", &glyph_dilate, -1.f, 1.f) )
        {
            c->glyph_dilate(glyph_dilate);
            c.owner().component<label::dirty>().ensure();
        }

        if ( f32 outline_width = c->outline_width();
            ImGui::SliderFloat("outline_width", &outline_width, 0.f, 1.f) )
        {
            c->outline_width(outline_width);
            c.owner().component<label::dirty>().ensure();
        }

        if ( color outline_color = color(c->outline_color());
            ImGui::ColorEdit4("outline_color", outline_color.data()) )
        {
            c->outline_color(color32(outline_color));
            c.owner().component<label::dirty>().ensure();
        }
    }
}
