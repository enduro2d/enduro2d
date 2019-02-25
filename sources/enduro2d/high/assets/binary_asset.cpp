/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets/assets.hpp>

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
        library& library, str_view address)
    {
        E2D_UNUSED(library);

        if ( !modules::is_initialized<vfs>() ) {
            return stdex::make_rejected_promise<load_result>(
                binary_asset_loading_exception());
        }

        const auto asset_url = library.root() / address;
        return the<vfs>().load_async(asset_url)
            .then([](auto&& content){
                return std::make_shared<binary_asset>(
                    std::forward<decltype(content)>(content));
            });
    }
}
