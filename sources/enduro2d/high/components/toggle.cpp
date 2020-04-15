/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/toggle.hpp>

namespace e2d
{
    const char* factory_loader<toggle>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "style" : { "$ref": "#/common_definitions/address" }
        }
    })json";

    bool factory_loader<toggle>::operator()(
        toggle& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("style") ) {
            auto style = ctx.dependencies.find_asset<toggle_style_asset>(
                path::combine(ctx.parent_address, ctx.root["style"].GetString()));
            if ( !style ) {
                the<debug>().error("TOGGLE: Dependency 'style' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["style"].GetString());
                return false;
            }
            component.style(style);
        }

        return true;
    }

    bool factory_loader<toggle>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("style") ) {
            dependencies.add_dependency<toggle_style_asset>(
                path::combine(ctx.parent_address, ctx.root["style"].GetString()));
        }

        return true;
    }
}

namespace e2d
{
    const char* component_inspector<toggle>::title = ICON_FA_TOGGLE_ON " toggle";

    void component_inspector<toggle>::operator()(gcomponent<toggle>& c) const {
        if ( bool pressed = c.component<toggle::pressed>().exists();
            ImGui::Checkbox("pressed", &pressed) )
        {
            if ( pressed ) {
                c.component<toggle::pressed>().ensure();
            } else {
                c.component<toggle::pressed>().remove();
            }
        }

        ///TODO(BlackMat): add 'style' inspector
    }
}
