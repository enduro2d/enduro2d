/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/slider.hpp>

namespace e2d
{
    const char* factory_loader<slider>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "min_value" : { "type" : "number" },
            "max_value" : { "type" : "number" },
            "value" : { "type" : "number" },
            "whole_numbers" : { "type" : "boolean" },
            "direction" : { "$ref": "#/definitions/directions" },
            "handle_style" : { "$ref": "#/common_definitions/address" }
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

    bool factory_loader<slider>::operator()(
        slider& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("min_value") ) {
            f32 min_value = component.min_value();
            if ( !json_utils::try_parse_value(ctx.root["min_value"], min_value) ) {
                the<debug>().error("SLIDER: Incorrect formatting of 'min_value' property");
                return false;
            }
            component.min_value(min_value);
        }

        if ( ctx.root.HasMember("max_value") ) {
            f32 max_value = component.max_value();
            if ( !json_utils::try_parse_value(ctx.root["max_value"], max_value) ) {
                the<debug>().error("SLIDER: Incorrect formatting of 'max_value' property");
                return false;
            }
            component.max_value(max_value);
        }

        if ( ctx.root.HasMember("value") ) {
            f32 value = component.value();
            if ( !json_utils::try_parse_value(ctx.root["value"], value) ) {
                the<debug>().error("SLIDER: Incorrect formatting of 'value' property");
                return false;
            }
            component.value(value);
        }

        if ( ctx.root.HasMember("whole_numbers") ) {
            bool whole_numbers = component.whole_numbers();
            if ( !json_utils::try_parse_value(ctx.root["whole_numbers"], whole_numbers) ) {
                the<debug>().error("SLIDER: Incorrect formatting of 'whole_numbers' property");
                return false;
            }
            component.whole_numbers(whole_numbers);
        }

        if ( ctx.root.HasMember("direction") ) {
            slider::directions direction = component.direction();
            if ( !json_utils::try_parse_value(ctx.root["direction"], direction) ) {
                the<debug>().error("SLIDER: Incorrect formatting of 'direction' property");
                return false;
            }
            component.direction(direction);
        }

        if ( ctx.root.HasMember("handle_style") ) {
            auto handle_style = ctx.dependencies.find_asset<button_style_asset>(
                path::combine(ctx.parent_address, ctx.root["handle_style"].GetString()));
            if ( !handle_style ) {
                the<debug>().error("SLIDER: Dependency 'handle_style' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["handle_style"].GetString());
                return false;
            }
            component.handle_style(handle_style);
        }

        return true;
    }

    bool factory_loader<slider>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("handle_style") ) {
            dependencies.add_dependency<button_style_asset>(
                path::combine(ctx.parent_address, ctx.root["handle_style"].GetString()));
        }

        return true;
    }
}

namespace e2d
{
    const char* component_inspector<slider>::title = ICON_FA_SLIDERS_H " slider";

    void component_inspector<slider>::operator()(gcomponent<slider>& c) const {
        if ( c->whole_numbers() ) {
            if ( i32 min_value = math::numeric_cast<i32>(c->min_value());
                ImGui::DragInt("min_value", &min_value) )
            {
                c->min_value(math::numeric_cast<f32>(min_value));
            }

            if ( i32 max_value = math::numeric_cast<i32>(c->max_value());
                ImGui::DragInt("max_value", &max_value) )
            {
                c->max_value(math::numeric_cast<f32>(max_value));
            }

            if ( i32 value = math::numeric_cast<i32>(c->value()),
                min_value = math::numeric_cast<i32>(c->min_value()),
                max_value = math::numeric_cast<i32>(c->max_value());
                ImGui::SliderInt("value", &value, min_value, max_value) )
            {
                c->value(math::numeric_cast<f32>(value));
            }
        } else {
            if ( f32 min_value = c->min_value();
                ImGui::DragFloat("min_value", &min_value, 0.1f) )
            {
                c->min_value(min_value);
            }

            if ( f32 max_value = c->max_value();
                ImGui::DragFloat("max_value", &max_value, 0.1f) )
            {
                c->max_value(max_value);
            }

            if ( f32 value = c->value(),
                min_value = c->min_value(),
                max_value = c->max_value();
                ImGui::SliderFloat("value", &value, min_value, max_value) )
            {
                c->value(value);
            }
        }

        if ( bool whole_numbers = c->whole_numbers();
            ImGui::Checkbox("whole_numbers", &whole_numbers) )
        {
            c->whole_numbers(whole_numbers);
        }

        if ( slider::directions direction = c->direction();
            imgui_utils::show_enum_combo_box("direction", &direction) )
        {
            c->direction(direction);
        }

        ///TODO(BlackMat): add 'handle_style' inspector
    }
}
