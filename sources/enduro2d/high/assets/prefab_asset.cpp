/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/prefab_asset.hpp>

#include "json_asset.hpp"

namespace
{
    using namespace e2d;

    class prefab_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "prefab asset loading exception";
        }
    };

    const char* prefab_asset_schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
        },
        "definitions" : {
        }
    })json";

    const rapidjson::SchemaDocument& prefab_asset_schema() {
        static std::mutex mutex;
        static std::unique_ptr<rapidjson::SchemaDocument> schema;

        std::lock_guard<std::mutex> guard(mutex);
        if ( !schema ) {
            rapidjson::Document doc;
            if ( doc.Parse(prefab_asset_schema_source).HasParseError() ) {
                the<debug>().error("ASSETS: Failed to parse prefab asset schema");
                throw prefab_asset_loading_exception();
            }
            json_utils::add_common_schema_definitions(doc);
            schema = std::make_unique<rapidjson::SchemaDocument>(doc);
        }

        return *schema;
    }

    stdex::promise<prefab> parse_prefab(
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_UNUSED(library, parent_address, root);
        return stdex::make_resolved_promise(prefab());
    }
}

namespace e2d
{
    prefab_asset::load_async_result prefab_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
        .then([
            &library,
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& prefab_data){
            return the<deferrer>().do_in_worker_thread([prefab_data](){
                const rapidjson::Document& doc = prefab_data->content();
                rapidjson::SchemaValidator validator(prefab_asset_schema());
                if ( !doc.Accept(validator) ) {
                    throw prefab_asset_loading_exception();
                }
            })
            .then([&library, parent_address, prefab_data](){
                return parse_prefab(
                    library, parent_address, prefab_data->content());
            })
            .then([](auto&& content){
                return prefab_asset::create(
                    std::forward<decltype(content)>(content));
            });
        });
    }
}
