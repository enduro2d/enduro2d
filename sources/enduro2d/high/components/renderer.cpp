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
            "transform" : { "$ref": "#/common_definitions/t3" },
            "materials" : { "$ref": "#/definitions/materials" }
        },
        "definitions" : {
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
        if ( ctx.root.HasMember("transform") ) {
            t3f transform = component.transform();
            if ( !json_utils::try_parse_value(ctx.root["transform"], transform) ) {
                the<debug>().error("RENDERER: Incorrect formatting of 'transform' property");
                return false;
            }
            component.transform(transform);
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

namespace e2d
{
    const char* component_inspector<renderer>::title = "renderer";

    void component_inspector<renderer>::operator()(gcomponent<renderer>& c) const {
        if ( v3f translation = c->translation();
            ImGui::DragFloat3("translation", translation.data(), 1.f) )
        {
            c->translation(translation);
        }

        if ( v3f rotation = c->rotation() * math::rad_to_deg<f32>();
            ImGui::DragFloat3("rotation", rotation.data(), 0.1f) )
        {
            c->rotation(rotation * math::deg_to_rad<f32>());
        }

        if ( v3f scale = c->scale();
            ImGui::DragFloat3("scale", scale.data(), 0.01f) )
        {
            c->scale(scale);
        }

        ///TODO(BlackMat): add 'properties' inspector
        ///TODO(BlackMat): add 'materials' inspector
    }
}
