/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets/image_asset.hpp>
#include <enduro2d/high/assets/binary_asset.hpp>

namespace
{
    using namespace e2d;

    class image_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "image asset loading exception";
        }
    };
}

namespace e2d
{
    image_asset::load_async_result image_asset::load_async(
        library& library, str_view address)
    {
        return library.load_asset_async<binary_asset>(address)
            .then([](const binary_asset::load_result& image_data){
                if ( !modules::is_initialized<deferrer>() ) {
                    throw image_asset_loading_exception();
                }
                return the<deferrer>().do_in_worker_thread([image_data](){
                    image content;
                    if ( !images::try_load_image(content, image_data->content()) ) {
                        throw image_asset_loading_exception();
                    }
                    return image_asset::create(std::move(content));
                });
            });
    }
}
