/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "renderer.hpp"

namespace e2d
{
    template <>
    class component_loader<renderer> {
    public:
        constexpr static const char*
        schema_source = R"json({
            "type" : "object",
            "required" : [],
            "additionalProperties" : false,
            "properties" : {
                "enabled" : { "type" : "boolean" },
                "materials" : {
                    "type" : "array",
                    "items" : { "$ref": "#/common_definitions/address" }
                }
            }
        })json";

        bool operator()(
            str_view parent_address,
            const rapidjson::Value& root,
            asset_dependencies& dependencies) const
        {
            if ( root.HasMember("materials") ) {
                const rapidjson::Value& materials_root = root["materials"];
                for ( rapidjson::SizeType i = 0; i < materials_root.Size(); ++i ) {
                    dependencies.add_dependency<material_asset>(
                        path::combine(parent_address, materials_root[i].GetString()));
                }
            }

            return true;
        }

        bool operator()(
            str_view parent_address,
            const rapidjson::Value& root,
            const asset_group& dependencies,
            renderer& component) const
        {
            if ( root.HasMember("enabled") ) {
                auto enabled = component.enabled();
                if ( !json_utils::try_parse_value(root["enabled"], enabled) ) {
                    the<debug>().error("FLIPBOOK_PLAYER: Incorrect formatting of 'enabled' property");
                    return false;
                }
                component.enabled(enabled);
            }

            if ( root.HasMember("properties") ) {
                //TODO(BlackMat): add properties parsing
            }

            if ( root.HasMember("materials") ) {
                const rapidjson::Value& materials_root = root["materials"];
                vector<material_asset::ptr> materials(materials_root.Size());
                for ( rapidjson::SizeType i = 0; i < materials_root.Size(); ++i ) {
                    auto material = dependencies.find_asset<material_asset>(
                        path::combine(parent_address, materials_root[i].GetString()));
                    if ( !material ) {
                        the<debug>().error("RENDERER: Dependency 'material' is not found:\n"
                            "--> Parent address: %0\n"
                            "--> Dependency address: %1",
                            parent_address,
                            materials_root[i].GetString());
                        return false;
                    }
                    materials[i] = material;
                }
                component.materials(std::move(materials));
            }

            return true;
        }
    };
}
