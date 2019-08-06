/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/spine_model_asset.hpp>
#include <enduro2d/high/assets/json_asset.hpp>
#include <enduro2d/high/assets/binary_asset.hpp>
#include <enduro2d/high/assets/texture_asset.hpp>

#include <spine/SkeletonJson.h>
#include <spine/SkeletonBinary.h>
#include <spine/extension.h>

using namespace e2d;

namespace
{
    class spine_model_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "spine model asset loading exception";
        }
    };

    const char* spine_model_asset_schema_source = R"json({
        "type" : "object",
        "required" : [ "skeleton", "atlas" ],
        "additionalProperties" : false,
        "properties" : {
            "skeleton" : { "$ref" : "#/common_definitions/address" },
            "scale" : { "type" : "number" },
            "atlas" : { "$ref" : "#/common_definitions/address" },
            "premultiplied_alpha" : { "type" : "boolean" },
            "default_mix" : { "type" : "number" },
            "mix_animations" : { "$ref": "#/definitions/spine_animation_mix_array" }
        },
        "definitions" : {
            "spine_animation_mix_array" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/spine_animation_mix" }
            },
            "spine_animation_mix" : {
                "type" : "object",
                "required" : [ "from_anim", "to_anim", "duration" ],
                "additionalProperties" : false,
                "properties" : {
                    "from_anim" : { "$ref": "#/common_definitions/name" },
                    "to_anim" : { "$ref": "#/common_definitions/name" },
                    "duration" : { "type" : "number" }
                }
            }
        }
    })json";

    const rapidjson::SchemaDocument& spine_model_asset_schema() {
        static std::mutex mutex;
        static std::unique_ptr<rapidjson::SchemaDocument> schema;

        std::lock_guard<std::mutex> guard(mutex);
        if ( !schema ) {
            rapidjson::Document doc;
            if ( doc.Parse(spine_model_asset_schema_source).HasParseError() ) {
                the<debug>().error("SPINE: Failed to parse spine model asset schema");
                throw spine_model_asset_loading_exception();
            }
            json_utils::add_common_schema_definitions(doc);
            schema = std::make_unique<rapidjson::SchemaDocument>(doc);
        }

        return *schema;
    }

    bool parse_mix_animations(
        const rapidjson::Value& root,
        spine_model& model)
    {
        E2D_ASSERT(root.IsArray());
        str from_anim;
        str to_anim;
        secf duration;

        for ( rapidjson::SizeType i = 0; i < root.Size(); ++i ) {
            E2D_ASSERT(root[i].IsObject());
            const auto& item_json = root[i];

            E2D_ASSERT(item_json.HasMember("from_anim"));
            if ( !json_utils::try_parse_value(item_json["from_anim"], from_anim) ) {
                the<debug>().error("SPINE: Incorrect formatting of 'from_anim' property");
                return false;
            }

            E2D_ASSERT(item_json.HasMember("to_anim"));
            if ( !json_utils::try_parse_value(item_json["to_anim"], to_anim) ) {
                the<debug>().error("SPINE: Incorrect formatting of 'to_anim' property");
                return false;
            }

            E2D_ASSERT(item_json.HasMember("duration"));
            if ( !json_utils::try_parse_value(item_json["duration"], duration.value) ) {
                the<debug>().error("SPINE: Incorrect formatting of 'duration' property");
                return false;
            }

            model.mix_animations(from_anim, to_anim, duration);
        }
        return true;
    }

    stdex::promise<spine_model> parse_spine_model(
        const library& library,
        const str& parent_address,
        const rapidjson::Value& root)
    {
        using skeleton_json_ptr = std::unique_ptr<spSkeletonJson, void(*)(spSkeletonJson*)>;
        using skeleton_bin_ptr = std::unique_ptr<spSkeletonBinary, void(*)(spSkeletonBinary*)>;

        E2D_ASSERT(root.HasMember("atlas") && root["atlas"].IsString());
        binary_asset::load_result atlas_data = library.load_asset<binary_asset>(
            path::combine(parent_address, root["atlas"].GetString()));
        spine_model::atlas_ptr atlas(
            spAtlas_create(
                reinterpret_cast<const char*>(atlas_data->content().data()),
                math::numeric_cast<int>(atlas_data->content().size()),
                parent_address.data(),
                nullptr),
            spAtlas_dispose);

        float skeleton_scale = 1.0f;
        if ( root.HasMember("scale") ) {
            if ( !json_utils::try_parse_value(root["scale"], skeleton_scale) ) {
                the<debug>().error("SPINE: Incorrect formating of 'scale' property");
            }
        }

        E2D_ASSERT(root.HasMember("skeleton") && root["skeleton"].IsString());
        const str ext = path::extension(root["skeleton"].GetString());
        spine_model::skeleton_data_ptr skeleton;

        if ( ext == ".skel" ) {
	        skeleton_bin_ptr skeleton_binary(
                spSkeletonBinary_create(atlas.get()),
                spSkeletonBinary_dispose);
            skeleton_binary->scale = skeleton_scale;

            binary_asset::load_result skeleton_data = library.load_asset<binary_asset>(
                path::combine(parent_address, root["skeleton"].GetString()));
            skeleton = spine_model::skeleton_data_ptr(
                spSkeletonBinary_readSkeletonData(
                    skeleton_binary.get(),
                    reinterpret_cast<const unsigned char*>(skeleton_data->content().data()),
                    math::numeric_cast<int>(skeleton_data->content().size())),
                spSkeletonData_dispose);

            if ( !skeleton ) {
                the<debug>().error("SPINE: Failed to read skeleton binary data:\n"
                    "--> Error: $s",
                    skeleton_binary->error);
                return stdex::make_rejected_promise<spine_model>(
                    spine_model_asset_loading_exception());
            }
        } else {
            skeleton_json_ptr skeleton_json(
                spSkeletonJson_create(atlas.get()),
                spSkeletonJson_dispose);
            skeleton_json->scale = skeleton_scale;

            binary_asset::load_result skeleton_data = library.load_asset<binary_asset>(
                path::combine(parent_address, root["skeleton"].GetString()));
            skeleton = spine_model::skeleton_data_ptr(
                spSkeletonJson_readSkeletonData(
                    skeleton_json.get(),
                    reinterpret_cast<const char*>(skeleton_data->content().data())),
                spSkeletonData_dispose);

            if ( !skeleton ) {
                the<debug>().error("SPINE: Failed to read skeleton json data:\n"
                    "--> Error: $s",
                    skeleton_json->error);
                return stdex::make_rejected_promise<spine_model>(
                    spine_model_asset_loading_exception());
            }
        }


        bool pma = false;
        if ( root.HasMember("premultiplied_alpha") ) {
            if ( !json_utils::try_parse_value(root["premultiplied_alpha"], pma) ) {
                the<debug>().error("SPINE: Incorrect formating of 'premultiplied_alpha' property");
            }
        }
        
        spine_model content;
        content.set_atlas(atlas, pma);
        content.set_skeleton(skeleton);

        secf default_mix(0.0f);
        if ( root.HasMember("default_mix") ) {
            if ( json_utils::try_parse_value(root["default_mix"], default_mix.value) ) {
                content.set_default_mix(default_mix);
            } else {
                the<debug>().error("SPINE: Incorrect formating of 'default_mix' property");
            } 
        }

        if ( root.HasMember("mix_animations") ) {
            const auto& mix_animations_json = root["mix_animations"];
            if ( !parse_mix_animations(mix_animations_json, content) ) {
                return stdex::make_rejected_promise<spine_model>(
                    spine_model_asset_loading_exception());
            }
        }

        return stdex::make_resolved_promise(std::move(content));
    }
}

