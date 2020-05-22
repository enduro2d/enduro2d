/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/scroll.hpp>

namespace e2d
{
    const char* factory_loader<scroll>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "vertical" : { "type" : "boolean" },
            "horizontal" : { "type" : "boolean" },
            "inertia" : { "type" : "number" },
            "elasticity" : { "type" : "number" },
            "sensitivity" : { "type" : "number" }
        }
    })json";

    bool factory_loader<scroll>::operator()(
        scroll& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("vertical") ) {
            bool vertical = component.vertical();
            if ( !json_utils::try_parse_value(ctx.root["vertical"], vertical) ) {
                the<debug>().error("SCROLL: Incorrect formatting of 'vertical' property");
                return false;
            }
            component.vertical(vertical);
        }

        if ( ctx.root.HasMember("horizontal") ) {
            bool horizontal = component.horizontal();
            if ( !json_utils::try_parse_value(ctx.root["horizontal"], horizontal) ) {
                the<debug>().error("SCROLL: Incorrect formatting of 'horizontal' property");
                return false;
            }
            component.horizontal(horizontal);
        }

        if ( ctx.root.HasMember("inertia") ) {
            f32 inertia = component.inertia();
            if ( !json_utils::try_parse_value(ctx.root["inertia"], inertia) ) {
                the<debug>().error("SCROLL: Incorrect formatting of 'inertia' property");
                return false;
            }
            component.inertia(inertia);
        }

        if ( ctx.root.HasMember("elasticity") ) {
            f32 elasticity = component.elasticity();
            if ( !json_utils::try_parse_value(ctx.root["elasticity"], elasticity) ) {
                the<debug>().error("SCROLL: Incorrect formatting of 'elasticity' property");
                return false;
            }
            component.elasticity(elasticity);
        }

        if ( ctx.root.HasMember("sensitivity") ) {
            f32 sensitivity = component.sensitivity();
            if ( !json_utils::try_parse_value(ctx.root["sensitivity"], sensitivity) ) {
                the<debug>().error("SCROLL: Incorrect formatting of 'sensitivity' property");
                return false;
            }
            component.sensitivity(sensitivity);
        }

        return true;
    }

    bool factory_loader<scroll>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<scroll>::title = ICON_FA_SCROLL " scroll";

    void component_inspector<scroll>::operator()(gcomponent<scroll>& c) const {
        if ( bool vertical = c->vertical();
            ImGui::Checkbox("vertical", &vertical) )
        {
            c->vertical(vertical);
        }

        if ( bool horizontal = c->horizontal();
            ImGui::Checkbox("horizontal", &horizontal) )
        {
            c->horizontal(horizontal);
        }

        if ( f32 inertia = c->inertia();
            ImGui::DragFloat("inertia", &inertia, 0.01f, 0.f, 1.f) )
        {
            c->inertia(inertia);
        }

        if ( f32 elasticity = c->elasticity();
            ImGui::DragFloat("elasticity", &elasticity, 0.01f, 0.f, 1.f) )
        {
            c->elasticity(elasticity);
        }

        if ( f32 sensitivity = c->sensitivity();
            ImGui::DragFloat("sensitivity", &sensitivity, 0.01f, 0.f, std::numeric_limits<f32>::max()) )
        {
            c->sensitivity(sensitivity);
        }
    }
}
