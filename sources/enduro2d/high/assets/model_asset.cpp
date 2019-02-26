/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets/model_asset.hpp>

#include "json_asset.hpp"

namespace
{
    using namespace e2d;

    class model_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "model asset loading exception";
        }
    };

    const char* model_asset_schema_source = R"json(
    {
        "type" : "object",
        "required" : [ "mesh" ],
        "additionalProperties" : false,
        "properties" : {
            "mesh" : { "$ref": "#/common_definitions/address" },
            "materials" : {
                "type" : "array",
                "items" : { "$ref" : "#/common_definitions/address" }
            }
        }
    })json";

    const rapidjson::SchemaDocument& model_asset_schema() {
        static std::mutex mutex;
        static std::unique_ptr<rapidjson::SchemaDocument> schema;

        std::lock_guard<std::mutex> guard(mutex);
        if ( !schema ) {
            rapidjson::Document doc;
            if ( doc.Parse(model_asset_schema_source).HasParseError() ) {
                the<debug>().error("ASSETS: Failed to parse model asset schema");
                throw model_asset_loading_exception();
            }
            json_utils::add_common_schema_definitions(doc);
            schema = std::make_unique<rapidjson::SchemaDocument>(doc);
        }

        return *schema;
    }

    stdex::promise<model> parse_model(
        library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_ASSERT(root.HasMember("mesh") && root["mesh"].IsString());
        auto mesh_p = library.load_asset_async<mesh_asset>(
            path::combine(parent_address, root["mesh"].GetString()));

        vector<stdex::promise<material_asset::load_result>> materials_p;
        if ( root.HasMember("materials") ) {
            E2D_ASSERT(root["materials"].IsArray());
            const auto& materials_json = root["materials"];

            materials_p.reserve(materials_json.Size());
            for ( rapidjson::SizeType i = 0; i < materials_json.Size(); ++i ) {
                E2D_ASSERT(materials_json[i].IsString());
                materials_p.emplace_back(
                    library.load_asset_async<material_asset>(
                        path::combine(parent_address, materials_json[i].GetString())));
            }
        }

        return stdex::make_tuple_promise(std::make_tuple(
            std::move(mesh_p),
            stdex::make_all_promise(materials_p)))
        .then([](const std::tuple<
            mesh_asset::load_result,
            vector<material_asset::load_result>
        >& results){
            model content;
            content.set_mesh(std::get<0>(results));
            content.set_materials(std::get<1>(results));
            return content;
        });
    }
}

namespace e2d
{
    model_asset::load_async_result model_asset::load_async(
        library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
        .then([
            &library,
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& model_data){
            if ( !modules::is_initialized<deferrer>() ) {
                throw model_asset_loading_exception();
            }
            return the<deferrer>().do_in_worker_thread([model_data](){
                const rapidjson::Document& doc = model_data->content();
                rapidjson::SchemaValidator validator(model_asset_schema());
                if ( !doc.Accept(validator) ) {
                    throw model_asset_loading_exception();
                }
            })
            .then([&library, parent_address, model_data](){
                return parse_model(
                    library, parent_address, model_data->content());
            })
            .then([](const model& model){
                return model_asset::create(model);
            });
        });
    }
}
