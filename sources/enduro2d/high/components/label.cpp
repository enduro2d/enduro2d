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
            "font" : { "$ref": "#/common_definitions/address" },
            "text" : { "type" : "string" },
            "pivot" : { "$ref": "#/common_definitions/v2" },
            "tint" : { "$ref": "#/common_definitions/color" }
        }
    })json";

    bool factory_loader<label>::operator()(
        label& component,
        const fill_context& ctx) const
    {
        i32 member_count = ctx.root.MemberCount();
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

        if ( ctx.root.HasMember("text") ) {
            str32 text;
            if ( !json_utils::try_parse_value(ctx.root["text"], text) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'text' property");
                return false;
            }
            component.text(text);
        }

        if ( ctx.root.HasMember("pivot") ) {
            v2f pivot;
            if ( !json_utils::try_parse_value(ctx.root["pivot"], pivot) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'pivot' property");
                return false;
            }
            component.pivot(pivot);
        }

        if ( ctx.root.HasMember("tint") ) {
            auto tint = component.tint();
            if ( !json_utils::try_parse_value(ctx.root["tint"], tint) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'tint' property");
                return false;
            }
            component.tint(tint);
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

    
    const char* factory_loader<label::label_dirty>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
    })json";

    bool factory_loader<label::label_dirty>::operator()(
        label::label_dirty& component,
        const fill_context& ctx) const
    {
        return true;
    }

    bool factory_loader<label::label_dirty>::operator()(
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
