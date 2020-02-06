/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/shape_asset.hpp>
#include <enduro2d/high/assets/binary_asset.hpp>

namespace
{
    using namespace e2d;

    class shape_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "shape asset loading exception";
        }
    };
}

namespace e2d
{
    shape_asset::load_async_result shape_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<binary_asset>(address)
        .then([
            address = str(address)
        ](const binary_asset::load_result& shape_data){
            return the<deferrer>().do_in_worker_thread([
                shape_data,
                address = std::move(address)
            ](){
                E2D_PROFILER_SCOPE_EX("shape_asset.parsing", {
                    {"address", address}
                });
                shape content;
                if ( !shapes::try_load_shape(content, shape_data->content()) ) {
                    throw shape_asset_loading_exception();
                }
                return shape_asset::create(std::move(content));
            });
        });
    }
}
