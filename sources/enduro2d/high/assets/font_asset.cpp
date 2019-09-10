/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/font_asset.hpp>

#include <enduro2d/high/assets/binary_asset.hpp>
#include <enduro2d/high/assets/texture_asset.hpp>

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
                return content;
            });
        })
        .then([
            &library,
            parent_address = path::parent_path(address)
        ](const font& content){
            return stdex::make_tuple_promise(std::make_tuple(
                stdex::make_resolved_promise(content),
                library.load_asset_async<texture_asset>(
                    path::combine(parent_address, content.info().atlas_file))
            ));
        })
        .then([](const std::tuple<
            font,
            texture_asset::load_result
        >& results){
            font content = std::get<0>(results);
            texture_asset::load_result texture_res = std::get<1>(results);
            if ( content.info().atlas_size != texture_res->content()->size() ) {
                throw font_asset_loading_exception();
            }
            nested_content ncontent{{
                make_hash(content.info().atlas_file),
                std::move(texture_res)}};
            return font_asset::create(std::move(content), std::move(ncontent));
        });
    }
}
