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
            "mode" : { "$ref": "#/definitions/modes" }
        },
        "definitions" : {
            "modes" : {
                "type" : "string",
                "enum" : [
                    "vertical",
                    "horizontal"
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
        "properties" : {}
    })json";

    bool factory_loader<layout_item>::operator()(
        layout_item& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
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
    }
}

namespace e2d
{
    const char* component_inspector<layout_item>::title = ICON_FA_GRIP_LINES " layout_item";

    void component_inspector<layout_item>::operator()(gcomponent<layout_item>& c) const {
        E2D_UNUSED(c);
    }
}
