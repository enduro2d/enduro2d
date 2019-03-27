/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets/flipbook_asset.hpp>

#include "json_asset.hpp"

namespace
{
    using namespace e2d;

    class flipbook_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "flipbook asset loading exception";
        }
    };

    const char* flipbook_asset_schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
        "definitions" : {}
    })json";

    const rapidjson::SchemaDocument& flipbook_asset_schema() {
        static std::mutex mutex;
        static std::unique_ptr<rapidjson::SchemaDocument> schema;

        std::lock_guard<std::mutex> guard(mutex);
        if ( !schema ) {
            rapidjson::Document doc;
            if ( doc.Parse(flipbook_asset_schema_source).HasParseError() ) {
                the<debug>().error("ASSETS: Failed to parse flipbook asset schema");
                throw flipbook_asset_loading_exception();
            }
            json_utils::add_common_schema_definitions(doc);
            schema = std::make_unique<rapidjson::SchemaDocument>(doc);
        }

        return *schema;
    }

    stdex::promise<flipbook> parse_flipbook(
        library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_UNUSED(library, parent_address, root);
        return stdex::make_rejected_promise<flipbook>(
            flipbook_asset_loading_exception());
    }
}

namespace e2d
{
    flipbook_asset::load_async_result flipbook_asset::load_async(
        library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
        .then([
            &library,
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& flipbook_data){
            return the<deferrer>().do_in_worker_thread([flipbook_data](){
                const rapidjson::Document& doc = flipbook_data->content();
                rapidjson::SchemaValidator validator(flipbook_asset_schema());
                if ( !doc.Accept(validator) ) {
                    throw flipbook_asset_loading_exception();
                }
            })
            .then([&library, parent_address, flipbook_data](){
                return parse_flipbook(
                    library, parent_address, flipbook_data->content());
            })
            .then([](auto&& content){
                return flipbook_asset::create(
                    std::forward<decltype(content)>(content));
            });
        });
    }
}
