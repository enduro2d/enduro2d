/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/label.hpp>

namespace
{
    using namespace e2d;

    bool parse_halign(str_view str, label::haligns& align) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { align = label::haligns::x; return true; }
        DEFINE_IF(left);
        DEFINE_IF(center);
        DEFINE_IF(right);
    #undef DEFINE_IF
        return false;
    }

    bool parse_valign(str_view str, label::valigns& align) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { align = label::valigns::x; return true; }
        DEFINE_IF(top);
        DEFINE_IF(center);
        DEFINE_IF(bottom);
        DEFINE_IF(baseline);
    #undef DEFINE_IF
        return false;
    }
}

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
            str32 text = component.text();
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
            if ( !parse_halign(ctx.root["halign"].GetString(), halign) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'halign' property");
                return false;
            }
            component.haligh(halign);
        }

        if ( ctx.root.HasMember("valign") ) {
            label::valigns valign = component.valign();
            if ( !parse_valign(ctx.root["valign"].GetString(), valign) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'valign' property");
                return false;
            }
            component.valigh(valign);
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
