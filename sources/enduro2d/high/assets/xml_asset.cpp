/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/xml_asset.hpp>
#include <enduro2d/high/assets/text_asset.hpp>

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
        const library& library, str_view address)
    {
        return library.load_asset_async<text_asset>(address)
        .then([
            address = str(address)
        ](const text_asset::load_result& xml_data){
            return the<deferrer>().do_in_worker_thread([
                xml_data,
                address = std::move(address)
            ](){
                auto xml = std::make_shared<pugi::xml_document>();
                if ( !xml->load_string(xml_data->content().c_str()) ) {
                    throw xml_asset_loading_exception();
                }
                return xml_asset::create(std::move(xml));
            });
        });
    }
}
