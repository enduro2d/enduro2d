/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/font_asset.hpp>
#include <enduro2d/high/assets/binary_asset.hpp>

namespace
{
    using namespace e2d;

    class font_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "font asset loading exception";
        }
    };
}

namespace e2d
{
    font_asset::load_async_result font_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<binary_asset>(address)
        .then([](const binary_asset::load_result& font_data){
            return the<deferrer>().do_in_worker_thread([font_data](){
                font content;
                if ( !fonts::try_load_font(content, font_data->content()) ) {
                    throw font_asset_loading_exception();
                }
                return font_asset::create(std::move(content));
            });
        });
    }
}
