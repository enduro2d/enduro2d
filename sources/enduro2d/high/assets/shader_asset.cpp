/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/shader_asset.hpp>

#include <enduro2d/high/assets/json_asset.hpp>
#include <enduro2d/high/assets/text_asset.hpp>

namespace
{
    using namespace e2d;

    class shader_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "shader asset loading exception";
        }
    };

    const char* shader_asset_schema_source = R"json(
        {
            "type" : "object",
            "required" : [ "vertex", "fragment" ],
            "additionalProperties" : false,
            "properties" : {
                "vertex" : { "$ref": "#/common_definitions/address" },
                "fragment" : { "$ref": "#/common_definitions/address" }
            }
        })json";

    const rapidjson::SchemaDocument& shader_asset_schema() {
        static std::mutex mutex;
        static std::unique_ptr<rapidjson::SchemaDocument> schema;

        std::lock_guard<std::mutex> guard(mutex);
        if ( !schema ) {
            rapidjson::Document doc;
            if ( doc.Parse(shader_asset_schema_source).HasParseError() ) {
                the<debug>().error("ASSETS: Failed to parse shader asset schema");
                throw shader_asset_loading_exception();
            }
            json_utils::add_common_schema_definitions(doc);
            schema = std::make_unique<rapidjson::SchemaDocument>(doc);
        }

        return *schema;
    }

    stdex::promise<shader_ptr> parse_shader(
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_ASSERT(root.HasMember("vertex") && root["vertex"].IsString());
        auto vertex_a = path::combine(parent_address, root["vertex"].GetString());
        auto vertex_p = library.load_asset_async<text_asset>(vertex_a);

        E2D_ASSERT(root.HasMember("fragment") && root["fragment"].IsString());
        auto fragment_a = path::combine(parent_address, root["fragment"].GetString());
        auto fragment_p = library.load_asset_async<text_asset>(fragment_a);

        return stdex::make_tuple_promise(std::make_tuple(
            std::move(vertex_p),
            std::move(fragment_p)))
        .then([
            vertex_a = std::move(vertex_a),
            fragment_a = std::move(fragment_a)
        ](const std::tuple<
            text_asset::load_result,
            text_asset::load_result
        >& results){
            return the<deferrer>().do_in_main_thread([
                results,
                vertex_a = std::move(vertex_a),
                fragment_a = std::move(fragment_a)
            ](){
                const shader_ptr content = the<render>().create_shader(
                    std::get<0>(results)->content(),
                    std::get<1>(results)->content());
                if ( !content ) {
                    throw shader_asset_loading_exception();
                }
                return content;
            });
        });
    }
}

namespace e2d
{
    shader_asset::load_async_result shader_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
        .then([
            &library,
            address = str(address),
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& shader_data){
            return the<deferrer>().do_in_worker_thread([address, shader_data](){
                const rapidjson::Document& doc = *shader_data->content();
                rapidjson::SchemaValidator validator(shader_asset_schema());

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

                throw shader_asset_loading_exception();
            })
            .then([&library, parent_address, shader_data](){
                return parse_shader(
                    library, parent_address, *shader_data->content());
            })
            .then([](auto&& content){
                return shader_asset::create(
                    std::forward<decltype(content)>(content));
            });
        });
    }
}
