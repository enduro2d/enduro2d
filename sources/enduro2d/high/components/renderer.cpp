/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/renderer.hpp>

namespace e2d
{
    const char* factory_loader<renderer>::schema_source = R"json({
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

    bool factory_loader<renderer>::operator()(
        renderer& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("enabled") ) {
            auto enabled = component.enabled();
            if ( !json_utils::try_parse_value(ctx.root["enabled"], enabled) ) {
                the<debug>().error("FLIPBOOK_PLAYER: Incorrect formatting of 'enabled' property");
                return false;
            }
            component.enabled(enabled);
        }

        if ( ctx.root.HasMember("properties") ) {
            //TODO(BlackMat): add properties parsing
        }

        if ( ctx.root.HasMember("materials") ) {
            const rapidjson::Value& materials_root = ctx.root["materials"];
            vector<material_asset::ptr> materials(materials_root.Size());
            for ( rapidjson::SizeType i = 0; i < materials_root.Size(); ++i ) {
                auto material = ctx.dependencies.find_asset<material_asset>(
                    path::combine(ctx.parent_address, materials_root[i].GetString()));
                if ( !material ) {
                    the<debug>().error("RENDERER: Dependency 'material' is not found:\n"
                        "--> Parent address: %0\n"
                        "--> Dependency address: %1",
                        ctx.parent_address,
                        materials_root[i].GetString());
                    return false;
                }
                materials[i] = material;
            }
            component.materials(std::move(materials));
        }

        return true;
    }

    bool factory_loader<renderer>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("materials") ) {
            const rapidjson::Value& materials_root = ctx.root["materials"];
            for ( rapidjson::SizeType i = 0; i < materials_root.Size(); ++i ) {
                dependencies.add_dependency<material_asset>(
                    path::combine(ctx.parent_address, materials_root[i].GetString()));
            }
        }

        return true;
    }
}
