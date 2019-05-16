/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "model_renderer.hpp"

namespace e2d
{
    template <>
    class component_loader<model_renderer> {
    public:
        constexpr static const char*
        schema_source = R"json({
            "type" : "object",
            "required" : [],
            "additionalProperties" : false,
            "properties" : {
                "model" : { "$ref": "#/common_definitions/address" }
            }
        })json";

        bool operator()(
            str_view parent_address,
            const rapidjson::Value& root,
            asset_dependencies& dependencies) const
        {
            if ( root.HasMember("model") ) {
                dependencies.add_dependency<model_asset>(
                    path::combine(parent_address, root["model"].GetString()));
            }

            return true;
        }

        bool operator()(
            str_view parent_address,
            const rapidjson::Value& root,
            const asset_group& dependencies,
            model_renderer& component) const
        {
            if ( root.HasMember("model") ) {
                auto model = dependencies.find_asset<model_asset>(
                    path::combine(parent_address, root["model"].GetString()));
                if ( !model ) {
                    the<debug>().error("MODEL_RENDERER: Dependency 'model' is not found:\n"
                        "--> Parent address: %0\n"
                        "--> Dependency address: %1",
                        parent_address,
                        root["model"].GetString());
                    return false;
                }
                component.model(model);
            }

            return true;;
        }
    };
}
