/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/behaviour.hpp>

#include <enduro2d/high/assets/script_asset.hpp>

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

            //TODO(BlackMat): thread safe?
            sol::protected_function_result meta = script->content().call();

            if ( !meta.valid() ) {
                the<debug>().error("BEHAVIOUR: Behaviour script error:\n"
                    "--> Error: %0",
                    sol::error(meta).what());
                return false;
            }

            if ( meta.get_type() != sol::type::table ) {
                the<debug>().error("BEHAVIOUR: Behaviour script must return the meta table");
                return false;
            }

            component.meta(std::move(meta));
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
