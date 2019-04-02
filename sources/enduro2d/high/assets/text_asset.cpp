/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets/text_asset.hpp>

namespace
{
    using namespace e2d;

    class text_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "text asset loading exception";
        }
    };
}

namespace e2d
{
    text_asset::load_async_result text_asset::load_async(
        const library& library, str_view address)
    {
        const auto asset_url = library.root() / address;
        return the<vfs>().load_as_string_async(asset_url)
            .then([](auto&& content){
                return text_asset::create(
                    std::forward<decltype(content)>(content));
            });
    }
}
