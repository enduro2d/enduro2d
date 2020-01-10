/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/script_asset.hpp>

#include <enduro2d/high/luasol.hpp>
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
        .then([
            address = str(address)
        ](const binary_asset::load_result& script_data){
            return the<deferrer>().do_in_main_thread([
                script_data,
                address = std::move(address)
            ](){
                E2D_PROFILER_SCOPE_EX("script_asset.parsing", {
                    {"address", address}
                });
                std::optional<script> script_opt = the<luasol>().load_script(
                    script_data->content());
                if ( !script_opt ) {
                    throw script_asset_loading_exception();
                }
                return script_asset::create(std::move(*script_opt));
            });
        });
    }
}
