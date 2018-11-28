/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets.hpp>

#include <3rdparty/rapidjson/schema.h>
#include <3rdparty/rapidjson/document.h>

namespace
{
    using namespace e2d;

    template < typename T >
    const char* asset_schema_source() noexcept;

    template <>
    const char* asset_schema_source<shader_asset>() noexcept {
        return R"json({
            "type" : "object",
            "required" : [ "vertex", "fragment" ],
            "properties" : {
                "vertex" : { "type" : "string", "minLength" : 1 },
                "fragment" : { "type" : "string", "minLength" : 1 }
            }
        })json";
    }

    template < typename T >
    const rapidjson::SchemaDocument& asset_file_schema() {
        static std::unique_ptr<rapidjson::SchemaDocument> schema;
        if ( !schema ) {
            rapidjson::Document doc;
            if ( doc.Parse(asset_schema_source<T>()).HasParseError() ) {
                the<debug>().error("ASSETS: Failed to parse asset file schema");
                throw bad_library_operation();
            }
            schema = std::make_unique<rapidjson::SchemaDocument>(doc);
        }
        return *schema;
    }

    template < typename T >
    bool load_json_asset_file(library& library, str_view address, rapidjson::Document& doc) {
        const auto json_data = library.load_asset<text_asset>(address);
        if ( !json_data ) {
            return false;
        }

        if ( doc.Parse(json_data->content().c_str()).HasParseError() ) {
            the<debug>().error("ASSETS: Failed to parse json asset file:\n"
                "--> Address: %0",
                address);
            return false;
        }

        rapidjson::SchemaValidator validator(asset_file_schema<T>());
        if ( !doc.Accept(validator) ) {
            the<debug>().error("ASSETS: Failed to validate json asset file:\n"
                "--> Address: %0",
                address);
            return false;
        }

        return true;
    }
}

namespace e2d
{
    //
    // text_asset
    //

    std::shared_ptr<text_asset> text_asset::load(library& library, str_view address) {
        E2D_UNUSED(library);

        const auto asset_url = library.root() / address;
        input_stream_uptr stream = the<vfs>().open(asset_url);
        if ( !stream ) {
            the<debug>().error("ASSETS: Failed to open text asset file:\n"
                "--> Url: %0",
                asset_url);
            return nullptr;
        }

        str content;
        if ( !streams::try_read_tail(content, stream) ) {
            the<debug>().error("ASSETS: Failed to read text asset file:\n"
                "--> Url: %0",
                asset_url);
            return nullptr;
        }

        return std::make_shared<text_asset>(std::move(content));
    }

    //
    // image_asset
    //

    std::shared_ptr<image_asset> image_asset::load(library& library, str_view address) {
        const auto image_data = library.load_asset<binary_asset>(address);
        if ( !image_data ) {
            return nullptr;
        }

        image content;
        if ( !images::try_load_image(content, image_data->content()) ) {
            the<debug>().error("ASSETS: Failed to create image asset:\n"
                "--> Address: %0",
                address);
            return nullptr;
        }

        return std::make_shared<image_asset>(std::move(content));
    }

    //
    // binary_asset
    //

    std::shared_ptr<binary_asset> binary_asset::load(library& library, str_view address) {
        E2D_UNUSED(library);

        const auto asset_url = library.root() / address;
        input_stream_uptr stream = the<vfs>().open(asset_url);
        if ( !stream ) {
            the<debug>().error("ASSETS: Failed to open binary asset file:\n"
                "--> Url: %0",
                asset_url);
            return nullptr;
        }

        buffer content;
        if ( !streams::try_read_tail(content, stream) ) {
            the<debug>().error("ASSETS: Failed to read binary asset file:\n"
                "--> Url: %0",
                asset_url);
            return nullptr;
        }

        return std::make_shared<binary_asset>(std::move(content));
    }

    //
    // shader_asset
    //

    std::shared_ptr<shader_asset> shader_asset::load(library& library, str_view address) {
        rapidjson::Document doc;
        if ( !load_json_asset_file<shader_asset>(library, address, doc) ) {
            return nullptr;
        }

        const auto parent_address = path::parent_path(address);

        E2D_ASSERT(doc.HasMember("vertex") && doc["vertex"].IsString());
        const auto vertex_source_data = library.load_asset<text_asset>(
            path::combine(parent_address, doc["vertex"].GetString()));

        if ( !vertex_source_data ) {
            return nullptr;
        }

        E2D_ASSERT(doc.HasMember("fragment") && doc["fragment"].IsString());
        const auto fragment_source_data = library.load_asset<text_asset>(
            path::combine(parent_address, doc["fragment"].GetString()));

        if ( !fragment_source_data ) {
            return nullptr;
        }

        const auto content = the<render>().create_shader(
            vertex_source_data->content(),
            fragment_source_data->content());

        if ( !content ) {
            the<debug>().error("ASSETS: Failed to create shader asset:\n"
                "--> Address: %0",
                address);
            return nullptr;
        }

        return std::make_shared<shader_asset>(content);
    }

    //
    // texture_asset
    //

    std::shared_ptr<texture_asset> texture_asset::load(library& library, str_view address) {
        const auto texture_data = library.load_asset<image_asset>(address);
        if ( !texture_data ) {
            return nullptr;
        }

        const auto content = the<render>().create_texture(texture_data->content());
        if ( !content ) {
            the<debug>().error("ASSETS: Failed to create texture asset:\n"
                "--> Address: %0",
                address);
            return nullptr;
        }

        return std::make_shared<texture_asset>(content);
    }
}
