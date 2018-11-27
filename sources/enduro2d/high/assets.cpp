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

    std::shared_ptr<text_asset> text_asset::load(library& library, const url& url) {
        E2D_UNUSED(library);

        input_stream_uptr stream = the<vfs>().open(url);
        if ( !stream ) {
            the<debug>().error("ASSETS: Failed to open text asset file:\n"
                "--> Url: %0",
                url);
            return nullptr;
        }

        str content;
        if ( !streams::try_read_tail(content, stream) ) {
            the<debug>().error("ASSETS: Failed to read text asset file:\n"
                "--> Url: %0",
                url);
            return nullptr;
        }

        return std::make_shared<text_asset>(std::move(content));
    }

    //
    // binary_asset
    //

    std::shared_ptr<binary_asset> binary_asset::load(library& library, const url& url) {
        E2D_UNUSED(library);

        input_stream_uptr stream = the<vfs>().open(url);
        if ( !stream ) {
            the<debug>().error("ASSETS: Failed to open binary asset file:\n"
                "--> Url: %0",
                url);
            return nullptr;
        }

        buffer content;
        if ( !streams::try_read_tail(content, stream) ) {
            the<debug>().error("ASSETS: Failed to read binary asset file:\n"
                "--> Url: %0",
                url);
            return nullptr;
        }

        return std::make_shared<binary_asset>(std::move(content));
    }
}
