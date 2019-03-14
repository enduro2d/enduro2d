/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets/sprite_asset.hpp>

#include "json_asset.hpp"

namespace
{
    using namespace e2d;

    class sprite_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "sprite asset loading exception";
        }
    };

    const char* sprite_asset_schema_source = R"json(
    {
        "type" : "object",
        "required" : [ "size", "pivot", "texrect" ],
        "additionalProperties" : false,
        "properties" : {
            "size" : { "$ref": "#/common_definitions/v2" },
            "pivot" : { "$ref": "#/common_definitions/v2" },
            "texrect" : { "$ref": "#/common_definitions/b2" },
            "texture" : { "$ref": "#/common_definitions/address" },
            "material" : { "$ref": "#/common_definitions/address" }
        }
    })json";

    const rapidjson::SchemaDocument& sprite_asset_schema() {
        static std::mutex mutex;
        static std::unique_ptr<rapidjson::SchemaDocument> schema;

        std::lock_guard<std::mutex> guard(mutex);
        if ( !schema ) {
            rapidjson::Document doc;
            if ( doc.Parse(sprite_asset_schema_source).HasParseError() ) {
                the<debug>().error("ASSETS: Failed to parse sprite asset schema");
                throw sprite_asset_loading_exception();
            }
            json_utils::add_common_schema_definitions(doc);
            schema = std::make_unique<rapidjson::SchemaDocument>(doc);
        }

        return *schema;
    }

    stdex::promise<sprite> parse_sprite(
        library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        v2f size;
        E2D_ASSERT(root.HasMember("size"));
        if ( !json_utils::try_parse_value(root["size"], size) ) {
            return stdex::make_rejected_promise<sprite>(
                sprite_asset_loading_exception());
        }

        v2f pivot;
        E2D_ASSERT(root.HasMember("pivot"));
        if ( !json_utils::try_parse_value(root["pivot"], pivot) ) {
            return stdex::make_rejected_promise<sprite>(
                sprite_asset_loading_exception());
        }

        b2f texrect;
        E2D_ASSERT(root.HasMember("texrect"));
        if ( !json_utils::try_parse_value(root["texrect"], texrect) ) {
            return stdex::make_rejected_promise<sprite>(
                sprite_asset_loading_exception());
        }

        E2D_ASSERT(!root.HasMember("texture") || root["texture"].IsString());
        auto texture_p = root.HasMember("texture")
            ? library.load_asset_async<texture_asset>(path::combine(
                parent_address,
                root["texture"].GetString()))
            : stdex::make_resolved_promise(texture_asset::ptr());

        E2D_ASSERT(!root.HasMember("material") || root["material"].IsString());
        auto material_p = root.HasMember("material")
            ? library.load_asset_async<material_asset>(path::combine(
                parent_address,
                root["material"].GetString()))
            : stdex::make_resolved_promise(material_asset::ptr());

        return stdex::make_tuple_promise(std::make_tuple(
            std::move(texture_p),
            std::move(material_p)
        )).then([size, pivot, texrect](const std::tuple<
            texture_asset::load_result,
            material_asset::load_result
        >& results){
            sprite content;
            content.set_size(size);
            content.set_pivot(pivot);
            content.set_texrect(texrect);
            content.set_texture(std::get<0>(results));
            content.set_material(std::get<1>(results));
            return content;
        });
    }
}

namespace e2d
{
    sprite_asset::load_async_result sprite_asset::load_async(
        library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
        .then([
            &library,
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& sprite_data){
            return the<deferrer>().do_in_worker_thread([sprite_data](){
                const rapidjson::Document& doc = sprite_data->content();
                rapidjson::SchemaValidator validator(sprite_asset_schema());
                if ( !doc.Accept(validator) ) {
                    throw sprite_asset_loading_exception();
                }
            })
            .then([&library, parent_address, sprite_data](){
                return parse_sprite(
                    library, parent_address, sprite_data->content());
            })
            .then([](auto&& content){
                return sprite_asset::create(
                    std::forward<decltype(content)>(content));
            });
        });
    }
}
