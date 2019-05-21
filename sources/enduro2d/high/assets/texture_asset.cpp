/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/texture_asset.hpp>
#include <enduro2d/high/assets/image_asset.hpp>

namespace
{
    using namespace e2d;

    class texture_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "texture asset loading exception";
        }
    };
}

namespace e2d
{
    texture_asset::load_async_result texture_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<image_asset>(address)
        .then([](const image_asset::load_result& texture_data){
            return the<deferrer>().do_in_main_thread([texture_data](){
                const texture_ptr content = the<render>().create_texture(
                    texture_data->content());
                if ( !content ) {
                    throw texture_asset_loading_exception();
                }
                return texture_asset::create(content);
            });
        });
    }
}
