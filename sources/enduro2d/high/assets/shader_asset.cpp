/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets/assets.hpp>

#include "json_asset.hpp"

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
                "vertex" : { "$ref": "#/definitions/generic_address" },
                "fragment" : { "$ref": "#/definitions/generic_address" }
            },
            "definitions" : {
                "generic_address" : {
                    "type" : "string",
                    "minLength" : 1
                }
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
            schema = std::make_unique<rapidjson::SchemaDocument>(doc);
        }

        return *schema;
    }

    stdex::promise<shader_ptr> parse_shader(
        library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_ASSERT(root.HasMember("vertex") && root["vertex"].IsString());
        auto vertex_p = library.load_asset_async<text_asset>(
            path::combine(parent_address, root["vertex"].GetString()));

        E2D_ASSERT(root.HasMember("fragment") && root["fragment"].IsString());
        auto fragment_p = library.load_asset_async<text_asset>(
            path::combine(parent_address, root["fragment"].GetString()));

        return stdex::make_tuple_promise(std::make_tuple(
            std::move(vertex_p),
            std::move(fragment_p)))
        .then([](const std::tuple<
            text_asset::ptr,
            text_asset::ptr
        >& results){
            if ( !modules::is_initialized<deferrer>() ) {
                throw shader_asset_loading_exception();
            }
            return the<deferrer>().do_in_main_thread([results](){
                if ( !modules::is_initialized<render>() ) {
                    throw shader_asset_loading_exception();
                }
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
        library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
            .then([
                &library,
                parent_address = path::parent_path(address)
            ](const json_asset::ptr& shader_data){
                if ( !modules::is_initialized<deferrer>() ) {
                    throw shader_asset_loading_exception();
                }
                return the<deferrer>().do_in_worker_thread([shader_data](){
                    const rapidjson::Document& doc = shader_data->content();
                    rapidjson::SchemaValidator validator(shader_asset_schema());
                    if ( !doc.Accept(validator) ) {
                        throw shader_asset_loading_exception();
                    }
                })
                .then([&library, parent_address, shader_data](){
                    return parse_shader(
                        library, parent_address, shader_data->content());
                })
                .then([](const shader_ptr& shader){
                    if ( !shader ) {
                        throw shader_asset_loading_exception();
                    }
                    return std::make_shared<shader_asset>(shader);
                });
            });
    }
}
