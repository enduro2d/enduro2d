/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/flipbook_asset.hpp>

#include "json_asset.hpp"
#include <enduro2d/high/assets/atlas_asset.hpp>
#include <enduro2d/high/assets/sprite_asset.hpp>

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
        "properties" : {
            "frames" : { "$ref": "#/definitions/frames" },
            "sequences" : { "$ref": "#/definitions/sequences" }
        },
        "definitions" : {
            "frames" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/frame" }
            },
            "frame" : {
                "type" : "object",
                "oneOf" : [{
                    "required" : [ "atlas" ],
                    "additionalProperties" : false,
                    "properties" : {
                        "atlas" : { "$ref": "#/common_definitions/address" }
                    }
                }, {
                    "required" : [ "sprite" ],
                    "additionalProperties" : false,
                    "properties" : {
                        "sprite" : { "$ref": "#/common_definitions/address" }
                    }
                }]
            },
            "sequences" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/sequence" }
            },
            "sequence" : {
                "type" : "object",
                "required" : [ "fps", "name" ],
                "additionalProperties" : false,
                "properties" : {
                    "fps" : { "type" : "number" },
                    "name" : { "$ref": "#/common_definitions/name" },
                    "frames" : {
                        "type" : "array",
                        "items" : { "type" : "number" }
                    }
                }
            }
        }
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

    stdex::promise<flipbook::frame> parse_flipbook_frame(
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        if ( root.HasMember("atlas") ) {
            E2D_ASSERT(root["atlas"].IsString());
            return library.load_asset_async<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["atlas"].GetString()))
            .then([](const sprite_asset::load_result& sprite){
                flipbook::frame frame;
                frame.sprite = sprite;
                return frame;
            });
        }

        if ( root.HasMember("sprite") ) {
            E2D_ASSERT(root["sprite"].IsString());
            return library.load_asset_async<sprite_asset>(
                path::combine(parent_address, root["sprite"].GetString()))
            .then([](const sprite_asset::load_result& sprite){
                flipbook::frame frame;
                frame.sprite = sprite;
                return frame;
            });
        }

        return stdex::make_rejected_promise<flipbook::frame>(
            flipbook_asset_loading_exception());
    }

    stdex::promise<vector<flipbook::frame>> parse_flipbook_frames(
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        E2D_ASSERT(root.IsArray());
        vector<stdex::promise<flipbook::frame>> frames_p;
        frames_p.reserve(root.Size());
        for ( rapidjson::SizeType i = 0; i < root.Size(); ++i ) {
            frames_p.push_back(
                parse_flipbook_frame(library, parent_address, root[i]));
        }
        return stdex::make_all_promise(frames_p);
    }

    stdex::promise<vector<flipbook::sequence>> parse_flipbook_sequences(
        const rapidjson::Value& root)
    {
        E2D_ASSERT(root.IsArray());

        vector<flipbook::sequence> sequences;
        sequences.reserve(root.Size());
        for ( rapidjson::SizeType i = 0; i < root.Size(); ++i ) {
            const auto& sequence_json = root[i];
            E2D_ASSERT(sequence_json.IsObject());

            f32 fps{0.f};
            if ( !json_utils::try_parse_value(sequence_json["fps"], fps) ) {
                return stdex::make_rejected_promise<vector<flipbook::sequence>>(
                    flipbook_asset_loading_exception());
            }

            str_hash name_hash;
            if ( !json_utils::try_parse_value(sequence_json["name"], name_hash) ) {
                return stdex::make_rejected_promise<vector<flipbook::sequence>>(
                    flipbook_asset_loading_exception());
            }

            vector<std::size_t> frames;
            if ( !json_utils::try_parse_values(sequence_json["frames"], frames) ) {
                return stdex::make_rejected_promise<vector<flipbook::sequence>>(
                    flipbook_asset_loading_exception());
            }

            flipbook::sequence sequence;
            sequence.fps = fps;
            sequence.name = name_hash;
            sequence.frames = std::move(frames);
            sequences.push_back(std::move(sequence));
        }

        return stdex::make_resolved_promise(sequences);
    }

    stdex::promise<flipbook> parse_flipbook(
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        auto frames_p = root.HasMember("frames")
            ? parse_flipbook_frames(library, parent_address, root["frames"])
            : stdex::make_resolved_promise(vector<flipbook::frame>());

        auto sequences_p = root.HasMember("sequences")
            ? parse_flipbook_sequences(root["sequences"])
            : stdex::make_resolved_promise(vector<flipbook::sequence>());

        return stdex::make_tuple_promise(std::make_tuple(
            std::move(frames_p),
            std::move(sequences_p)))
        .then([](const std::tuple<
            vector<flipbook::frame>,
            vector<flipbook::sequence>
        >& results){
            flipbook content;
            content.set_frames(std::get<0>(results));
            content.set_sequences(std::get<1>(results));
            return content;
        });
    }
}

namespace e2d
{
    flipbook_asset::load_async_result flipbook_asset::load_async(
        const library& library, str_view address)
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
