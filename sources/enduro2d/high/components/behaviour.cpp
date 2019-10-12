/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/behaviour.hpp>

namespace e2d
{
    const char* factory_loader<behaviour>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "script" : { "$ref": "#/common_definitions/address" }
        }
    })json";

    bool factory_loader<behaviour>::operator()(
        behaviour& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("script") ) {
            auto script = ctx.dependencies.find_asset<script_asset>(
                path::combine(ctx.parent_address, ctx.root["script"].GetString()));
            if ( !script ) {
                the<debug>().error("BEHAVIOUR: Dependency 'script' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["script"].GetString());
                return false;
            }
            component.script(script);
        }

        return true;
    }

    bool factory_loader<behaviour>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("script") ) {
            dependencies.add_dependency<script_asset>(
                path::combine(ctx.parent_address, ctx.root["script"].GetString()));
        }

        return true;
    }
}

namespace e2d
{
    const char* factory_loader<behaviour::started>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
    })json";

    bool factory_loader<behaviour::started>::operator()(
        behaviour::started& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<behaviour::started>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<behaviour::disabled>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
    })json";

    bool factory_loader<behaviour::disabled>::operator()(
        behaviour::disabled& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<behaviour::disabled>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}