extern "C" void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {
    try {
        texture_asset::load_result texture = the<library>().load_asset<texture_asset>(path);
        if ( !texture ) {
            throw;
        }
        self->width = texture->content()->size().x;
        self->height = texture->content()->size().y;
        self->rendererObject = texture.release();
    } catch(...) {
        the<debug>().error("SPINE: Failed to load atlas texture");
    }
}

extern "C" void _spAtlasPage_disposeTexture (spAtlasPage* self) {
    // decrease ref counter
    E2D_UNUSED(texture_asset::ptr(static_cast<texture_asset*>(self->rendererObject), false));
}

extern "C" char* _spUtil_readFile (const char* path, int* length) {
    try {
        binary_asset::load_result file = the<library>().load_asset<binary_asset>(path);
        if ( !file ) {
            throw;
        }
        if ( file->content().size() > std::numeric_limits<int>::max() ) {
            throw;
        }
        *length = math::numeric_cast<int>(file->content().size());
	    char*  data = MALLOC(char, *length);
        if ( !data ) {
            throw;
        }
        memcpy(data, file->content().data(), *length);
        return data;
    } catch(...) {
        the<debug>().error("SPINE: Failed to read file");
    }
    return nullptr;
}

namespace e2d
{
    spine_model_asset::load_async_result spine_model_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
        .then([
            &library,
            address = str(address),
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& spine_data){
            return the<deferrer>().do_in_worker_thread([address, spine_data](){
                const rapidjson::Document& doc = *spine_data->content();
                rapidjson::SchemaValidator validator(spine_model_asset_schema());

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

                throw spine_model_asset_loading_exception();
            })
            .then([&library, parent_address, spine_data](){
                return parse_spine_model(
                    library, parent_address, *spine_data->content());
            })
            .then([](auto&& content){
                return spine_model_asset::create(
                    std::forward<decltype(content)>(content));
            });
        });
    }
}
