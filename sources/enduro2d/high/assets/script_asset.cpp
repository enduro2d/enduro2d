/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/script_asset.hpp>
#include <enduro2d/high/assets/binary_asset.hpp>

namespace
{
    using namespace e2d;

    class script_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "script asset loading exception";
        }
    };
}

namespace e2d
{
    script_asset::load_async_result script_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<binary_asset>(address)
        .then([](const binary_asset::load_result& script_data){
            script content;
            return script_asset::create(std::move(content));
        });
    }
}
