/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include <enduro2d/high/assets.hpp>

#include <3rdparty/pugixml/pugixml.hpp>

#include <3rdparty/rapidjson/schema.h>
#include <3rdparty/rapidjson/document.h>

namespace e2d
{
    class xml_asset final : public content_asset<xml_asset, pugi::xml_document> {
    public:
        using content_asset<xml_asset, pugi::xml_document>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };

    class json_asset final : public content_asset<json_asset, rapidjson::Document> {
    public:
        using content_asset<json_asset, rapidjson::Document>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };
}
