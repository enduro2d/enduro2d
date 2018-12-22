/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "assets.hpp"

namespace
{
    using namespace e2d;

    class json_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "json asset loading exception";
        }
    };
}

namespace e2d
{
    json_asset::load_async_result json_asset::load_async(
        library& library, str_view address)
    {
        return library.load_asset_async<text_asset>(address)
            .then([](const text_asset::ptr& json_data){
                if ( !modules::is_initialized<deferrer>() ) {
                    throw json_asset_loading_exception();
                }
                return the<deferrer>().do_in_worker_thread([json_data](){
                    rapidjson::Document doc;
                    if ( doc.Parse(json_data->content().c_str()).HasParseError() ) {
                        throw json_asset_loading_exception();
                    }
                    return std::make_shared<json_asset>(std::move(doc));
                });
            });
    }
}
