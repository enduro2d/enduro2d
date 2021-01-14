/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/mesh_asset.hpp>
#include <enduro2d/high/assets/binary_asset.hpp>

namespace
{
    using namespace e2d;

    class mesh_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "mesh asset loading exception";
        }
    };
}

namespace e2d
{
    mesh_asset::load_async_result mesh_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<binary_asset>(address)
        .then([
            address = str(address)
        ](const binary_asset::load_result& mesh_data){
            return the<deferrer>().do_in_worker_thread([
                mesh_data,
                address = std::move(address)
            ](){
                mesh content;
                if ( !meshes::try_load_mesh(content, mesh_data->content()) ) {
                    throw mesh_asset_loading_exception();
                }
                return mesh_asset::create(std::move(content));
            });
        });
    }
}
