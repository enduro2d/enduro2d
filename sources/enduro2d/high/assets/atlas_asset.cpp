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
            "texture" : { "$ref": "#/common_definitions/address" },
            "regions" : { "$ref": "#/definitions/regions" },
            "shape_regions" : { "$ref": "#/definitions/shape_regions" }
        },
        "definitions" : {
            "regions" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/region" }
            },
            "region" : {
                "type" : "object",
                "required" : [ "name", "pivot", "texrect" ],
                "additionalProperties" : false,
                "properties" : {
                    "name" : { "$ref": "#/common_definitions/name" },
                    "pivot" : { "$ref": "#/common_definitions/v2" },
                    "texrect" : { "$ref": "#/common_definitions/b2" }
                }
            },
            "shape_regions" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/shape_region" }
            },
            "shape_region" : {
                "type" : "object",
                "required" : [ "name", "pivot", "points" ],
                "additionalProperties" : false,
                "properties" : {
                    "name" : { "$ref": "#/common_definitions/name" },
                    "pivot" : { "$ref": "#/common_definitions/v2" },
                    "points" : {
                        "type" : "array",
                        "minItems" : 3,
                        "items" : { "$ref": "#/common_definitions/v2" }
                    }
                }
            }
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

    bool parse_regions(
        const rapidjson::Value& root,
        vector<atlas::region>& regions)
    {
        E2D_ASSERT(root.IsArray());
        vector<atlas::region> tregions(root.Size());

        for ( rapidjson::SizeType i = 0; i < root.Size(); ++i ) {
            E2D_ASSERT(root[i].IsObject());
            const auto& region_json = root[i];

            E2D_ASSERT(region_json.HasMember("name"));
            if ( !json_utils::try_parse_value(region_json["name"], tregions[i].name) ) {
                return false;
            }

            E2D_ASSERT(region_json.HasMember("pivot"));
            if ( !json_utils::try_parse_value(region_json["pivot"], tregions[i].pivot) ) {
                return false;
            }

            E2D_ASSERT(region_json.HasMember("texrect"));
            if ( !json_utils::try_parse_value(region_json["texrect"], tregions[i].texrect) ) {
                return false;
            }
        }

        regions = std::move(tregions);
        return true;
    }

    bool parse_shape_regions(
        const rapidjson::Value& root,
        vector<atlas::shape_region>& regions)
    {
        E2D_ASSERT(root.IsArray());
        vector<atlas::shape_region> tregions(root.Size());

        for ( rapidjson::SizeType i = 0; i < root.Size(); ++i ) {
            E2D_ASSERT(root[i].IsObject());
            const auto& region_json = root[i];

            E2D_ASSERT(region_json.HasMember("name"));
            if ( !json_utils::try_parse_value(region_json["name"], tregions[i].name) ) {
                return false;
            }

            E2D_ASSERT(region_json.HasMember("pivot"));
            if ( !json_utils::try_parse_value(region_json["pivot"], tregions[i].pivot) ) {
                return false;
            }

            E2D_ASSERT(region_json.HasMember("points"));
            if ( !json_utils::try_parse_values(region_json["points"], tregions[i].points) ) {
                return false;
            }
        }

        regions = std::move(tregions);
        return true;
    }

    stdex::promise<atlas> parse_atlas(
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_ASSERT(root.HasMember("texture") && root["texture"].IsString());
        auto texture_p = library.load_asset_async<texture_asset>(
            path::combine(parent_address, root["texture"].GetString()));

        vector<atlas::region> regions;
        if ( root.HasMember("regions") ) {
            const auto& regions_json = root["regions"];
            if ( !parse_regions(regions_json, regions) ) {
                return stdex::make_rejected_promise<atlas>(
                    atlas_asset_loading_exception());
            }
        }

        vector<atlas::shape_region> shape_regions;
        if ( root.HasMember("shape_regions") ) {
            const auto& shape_regions_json = root["shape_regions"];
            if ( !parse_shape_regions(shape_regions_json, shape_regions) ) {
                return stdex::make_rejected_promise<atlas>(
                    atlas_asset_loading_exception());
            }
        }

        return texture_p.then([
            regions = std::move(regions),
            shape_regions = std::move(shape_regions)
        ](const texture_asset::load_result& texture) mutable {
            atlas content;
            content.set_texture(texture);
            content.set_regions(std::move(regions));
            content.set_shape_regions(std::move(shape_regions));
            return content;
        });
    }
}

namespace e2d
{
    atlas_asset::load_async_result atlas_asset::load_async(
        const library& library, str_view address)
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
