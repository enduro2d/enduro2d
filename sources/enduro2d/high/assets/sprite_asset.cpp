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

    const char* sprite_asset_schema_source = R"json({
        "type" : "object",
        "oneOf" : [{
            "required" : [ "atlas", "material" ],
            "additionalProperties" : false,
            "properties" : {
                "atlas" : { "$ref": "#/definitions/atlas" },
                "material" : { "$ref": "#/common_definitions/address" }
            }
        }, {
            "required" : [ "texture", "material" ],
            "additionalProperties" : false,
            "properties" : {
                "texture" : { "$ref": "#/definitions/texture" },
                "material" : { "$ref": "#/common_definitions/address" }
            }
        }],
        "definitions" : {
            "atlas" : {
                "type" : "object",
                "required" : [ "atlas", "region" ],
                "additionalProperties" : false,
                "properties" : {
                    "atlas" : { "$ref": "#/common_definitions/address" },
                    "region" : { "$ref": "#/common_definitions/name" }
                }
            },
            "texture" : {
                "type" : "object",
                "required" : [ "texture", "pivot", "texrect" ],
                "additionalProperties" : false,
                "properties" : {
                    "texture" : { "$ref": "#/common_definitions/address" },
                    "pivot" : { "$ref": "#/common_definitions/v2" },
                    "texrect" : { "$ref": "#/common_definitions/b2" }
                }
            }
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

    stdex::promise<sprite> parse_sprite_with_atlas(
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_ASSERT(root.HasMember("material") && root["material"].IsString());
        auto material_p = library.load_asset_async<material_asset>(
            path::combine(parent_address, root["material"].GetString()));

        E2D_ASSERT(root.HasMember("atlas") && root["atlas"].IsObject());
        const auto& atlas_root_json = root["atlas"];

        E2D_ASSERT(atlas_root_json.HasMember("atlas") && atlas_root_json["atlas"].IsString());
        auto atlas_p = library.load_asset_async<atlas_asset>(
            path::combine(parent_address, atlas_root_json["atlas"].GetString()));

        str_hash region_hash;
        if ( !json_utils::try_parse_value(atlas_root_json["region"], region_hash) ) {
            return stdex::make_rejected_promise<sprite>(
                sprite_asset_loading_exception());
        }

        return stdex::make_tuple_promise(std::make_tuple(
            std::move(atlas_p),
            std::move(material_p)
        )).then([region_hash](const std::tuple<
            atlas_asset::load_result,
            material_asset::load_result
        >& results){
            const atlas_asset::load_result& atlas = std::get<0>(results);
            const material_asset::load_result& material = std::get<1>(results);

            if ( !atlas || !material ) {
                throw sprite_asset_loading_exception();
            }

            const texture_asset::ptr& texture = atlas->content().texture();
            const atlas::region* region = atlas->content().find_region(region_hash);

            if ( !texture || !region ) {
                throw sprite_asset_loading_exception();
            }

            sprite content;
            content.set_region(*region);
            content.set_texture(texture);
            content.set_material(material);
            return content;
        });
    }

    stdex::promise<sprite> parse_sprite_with_texture(
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_ASSERT(root.HasMember("material") && root["material"].IsString());
        auto material_p = library.load_asset_async<material_asset>(
            path::combine(parent_address, root["material"].GetString()));

        E2D_ASSERT(root.HasMember("texture") && root["texture"].IsObject());
        const auto& texture_root_json = root["texture"];

        E2D_ASSERT(texture_root_json.HasMember("texture") && texture_root_json["texture"].IsString());
        auto texture_p = library.load_asset_async<texture_asset>(
            path::combine(parent_address, texture_root_json["texture"].GetString()));

        v2f pivot;
        E2D_ASSERT(texture_root_json.HasMember("pivot"));
        if ( !json_utils::try_parse_value(texture_root_json["pivot"], pivot) ) {
            return stdex::make_rejected_promise<sprite>(
                sprite_asset_loading_exception());
        }

        b2f texrect;
        E2D_ASSERT(texture_root_json.HasMember("texrect"));
        if ( !json_utils::try_parse_value(texture_root_json["texrect"], texrect) ) {
            return stdex::make_rejected_promise<sprite>(
                sprite_asset_loading_exception());
        }

        return stdex::make_tuple_promise(std::make_tuple(
            std::move(texture_p),
            std::move(material_p)
        )).then([pivot, texrect](const std::tuple<
            texture_asset::load_result,
            material_asset::load_result
        >& results){
            const texture_asset::load_result& texture = std::get<0>(results);
            const material_asset::load_result& material = std::get<1>(results);

            if ( !texture || !material ) {
                throw sprite_asset_loading_exception();
            }

            sprite content;
            content.set_pivot(pivot);
            content.set_texrect(texrect);
            content.set_texture(texture);
            content.set_material(material);
            return content;
        });
    }

    stdex::promise<sprite> parse_sprite(
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        if ( root.HasMember("atlas") ) {
            return parse_sprite_with_atlas(library, parent_address, root);
        }

        if ( root.HasMember("texture") ) {
            return parse_sprite_with_texture(library, parent_address, root);
        }

        return stdex::make_rejected_promise<sprite>(
            sprite_asset_loading_exception());
    }
}

namespace e2d
{
    sprite_asset::load_async_result sprite_asset::load_async(
        const library& library, str_view address)
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
