/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/bmfont_asset.hpp>
#include <enduro2d/high/assets/text_asset.hpp>

namespace
{
    using namespace e2d;

    class bmfont_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "shader asset loading exception";
        }
    };
}

namespace e2d
{
    bmfont_asset::load_async_result bmfont_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<text_asset>(address)
        .then([](const text_asset::load_result& text_data){
            return the<deferrer>().do_in_main_thread([text_data](){
                auto font = bmfont::create(text_data->content());
                if ( !font ) {
                    throw bmfont_asset_loading_exception();
                }
                return bmfont_asset::create(font);
            });
        });
    }
}
