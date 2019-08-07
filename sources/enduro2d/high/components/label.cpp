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
            "width" : { "type" : "number" },
            "halign" : { "$ref": "#/definitions/haligns" },
            "valign" : { "$ref": "#/definitions/valigns" },
            "smoothing" : { "type" : "number" },
            "outline_distance" : { "type" : "number" },
            "outline_color" : { "$ref": "#/common_definitions/color" },
            "shadow_smoothing" : { "type" : "number" },
            "shadow_offset" : { "$ref": "#/common_definitions/v2" },
            "shadow_color" : { "$ref": "#/common_definitions/color" }
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

        if ( ctx.root.HasMember("width") ) {
            f32 width = component.width();
            if ( !json_utils::try_parse_value(ctx.root["width"], width) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'width' property");
                return false;
            }
            component.width(width);
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

        if ( ctx.root.HasMember("smoothing") ) {
            f32 smoothing = component.smoothing();
            if ( !json_utils::try_parse_value(ctx.root["smoothing"], smoothing) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'smoothing' property");
                return false;
            }
            component.smoothing(smoothing);
        }

        if ( ctx.root.HasMember("outline_distance") ) {
            f32 outline_distance = component.outline_distance();
            if ( !json_utils::try_parse_value(ctx.root["outline_distance"], outline_distance) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'outline_distance' property");
                return false;
            }
            component.outline_distance(outline_distance);
        }

        if ( ctx.root.HasMember("outline_color") ) {
            color32 outline_color = component.outline_color();
            if ( !json_utils::try_parse_value(ctx.root["outline_color"], outline_color) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'outline_color' property");
                return false;
            }
            component.outline_color(outline_color);
        }

        if ( ctx.root.HasMember("shadow_smoothing") ) {
            f32 shadow_smoothing = component.shadow_smoothing();
            if ( !json_utils::try_parse_value(ctx.root["shadow_smoothing"], shadow_smoothing) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'shadow_smoothing' property");
                return false;
            }
            component.shadow_smoothing(shadow_smoothing);
        }

        if ( ctx.root.HasMember("shadow_offset") ) {
            v2f shadow_offset = component.shadow_offset();
            if ( !json_utils::try_parse_value(ctx.root["shadow_offset"], shadow_offset) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'shadow_offset' property");
                return false;
            }
            component.shadow_offset(shadow_offset);
        }

        if ( ctx.root.HasMember("shadow_color") ) {
            color32 shadow_color = component.shadow_color();
            if ( !json_utils::try_parse_value(ctx.root["shadow_color"], shadow_color) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'shadow_color' property");
                return false;
            }
            component.shadow_color(shadow_color);
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
