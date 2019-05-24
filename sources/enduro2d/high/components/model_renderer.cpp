/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/model_renderer.hpp>

namespace e2d
{
    const char* factory_loader<model_renderer>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "model" : { "$ref": "#/common_definitions/address" }
        }
    })json";

    bool factory_loader<model_renderer>::operator()(
        model_renderer& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("model") ) {
            auto model = ctx.dependencies.find_asset<model_asset>(
                path::combine(ctx.parent_address, ctx.root["model"].GetString()));
            if ( !model ) {
                the<debug>().error("MODEL_RENDERER: Dependency 'model' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["model"].GetString());
                return false;
            }
            component.model(model);
        }

        return true;;
    }

    bool factory_loader<model_renderer>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("model") ) {
            dependencies.add_dependency<model_asset>(
                path::combine(ctx.parent_address, ctx.root["model"].GetString()));
        }

        return true;
    }
}
