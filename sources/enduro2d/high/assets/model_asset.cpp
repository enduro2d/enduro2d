/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets/model_asset.hpp>

#include "json_asset.hpp"

namespace
{
    using namespace e2d;

    class model_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "model asset loading exception";
        }
    };
}

namespace e2d
{
    model_asset::load_async_result model_asset::load_async(
        library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
            .then([](const json_asset::load_result& model_data){
                model mdl;
                return model_asset::create(std::move(mdl));
            });
    }
}
