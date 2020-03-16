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
            "style" : { "$ref": "#/common_definitions/address" }
        }
    })json";

    bool factory_loader<slider>::operator()(
        slider& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("style") ) {
            auto style = ctx.dependencies.find_asset<button_style_asset>(
                path::combine(ctx.parent_address, ctx.root["style"].GetString()));
            if ( !style ) {
                the<debug>().error("SLIDER: Dependency 'style' is not found:\n"
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

    bool factory_loader<slider>::operator()(
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
    const char* component_inspector<slider>::title = ICON_FA_SLIDERS_H " slider";

    void component_inspector<slider>::operator()(gcomponent<slider>& c) const {
        E2D_UNUSED(c);
        ///TODO(BlackMat): add 'style' inspector
    }
}
