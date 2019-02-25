/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "xml_asset.hpp"

namespace
{
    using namespace e2d;

    class xml_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "xml asset loading exception";
        }
    };
}

namespace e2d
{
    xml_asset::load_async_result xml_asset::load_async(
        library& library, str_view address)
    {
        return library.load_asset_async<text_asset>(address)
            .then([](const text_asset::load_result& xml_data){
                if ( !modules::is_initialized<deferrer>() ) {
                    throw xml_asset_loading_exception();
                }
                return the<deferrer>().do_in_worker_thread([xml_data](){
                    pugi::xml_document doc;
                    if ( !doc.load_string(xml_data->content().c_str()) ) {
                        throw xml_asset_loading_exception();
                    }
                    return xml_asset::create(std::move(doc));
                });
            });
    }
}
