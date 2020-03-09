/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/touchable.hpp>

namespace e2d
{
    const char* factory_loader<touchable>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "bubbling" : { "type" : "boolean" },
            "capturing" : { "type" : "boolean" }
        }
    })json";

    bool factory_loader<touchable>::operator()(
        touchable& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("bubbling") ) {
            bool bubbling = component.bubbling();
            if ( !json_utils::try_parse_value(ctx.root["bubbling"], bubbling) ) {
                the<debug>().error("TOUCHABLE: Incorrect formatting of 'bubbling' property");
                return false;
            }
            component.bubbling(bubbling);
        }

        if ( ctx.root.HasMember("capturing") ) {
            bool capturing = component.capturing();
            if ( !json_utils::try_parse_value(ctx.root["capturing"], capturing) ) {
                the<debug>().error("TOUCHABLE: Incorrect formatting of 'capturing' property");
                return false;
            }
            component.capturing(capturing);
        }

        return true;
    }

    bool factory_loader<touchable>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<touchable>::title = ICON_FA_FINGERPRINT " touchable";

    void component_inspector<touchable>::operator()(gcomponent<touchable>& c) const {
        if ( bool pressed = c.owner().component<touchable::pressed>().exists();
            ImGui::Checkbox("pressed", &pressed) )
        {
            if ( pressed ) {
                c.owner().component<touchable::pressed>().ensure();
            } else {
                c.owner().component<touchable::pressed>().remove();
            }
        }

        ImGui::SameLine();

        if ( bool released = c.owner().component<touchable::released>().exists();
            ImGui::Checkbox("released", &released) )
        {
            if ( released ) {
                c.owner().component<touchable::released>().ensure();
            } else {
                c.owner().component<touchable::released>().remove();
            }
        }

        ImGui::Separator();

        if ( bool hover_over = c.owner().component<touchable::hover_over>().exists();
            ImGui::Checkbox("hover_over", &hover_over) )
        {
            if ( hover_over ) {
                c.owner().component<touchable::hover_over>().ensure();
            } else {
                c.owner().component<touchable::hover_over>().remove();
            }
        }

        ImGui::SameLine();

        if ( bool hover_out = c.owner().component<touchable::hover_out>().exists();
            ImGui::Checkbox("hover_out", &hover_out) )
        {
            if ( hover_out ) {
                c.owner().component<touchable::hover_out>().ensure();
            } else {
                c.owner().component<touchable::hover_out>().remove();
            }
        }

        ImGui::Separator();

        if ( bool hover_enter = c.owner().component<touchable::hover_enter>().exists();
            ImGui::Checkbox("hover_enter", &hover_enter) )
        {
            if ( hover_enter ) {
                c.owner().component<touchable::hover_enter>().ensure();
            } else {
                c.owner().component<touchable::hover_enter>().remove();
            }
        }

        ImGui::SameLine();

        if ( bool hover_leave = c.owner().component<touchable::hover_leave>().exists();
            ImGui::Checkbox("hover_leave", &hover_leave) )
        {
            if ( hover_leave ) {
                c.owner().component<touchable::hover_leave>().ensure();
            } else {
                c.owner().component<touchable::hover_leave>().remove();
            }
        }

        ImGui::Separator();

        if ( bool bubbling = c->bubbling();
            ImGui::Checkbox("bubbling", &bubbling) )
        {
            c->bubbling(bubbling);
        }

        ImGui::SameLine();

        if ( bool capturing = c->capturing();
            ImGui::Checkbox("capturing", &capturing) )
        {
            c->capturing(capturing);
        }
    }
}
