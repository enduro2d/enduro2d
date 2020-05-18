/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/progress.hpp>

namespace e2d
{
    const char* factory_loader<progress>::schema_source = R"json({
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

    bool factory_loader<progress>::operator()(
        progress& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("value") ) {
            f32 value = component.value();
            if ( !json_utils::try_parse_value(ctx.root["value"], value) ) {
                the<debug>().error("progress: Incorrect formatting of 'value' property");
                return false;
            }
            component.value(value);
        }

        if ( ctx.root.HasMember("direction") ) {
            progress::directions direction = component.direction();
            if ( !json_utils::try_parse_value(ctx.root["direction"], direction) ) {
                the<debug>().error("progress: Incorrect formatting of 'direction' property");
                return false;
            }
            component.direction(direction);
        }

        return true;
    }

    bool factory_loader<progress>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<progress>::title = ICON_FA_SLIDERS_H " progress";

    void component_inspector<progress>::operator()(gcomponent<progress>& c) const {
        if ( f32 value = c->value();
            ImGui::SliderFloat("value", &value, 0.f, 1.f) )
        {
            c->value(value);
        }

        if ( progress::directions direction = c->direction();
            imgui_utils::show_enum_combo_box("direction", &direction) )
        {
            c->direction(direction);
        }
    }
}
