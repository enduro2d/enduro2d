/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/sprite_asset.hpp>

#include <enduro2d/high/assets/json_asset.hpp>
#include <enduro2d/high/assets/atlas_asset.hpp>
#include <enduro2d/high/assets/texture_asset.hpp>

namespace
{
    using namespace e2d;

    class sprite_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "sprite asset loading exception";
        }
    };

    const char* sprite_asset_schema_source = R"json({
        "anyOf" : [{
            "type" : "object",
            "required" : [ "texture", "texrect" ],
            "additionalProperties" : false,
            "properties" : {
                "texture" : { "$ref": "#/common_definitions/address" },
                "texrect" : { "$ref": "#/common_definitions/b2" }
            }
        },{
            "type" : "object",
            "required" : [ "texture", "inner_texrect", "outer_texrect" ],
            "additionalProperties" : false,
            "properties" : {
                "texture" : { "$ref": "#/common_definitions/address" },
                "inner_texrect" : { "$ref": "#/common_definitions/b2" },
                "outer_texrect" : { "$ref": "#/common_definitions/b2" }
            }
        }]
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
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_ASSERT(root.HasMember("texture") && root["texture"].IsString());
        auto texture_p = library.load_asset_async<texture_asset>(
            path::combine(parent_address, root["texture"].GetString()));

        b2f inner_texrect;
        b2f outer_texrect;

        E2D_ASSERT(
            root.HasMember("texrect") ||
            (root.HasMember("inner_texrect") && root.HasMember("outer_texrect")));

        if ( root.HasMember("texrect") ) {
            b2f texrect;
            if ( !json_utils::try_parse_value(root["texrect"], texrect) ) {
                the<debug>().error("SPRITE: Incorrect formatting of 'texrect' property");
                return stdex::make_rejected_promise<sprite>(sprite_asset_loading_exception());
            }
            inner_texrect = texrect;
            outer_texrect = texrect;
        } else {
            if ( !json_utils::try_parse_value(root["inner_texrect"], inner_texrect) ) {
                the<debug>().error("SPRITE: Incorrect formatting of 'inner_texrect' property");
                return stdex::make_rejected_promise<sprite>(sprite_asset_loading_exception());
            }

            if ( !json_utils::try_parse_value(root["outer_texrect"], outer_texrect) ) {
                the<debug>().error("SPRITE: Incorrect formatting of 'outer_texrect' property");
                return stdex::make_rejected_promise<sprite>(sprite_asset_loading_exception());
            }
        }

        return texture_p.then([
            inner_texrect,
            outer_texrect
        ](const texture_asset::load_result& texture){
            sprite content;
            content.set_inner_texrect(inner_texrect);
            content.set_outer_texrect(outer_texrect);
            content.set_texture(texture);
            return content;
        });
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
            address = str(address),
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& sprite_data){
            return the<deferrer>().do_in_worker_thread([address, sprite_data](){
                const rapidjson::Document& doc = *sprite_data->content();
                rapidjson::SchemaValidator validator(sprite_asset_schema());

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

                throw sprite_asset_loading_exception();
            })
            .then([&library, parent_address, sprite_data](){
                return parse_sprite(
                    library, parent_address, *sprite_data->content());
            })
            .then([](auto&& content){
                return sprite_asset::create(
                    std::forward<decltype(content)>(content));
            });
        });
    }
}
