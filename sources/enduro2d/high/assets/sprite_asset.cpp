/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets/assets.hpp>

#include "json_asset.hpp"

namespace
{
    using namespace e2d;

    class sprite_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "sprite asset loading exception";
        }
    };
}

namespace e2d
{
    sprite_asset::load_async_result sprite_asset::load_async(
        library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
            .then([](const json_asset::load_result& sprite_data){
                sprite spr;
                return sprite_asset::create(std::move(spr));
            });
    }
}
