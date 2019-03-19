/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets/atlas_asset.hpp>

#include "json_asset.hpp"

namespace
{
    using namespace e2d;

    class atlas_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "atlas asset loading exception";
        }
    };

    const char* atlas_asset_schema_source = R"json(
    {
        "type" : "object",
        "required" : [ "texture" ],
        "additionalProperties" : false,
        "properties" : {
            "texture" : { "$ref": "#/common_definitions/address" }
        }
    })json";

    const rapidjson::SchemaDocument& atlas_asset_schema() {
        static std::mutex mutex;
        static std::unique_ptr<rapidjson::SchemaDocument> schema;

        std::lock_guard<std::mutex> guard(mutex);
        if ( !schema ) {
            rapidjson::Document doc;
            if ( doc.Parse(atlas_asset_schema_source).HasParseError() ) {
                the<debug>().error("ASSETS: Failed to parse atlas asset schema");
                throw atlas_asset_loading_exception();
            }
            json_utils::add_common_schema_definitions(doc);
            schema = std::make_unique<rapidjson::SchemaDocument>(doc);
        }

        return *schema;
    }

    stdex::promise<atlas> parse_atlas(
        library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_UNUSED(library, parent_address, root);
        return stdex::make_resolved_promise<atlas>(atlas());
    }
}

namespace e2d
{
    atlas_asset::load_async_result atlas_asset::load_async(
        library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
        .then([
            &library,
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& atlas_data){
            return the<deferrer>().do_in_worker_thread([atlas_data](){
                const rapidjson::Document& doc = atlas_data->content();
                rapidjson::SchemaValidator validator(atlas_asset_schema());
                if ( !doc.Accept(validator) ) {
                    throw atlas_asset_loading_exception();
                }
            })
            .then([&library, parent_address, atlas_data](){
                return parse_atlas(
                    library, parent_address, atlas_data->content());
            })
            .then([](auto&& content){
                return atlas_asset::create(
                    std::forward<decltype(content)>(content));
            });
        });
    }
}
