/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/scrollbar.hpp>

namespace e2d
{
    const char* factory_loader<scrollbar>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "value" : { "type" : "number" },
            "direction" : { "$ref": "#/definitions/directions" }
        },
        "definitions" : {
            "directions" : {
                "type" : "string",
                "enum" : [
                    "row",
                    "row_reversed",
                    "column",
                    "column_reversed"
                ]
            }
        }
    })json";

    bool factory_loader<scrollbar>::operator()(
        scrollbar& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("value") ) {
            f32 value = component.value();
            if ( !json_utils::try_parse_value(ctx.root["value"], value) ) {
                the<debug>().error("SCROLLBAR: Incorrect formatting of 'value' property");
                return false;
            }
            component.value(value);
        }

        if ( ctx.root.HasMember("direction") ) {
            scrollbar::directions direction = component.direction();
            if ( !json_utils::try_parse_value(ctx.root["direction"], direction) ) {
                the<debug>().error("SCROLLBAR: Incorrect formatting of 'direction' property");
                return false;
            }
            component.direction(direction);
        }

        return true;
    }

    bool factory_loader<scrollbar>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<scrollbar>::title = ICON_FA_SLIDERS_H " scrollbar";

    void component_inspector<scrollbar>::operator()(gcomponent<scrollbar>& c) const {
        if ( f32 value = c->value();
            ImGui::SliderFloat("value", &value, 0.f, 1.f) )
        {
            c->value(value);
        }

        if ( scrollbar::directions direction = c->direction();
            imgui_utils::show_enum_combo_box("direction", &direction) )
        {
            c->direction(direction);
        }
    }
}
