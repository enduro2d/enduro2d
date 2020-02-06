/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/binary_asset.hpp>

namespace
{
    using namespace e2d;

    class binary_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "binary asset loading exception";
        }
    };
}

namespace e2d
{
    binary_asset::load_async_result binary_asset::load_async(
        const library& library, str_view address)
    {
        const auto asset_url = library.root() / address;
        return the<vfs>().load_async(asset_url)
        .then([](auto&& content){
            return binary_asset::create(
                std::forward<decltype(content)>(content));
        });
    }
}
