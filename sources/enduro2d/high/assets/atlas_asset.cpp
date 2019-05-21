/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/atlas_asset.hpp>

#include <enduro2d/high/assets/json_asset.hpp>
#include <enduro2d/high/assets/sprite_asset.hpp>
#include <enduro2d/high/assets/texture_asset.hpp>

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
            "sprites" : { "$ref": "#/definitions/sprites" }
        },
        "definitions" : {
            "sprites" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/sprite" }
            },
            "sprite" : {
                "type" : "object",
                "required" : [ "name", "pivot", "texrect" ],
                "additionalProperties" : false,
                "properties" : {
                    "name" : { "$ref": "#/common_definitions/name" },
                    "pivot" : { "$ref": "#/common_definitions/v2" },
                    "texrect" : { "$ref": "#/common_definitions/b2" }
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

    struct sprite_desc {
        str_hash name;
        v2f pivot;
        b2f texrect;
    };

    bool parse_sprites(
        const rapidjson::Value& root,
        vector<sprite_desc>& sprite_descs)
    {
        E2D_ASSERT(root.IsArray());
        vector<sprite_desc> tsprite_descs(root.Size());

        for ( rapidjson::SizeType i = 0; i < root.Size(); ++i ) {
            E2D_ASSERT(root[i].IsObject());
            const auto& sprite_json = root[i];

            E2D_ASSERT(sprite_json.HasMember("name"));
            if ( !json_utils::try_parse_value(sprite_json["name"], tsprite_descs[i].name) ) {
                return false;
            }

            E2D_ASSERT(sprite_json.HasMember("pivot"));
            if ( !json_utils::try_parse_value(sprite_json["pivot"], tsprite_descs[i].pivot) ) {
                return false;
            }

            E2D_ASSERT(sprite_json.HasMember("texrect"));
            if ( !json_utils::try_parse_value(sprite_json["texrect"], tsprite_descs[i].texrect) ) {
                return false;
            }
        }

        sprite_descs = std::move(tsprite_descs);
        return true;
    }

    using parse_atlas_result = std::tuple<atlas, nested_content>;
    stdex::promise<parse_atlas_result> parse_atlas(
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_ASSERT(root.HasMember("texture") && root["texture"].IsString());
        auto texture_p = library.load_asset_async<texture_asset>(
            path::combine(parent_address, root["texture"].GetString()));

        vector<sprite_desc> sprite_descs;
        if ( root.HasMember("sprites") ) {
            const auto& sprites_json = root["sprites"];
            if ( !parse_sprites(sprites_json, sprite_descs) ) {
                return stdex::make_rejected_promise<parse_atlas_result>(
                    atlas_asset_loading_exception());
            }
        }

        return texture_p.then([
            sprite_descs = std::move(sprite_descs)
        ](const texture_asset::load_result& texture) mutable {
            atlas content;
            content.set_texture(texture);

            nested_content ncontent;
            for ( const sprite_desc& desc : sprite_descs ) {
                sprite spr;
                spr.set_pivot(desc.pivot);
                spr.set_texrect(desc.texrect);
                spr.set_texture(texture);
                ncontent.insert(std::make_pair(desc.name, sprite_asset::create(std::move(spr))));
            }

            return std::make_tuple(std::move(content), std::move(ncontent));
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
            address = str(address),
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& atlas_data){
            return the<deferrer>().do_in_worker_thread([address, atlas_data](){
                const rapidjson::Document& doc = *atlas_data->content();
                rapidjson::SchemaValidator validator(atlas_asset_schema());

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

                throw atlas_asset_loading_exception();
            })
            .then([&library, parent_address, atlas_data](){
                return parse_atlas(
                    library, parent_address, *atlas_data->content());
            })
            .then([](const parse_atlas_result& result){
                return atlas_asset::create(
                    std::get<0>(result),
                    std::get<1>(result));
            });
        });
    }
}
