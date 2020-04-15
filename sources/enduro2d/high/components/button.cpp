/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/button.hpp>

namespace e2d
{
    const char* factory_loader<button>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "style" : { "$ref": "#/common_definitions/address" }
        }
    })json";

    bool factory_loader<button>::operator()(
        button& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("style") ) {
            auto style = ctx.dependencies.find_asset<button_style_asset>(
                path::combine(ctx.parent_address, ctx.root["style"].GetString()));
            if ( !style ) {
                the<debug>().error("BUTTON: Dependency 'style' is not found:\n"
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

    bool factory_loader<button>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("style") ) {
            dependencies.add_dependency<button_style_asset>(
                path::combine(ctx.parent_address, ctx.root["style"].GetString()));
        }

        return true;
    }
}

namespace e2d
{
    const char* component_inspector<button>::title = ICON_FA_HAND_POINT_UP " button";

    void component_inspector<button>::operator()(gcomponent<button>& c) const {
        if ( bool pressed = c.component<button::pressed>().exists();
            ImGui::Checkbox("pressed", &pressed) )
        {
            if ( pressed ) {
                c.component<button::pressed>().ensure();
            } else {
                c.component<button::pressed>().remove();
            }
        }

        ///TODO(BlackMat): add 'style' inspector
    }
}
