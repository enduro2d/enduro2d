/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "assets.hpp"

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
        library& library, str_view address)
    {
        return library.load_asset_async<binary_asset>(address)
            .then([](const binary_asset::ptr& mesh_data){
                if ( !modules::is_initialized<deferrer>() ) {
                    throw mesh_asset_loading_exception();
                }
                return the<deferrer>().do_in_worker_thread([mesh_data](){
                    mesh content;
                    if ( !meshes::try_load_mesh(content, mesh_data->content()) ) {
                        throw mesh_asset_loading_exception();
                    }
                    return std::make_shared<mesh_asset>(std::move(content));
                });
            });
    }
}
