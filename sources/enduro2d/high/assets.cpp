/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets.hpp>

namespace
{
    using namespace e2d;
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
