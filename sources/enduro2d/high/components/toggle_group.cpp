/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/toggle_group.hpp>

namespace e2d
{
    const char* factory_loader<toggle_group>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "allow_switch_off" : { "type" : "boolean" },
            "allow_multiple_on" : { "type" : "boolean" }
        }
    })json";

    bool factory_loader<toggle_group>::operator()(
        toggle_group& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("allow_switch_off") ) {
            bool allow_switch_off = component.allow_switch_off();
            if ( !json_utils::try_parse_value(ctx.root["allow_switch_off"], allow_switch_off) ) {
                the<debug>().error("TOGGLE_GROUP: Incorrect formatting of 'allow_switch_off' property");
                return false;
            }
            component.allow_switch_off(allow_switch_off);
        }

        if ( ctx.root.HasMember("allow_multiple_on") ) {
            bool allow_multiple_on = component.allow_multiple_on();
            if ( !json_utils::try_parse_value(ctx.root["allow_multiple_on"], allow_multiple_on) ) {
                the<debug>().error("TOGGLE_GROUP: Incorrect formatting of 'allow_multiple_on' property");
                return false;
            }
            component.allow_multiple_on(allow_multiple_on);
        }

        return true;
    }

    bool factory_loader<toggle_group>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<toggle_group>::title = ICON_FA_TOGGLE_OFF " toggle_group";

    void component_inspector<toggle_group>::operator()(gcomponent<toggle_group>& c) const {
        if ( bool allow_switch_off = c->allow_switch_off();
            ImGui::Checkbox("allow_switch_off", &allow_switch_off) )
        {
            c->allow_switch_off(allow_switch_off);
        }

        if ( bool allow_multiple_on = c->allow_multiple_on();
            ImGui::Checkbox("allow_multiple_on", &allow_multiple_on) )
        {
            c->allow_multiple_on(allow_multiple_on);
        }
    }
}
