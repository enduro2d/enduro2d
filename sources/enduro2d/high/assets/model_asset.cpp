/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/model_asset.hpp>

#include <enduro2d/high/assets/json_asset.hpp>

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
            "mesh" : { "$ref": "#/common_definitions/address" }
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
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_ASSERT(root.HasMember("mesh") && root["mesh"].IsString());
        auto mesh_p = library.load_asset_async<mesh_asset>(
            path::combine(parent_address, root["mesh"].GetString()));

        return mesh_p.then([
        ](const mesh_asset::load_result& mesh){
            return the<deferrer>().do_in_main_thread([mesh](){
                model content;
                content.set_mesh(mesh);
                content.regenerate_geometry(the<render>());
                return content;
            });
        });
    }
}

namespace e2d
{
    model_asset::load_async_result model_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
        .then([
            &library,
            address = str(address),
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& model_data){
            return the<deferrer>().do_in_worker_thread([address, model_data](){
                const rapidjson::Document& doc = *model_data->content();
                rapidjson::SchemaValidator validator(model_asset_schema());

                if ( doc.Accept(validator) ) {
                    return;
                }

                rapidjson::StringBuffer sb;
                if ( validator.GetInvalidDocumentPointer().StringifyUriFragment(sb) ) {
                    the<debug>().error("ASSET: Failed to validate asset json:\n"
                        "--> Address: %0\n"
                        "--> Invalid schema keyword: %1\n"
                        "--> Invalid document pointer: %2",
                        address,
                        validator.GetInvalidSchemaKeyword(),
                        sb.GetString());
                } else {
                    the<debug>().error("ASSET: Failed to validate asset json");
                }

                throw model_asset_loading_exception();
            })
            .then([&library, parent_address, model_data](){
                return parse_model(
                    library, parent_address, *model_data->content());
            })
            .then([](auto&& content){
                return model_asset::create(
                    std::forward<decltype(content)>(content));
            });
        });
    }
}
